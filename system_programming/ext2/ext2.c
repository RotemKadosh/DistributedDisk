#include <fcntl.h>/*open, close, O_RDONLY*/
#include <stdio.h>/*printf*/
#include <stdlib.h> /*malloc free*/
#include <string.h>/*memcpy, strdup, strtok*/
#include <unistd.h> /*lseek, read*/
#include <assert.h> /*assert*/
#include "ex2fs.h" /*super_block_ty, group_desc_ty, i_node_ty, dir_entry_ty*/
#include "ext2.h"

#define BASIC_BLOCK_SIZE (1024)
#define BASE_OFFSET (1024)
#define FAIL (-1)
#define SUCCESS (0)
#define ROOT_INODE (2)
#define FIRST_GROUP_BLOCK (0)
typedef struct ext2_super_block super_block_ty;
typedef struct ext2_group_desc group_desc_ty;
typedef struct ext2_inode i_node_ty;
typedef struct ext2_dir_entry_2 dir_entry_ty;

static int GetBlockDescriptor(int fd, group_desc_ty *group, int block_group_num, size_t super_block_size);
static void PrintBlockDescriptor(const group_desc_ty *group, const char *dev);
static int GetSuperBlock(int fd, super_block_ty *super);
static void PrintSuperBlock(const super_block_ty *super, const char *dev, size_t super_block_size);
static int ReadInode(int fd, i_node_ty *inode, int inode_num, group_desc_ty *group,size_t super_block_size);
static int PrintData(int fd, unsigned int data_offset, size_t block_size);
static unsigned int GetDataBlock(int fd, i_node_ty *inode, group_desc_ty *group, const char *c_file_path, super_block_ty *super, size_t block_size);

char * strdup(const char *src);

#define BLOCK_OFFSET(block, block_size) (BASE_OFFSET + (block - 1) * block_size)
 
int Find(const char *dev, const char *file_path)
{
	int fd = 0;
    super_block_ty super = {0};
    size_t super_block_size = 0;
    group_desc_ty group = {0};
    i_node_ty inode  = {0};
    unsigned int data_offset = 0;

    assert(NULL != dev);
    assert(NULL != file_path);

    if ((fd = open(dev, O_RDONLY)) < 0)
    {
		perror(dev);
		return FAIL;  
	}
    if(FAIL == GetSuperBlock(fd, &super))
    {
        close(fd);
        return FAIL;
    }
		
	super_block_size = BASIC_BLOCK_SIZE << super.s_log_block_size;

    PrintSuperBlock(&super, dev, super_block_size);

    if(FAIL == GetBlockDescriptor(fd, &group, FIRST_GROUP_BLOCK, super_block_size))
    {
        close(fd);
        return FAIL;
    }
    
    if(FAIL == ReadInode(fd, &inode, ROOT_INODE , &group,super_block_size))
    {
        close(fd);
        return FAIL;
    }

    data_offset = GetDataBlock(fd, &inode, &group, file_path, &super, super_block_size);
    if((FAIL == ( int)data_offset))
    {
        close(fd);
        return FAIL;
    }

    PrintBlockDescriptor(&group, dev);

    if(FAIL == PrintData(fd, data_offset, super_block_size))
    {
        close(fd);
        return FAIL;
    }

    if (SUCCESS != close(fd))
    {
        perror("closing fail:");
    }

    return SUCCESS;
}

static int PrintData(int fd, unsigned int data_offset, size_t block_size)
{

    char *block = (char *)malloc(block_size);
	if (NULL == block) 
	{
		perror("Allocation error\n");
		close(fd);
		return FAIL;
	}

	if((unsigned int)(BLOCK_OFFSET(data_offset, block_size) )!= 
                    lseek(fd, BLOCK_OFFSET(data_offset, block_size), SEEK_SET))
    {
        perror("lseek error\n");
		close(fd);
        free (block);
        block = NULL;
		return FAIL;
    }
	if(FAIL == read(fd, block, block_size))
    {
        perror("read error\n");
		close(fd);
        free (block);
        block = NULL;
		return FAIL;
    }

	printf("%s", block);
	free(block);
	block = NULL;
    return SUCCESS;

}

static unsigned int GetInode(int fd, const i_node_ty *parent_inode,
                 char *file_name_buffer, const char *file_name, size_t block_size)
{
	dir_entry_ty *entry = NULL;
	unsigned int size = 0;
	unsigned int found_inode = 0;
    void *block = NULL;
	
    assert(NULL != parent_inode);
    assert(NULL != file_name_buffer);
    assert(NULL != file_name);

    block = malloc(block_size);
	if (NULL == block) 
	{
		perror( "Allocation error\n");
		close(fd);
        free(block);
        block = NULL;
		exit(1);
	}

	if((int)(BLOCK_OFFSET(parent_inode->i_block[0], block_size) )!=
             (int)lseek(fd, BLOCK_OFFSET(parent_inode->i_block[0], block_size),
                 SEEK_SET))
    {
        perror( "lseek error\n");
        close(fd);
        free(block);
        block = NULL;
        exit(1);
    }
	if(FAIL == read(fd, block, block_size))
    {
        perror( "read error\n");
        close(fd);
        free(block);
        block = NULL;
        exit(1);
    }

	entry = (dir_entry_ty *)block;  
	while(size < parent_inode->i_size)
	{
		memcpy(file_name_buffer, entry->name, entry->name_len);
		file_name_buffer[entry->name_len] = '\0';

		if (0 == strcmp(file_name, file_name_buffer))
		{
			found_inode = entry->inode;
			break;
		}
		size += entry->rec_len;
		entry = (dir_entry_ty *)((char *)entry + entry->rec_len);
	}

	free(block);
	block = NULL;

	return found_inode;
}

static unsigned int GetDataBlock(int fd, i_node_ty *inode, group_desc_ty *group, 
                const char *c_file_path, super_block_ty *super, size_t block_size)
{
	char *delims = "/";
	char *token = NULL;
	char file_name_buffer[EXT2_NAME_LEN+1] = {0};
	int inode_num = 0;
	unsigned int group_number = 0;
    int local_inode_index = 0;
    char *file_path = NULL;

    assert(NULL != inode);
    assert(NULL != group);
    assert(NULL != c_file_path);
    assert(NULL != super);
    file_path = strdup(c_file_path);
    if(NULL == file_path)
    {
        return FAIL;
    }
    
	token = strtok(file_path, delims);

	while (NULL != token)
	{
		inode_num = GetInode(fd, inode,file_name_buffer, token, block_size);
		group_number = (inode_num - 1) / super->s_inodes_per_group;
        local_inode_index = (inode_num) % super->s_inodes_per_group;
		if(FAIL == GetBlockDescriptor(fd ,group, group_number, block_size) )
        {
            free(file_path);
            file_path = NULL;
            return FAIL;
        }
        if( FAIL == ReadInode(fd ,inode, local_inode_index, group, block_size))
        {
            free(file_path);
            file_path = NULL;
            return FAIL;
        }
		token = strtok(NULL, delims);
	}
    free(file_path);
    file_path = NULL;
	return inode->i_block[0];
}


static int ReadInode(int fd, i_node_ty *inode, int inode_num, group_desc_ty *group,size_t super_block_size)
{
    int block = group->bg_inode_table;
    int inode_offset = BASE_OFFSET + (block -1) * super_block_size + (inode_num - 1) * sizeof(i_node_ty);

    assert(NULL != inode);
    assert(NULL != group);


    if(inode_offset != lseek(fd, inode_offset, SEEK_SET) )
    {
        perror("lseek error\n");
        return FAIL;
    }
	
    if(FAIL == read(fd, inode, sizeof(i_node_ty)))
    {
        perror("read error\n");
        return FAIL;
    }
    return SUCCESS;
}

static int GetBlockDescriptor(int fd, group_desc_ty *group,
                 int block_group_num, size_t super_block_size)
{
    int block_offset = BASE_OFFSET + super_block_size + (block_group_num * sizeof(group_desc_ty));
   
    assert(NULL != group);

    if(block_offset != lseek(fd, block_offset, SEEK_SET) )
    {
        perror("lseek error\n");
        return FAIL;
    }
    if(FAIL == read(fd, group, sizeof(group_desc_ty)))
    {
        perror("read error\n");
        return FAIL;
    }
    return SUCCESS;
}

static void PrintBlockDescriptor(const group_desc_ty *group, const char *dev)
{
    assert(NULL != dev);
    assert(NULL != group);

    printf("Reading first group-descriptor from device  %s :\n"
	       "Blocks bitmap block: %u\n"
	       "Inodes bitmap block: %u\n"
	       "Inodes table block : %u\n"
	       "Free blocks count  : %u\n"
	       "Free inodes count  : %u\n"
	       "Directories count  : %u\n"
	       ,
           dev,
	       group->bg_block_bitmap,
	       group->bg_inode_bitmap,
	       group->bg_inode_table,
	       group->bg_free_blocks_count,
	       group->bg_free_inodes_count,
	       group->bg_used_dirs_count);   
}


static int GetSuperBlock(int fd, super_block_ty *super)
{
    assert(NULL != super);
    if(BASE_OFFSET != lseek(fd, BASE_OFFSET, SEEK_SET))
    {
        perror("lseek error\n");
        return FAIL;
    }
    if(FAIL == read(fd, super, sizeof(super_block_ty)))
    {
        perror("read error\n");
        return FAIL;
    }
	if (super->s_magic != EXT2_SUPER_MAGIC)
    {
		perror("Not a Ext2 filesystem\n");
		return FAIL;
	}
    return SUCCESS;
}

static void PrintSuperBlock(const super_block_ty *super, const char *dev,
                                                     size_t super_block_size)
{
    assert(NULL != dev);
    assert(NULL != super);

	printf("Reading super-block from device %s :\n"
	       "Inodes count            : %u\n"
	       "Blocks count            : %u\n"
	       "Reserved blocks count   : %u\n"
	       "Free blocks count       : %u\n"
	       "Free inodes count       : %u\n"
	       "First data block        : %u\n"
	       "Block size              : %lu\n"
	       "Blocks per group        : %u\n"
	       "Inodes per group        : %u\n"
	       "Creator OS              : %u\n"
	       "First non-reserved inode: %u\n"
	       "Size of inode structure : %hu\n"
	       ,
           dev,
	       super->s_inodes_count,  
	       super->s_blocks_count,
	       super->s_r_blocks_count,     /* reserved blocks count */
	       super->s_free_blocks_count,
	       super->s_free_inodes_count,
	       super->s_first_data_block,
	       super_block_size,
	       super->s_blocks_per_group,
	       super->s_inodes_per_group,
	       super->s_creator_os,
	       super->s_first_ino,          /* first non-reserved inode */
	       super->s_inode_size);
	

} 



#include <stdlib.h> /*malloc, free*/
#include <string.h> /*strcmp*/
#include "../utils/test.h"
#include "hash.h"

#define FALSE (0)
#define TRUE (1)
#define SUCCESS (1)

static test_status_t TestCreateDestroy(void);
static test_status_t TestSizeIsEmptyInsert(void);
static test_status_t TestRemove(void);
static test_status_t Testfind(void);
static test_status_t TestForeach(void);

typedef struct entry
{
    size_t key;
    size_t data;
} entry_ty;


int main(void)
{

    RUNTEST(TestCreateDestroy);
    RUNTEST(TestSizeIsEmptyInsert);
    RUNTEST(TestRemove);      
    RUNTEST(Testfind); 
    RUNTEST(TestForeach);
    return 0;
}

int Match(const void *data, const void *data_to_compare)
{
    if(((const entry_ty *)data)->key == ((const entry_ty *)data_to_compare)->key )
    {
        return TRUE;
    }
    return FALSE;
}

size_t HashFunc(const void *data)
{
    return ((const entry_ty *)data)->key;
}

typedef struct sentry
{
    char *key;
    char *data;
}s_entry_ty;

int STRaction(void *data, void *param)
{
  ((s_entry_ty *)data)->data = (char *)param;
  return 0;
}

int STRMatch(const void *data, const void *data_to_compare)
{
    if(0 == strcmp(((const s_entry_ty *)data)->key ,((const s_entry_ty *)data_to_compare)->key))
    {
        return TRUE;
    }
    return FALSE;
}


static test_status_t TestCreateDestroy(void)
{
    hash_table_ty *hash = HashCreate(HashFunc, Match, 20);
    REQUIRE(NULL != hash);
    HashDestroy(hash);
    
    return PASSED;
}

static test_status_t TestSizeIsEmptyInsert(void)
{
    entry_ty en = {1, 5};
    entry_ty en1 = {2, 5};
    entry_ty en2 = {3, 5};
    entry_ty en3 = {4, 10};
    entry_ty en4 = {5, 10};
    entry_ty en5 = {6, 10};
    entry_ty en6 = {7, 5};
    entry_ty en7 = {8, 5};
    entry_ty en8 = {9, 5};
    entry_ty en9 = {10, 10};
    entry_ty en10 = {1, 10};
    entry_ty en11 = {2, 10};
    hash_table_ty *hash = HashCreate(HashFunc, Match, 5);
    REQUIRE(NULL != hash);
    REQUIRE(TRUE == HashIsEmpty(hash));
    REQUIRE(HashSize(hash) == 0);
    HashInsert(hash, (void *)&en);
    HashInsert(hash, (void *)&en1);
    HashInsert(hash, (void *)&en2);
    HashInsert(hash, (void *)&en3);
    HashInsert(hash, (void *)&en4);
    REQUIRE(HashSize(hash) == 5);
    REQUIRE(FALSE == HashIsEmpty(hash));   
    HashInsert(hash, (void *)&en5);      
    HashInsert(hash, (void *)&en6);
    HashInsert(hash, (void *)&en7);
    HashInsert(hash, (void *)&en8);
    HashInsert(hash, (void *)&en9);
    REQUIRE(HashSize(hash) == 10); 
    HashInsert(hash, (void *)&en10);
    HashInsert(hash, (void *)&en11);
    REQUIRE(HashSize(hash) == 10);
    REQUIRE(FALSE == HashIsEmpty(hash)); 

    HashDestroy(hash);    
    return PASSED;
}

static test_status_t TestRemove(void)
{
    entry_ty en = {1, 5};
    entry_ty en1 = {2, 5};
    entry_ty en2 = {3, 5};
    entry_ty en3 = {4, 10};
    entry_ty en4 = {5, 10};
    entry_ty en5 = {6, 10};
    entry_ty en6 = {7, 10};
    hash_table_ty *hash = HashCreate(HashFunc, Match, 5);
    REQUIRE(NULL != hash);
    HashInsert(hash, (void *)&en);
    HashInsert(hash, (void *)&en1);
    HashInsert(hash, (void *)&en2);
    HashInsert(hash, (void *)&en3);
    HashInsert(hash, (void *)&en4);
    HashInsert(hash, (void *)&en5);

    HashRemove(hash, (void *)&en);
    REQUIRE(HashSize(hash) == 5);
    HashRemove(hash, (void *)&en1);
    HashRemove(hash, (void *)&en2);
    REQUIRE(HashSize(hash) == 3);
    HashRemove(hash, (void *)&en6);
    REQUIRE(HashSize(hash) == 3);

    HashDestroy(hash);    
    return PASSED;
}

static test_status_t Testfind(void)
{
    entry_ty en = {1, 5};
    entry_ty en1 = {2, 5};
    entry_ty en2 = {3, 5};
    entry_ty en3 = {2, 10};
    entry_ty en4 = {3, 10};
    entry_ty en5 = {4, 10};
    entry_ty en6 = {7, 10};

    entry_ty *found = NULL;
    hash_table_ty *hash = HashCreate(HashFunc, Match, 3);
    REQUIRE(NULL != hash);
    REQUIRE(TRUE == HashIsEmpty(hash));
    REQUIRE(HashSize(hash) == 0);
    HashInsert(hash, (void *)&en);
    REQUIRE(HashSize(hash) == 1);
    HashInsert(hash, (void *)&en1);
    HashInsert(hash, (void *)&en2);
    HashInsert(hash, (void *)&en3);
    HashInsert(hash, (void *)&en4);
    HashInsert(hash, (void *)&en5);     
    REQUIRE(HashSize(hash) == 4);
    found = HashFind(hash, (void *)&en1);
    REQUIRE(found->data == en3.data);
    found = HashFind(hash, (void *)&en2);
    REQUIRE(found->data == en4.data);
    found = HashFind(hash, (void *)&en5);
    REQUIRE(found->data == en5.data);
    found = HashFind(hash, (void *)&en6);
    REQUIRE(found == NULL);

    HashDestroy(hash);    
    return PASSED;
}

static test_status_t TestForeach(void)
{
    int ans = 0;
    char *str = "A";
    s_entry_ty en = {"1", "1"};
    s_entry_ty en1 = {"2", "str2"};
    s_entry_ty en2 = {"3", "str3"};
    s_entry_ty en3 = {"4", "str4"};
    s_entry_ty en4 = {"5", "str5"};
    s_entry_ty en5 = {"6", "str6"};
    s_entry_ty *found = NULL;
    hash_table_ty *hash = HashCreate(HashFunc, Match, 3);
    REQUIRE(NULL != hash);
    REQUIRE(TRUE == HashIsEmpty(hash));
    REQUIRE(HashSize(hash) == 0);
    HashInsert(hash, (void *)&en);
    REQUIRE(HashSize(hash) == 1);
    HashInsert(hash, (void *)&en1);
    HashInsert(hash, (void *)&en2);
    HashInsert(hash, (void *)&en3);
    HashInsert(hash, (void *)&en4);
    HashInsert(hash, (void *)&en5);     

    ans = HashForEach(hash, STRaction, str);
    REQUIRE(0 == ans);   
    found = HashFind(hash, (void *)&en1);
    REQUIRE(0 == strcmp(found->data, str));
    found = HashFind(hash, (void *)&en2);
    REQUIRE(0 == strcmp(found->data, str));
    found = HashFind(hash, (void *)&en3);
    REQUIRE(0 == strcmp(found->data, str));
    found = HashFind(hash, (void *)&en4);
    REQUIRE(0 == strcmp(found->data, str));
    found = HashFind(hash, (void *)&en5);
    REQUIRE(0 == strcmp(found->data, str));
    found = HashFind(hash, (void *)&en);
    REQUIRE(0 == strcmp(found->data, str));
    HashDestroy(hash);    
    return PASSED;
}
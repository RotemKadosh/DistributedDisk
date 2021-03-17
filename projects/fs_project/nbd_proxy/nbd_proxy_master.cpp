#include <boost/make_shared.hpp>
#include <sys/ioctl.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/stat.h>
#include <boost/thread/mutex.hpp>
#include <fcntl.h>
#include <linux/nbd.h>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>
#include "nbdproxy.hpp"
#include "framework.hpp"
#include "singleton.hpp"
#include <netinet/in.h>
#include <inttypes.h>
#include<boost/unordered_map.hpp>
#include <map>
#include <sys/mount.h>
#include "singleton_impl.hpp"
#include <endian.h>
#include <minion_menager.hpp>

namespace ilrd
{

static void read_all(int fd, char* buf, size_t count)
{
    while (count > 0) 
    {
        int bytes_read = read(fd, buf, count);
        buf += bytes_read;
        count -= bytes_read;
    }

}

static void write_all(int fd, char* buf, size_t count)
{

    while (count > 0) 
    {
        int bytes_written = write(fd, buf, count);
        buf += bytes_written;
        count -= bytes_written;
    }
}


class ReadCmd : public Command
{
public:
    ReadCmd(boost::shared_ptr<std::vector<char> > data_);
    virtual ~ReadCmd() noexcept;
    virtual void Run();
};


ReadCmd::ReadCmd(boost::shared_ptr<std::vector<char> > data_):
Command(data_)
{

}

ReadCmd::~ReadCmd() noexcept
{

}

void ReadCmd::Run()
{
    NbdProxy::CmdArgs args = {0};
    size_t len = 4096;
    memcpy(&args, &(*m_data)[0], sizeof(args));
    
    MinionManager *manager = Singleton<MinionManager>::GetInstance();
    manager->Read(args.key, &args.block);
    
    args.reply_func((int)args.fd,(const char *) &args.rep, sizeof(struct nbd_reply));
    args.reply_func((int)args.fd, (const char *)&args.block, len);

}


class WriteCmd : public Command
{
public:
    WriteCmd(boost::shared_ptr<std::vector<char> > data_);
    virtual ~WriteCmd() noexcept;
    virtual void Run();
};

WriteCmd::WriteCmd(boost::shared_ptr<std::vector<char> > data_):
Command(data_)
{

}

WriteCmd::~WriteCmd() noexcept
{

}

void WriteCmd::Run()
{
    
    NbdProxy::CmdArgs args = {0};
    memcpy(&args, &(*m_data)[0], sizeof(args));
    
    MinionManager *manager = Singleton<MinionManager>::GetInstance();
    manager->Write(args.key, &args.block);
    
    args.reply_func((int)args.fd,(const char *) &args.rep, sizeof(struct nbd_reply));
      
}

void NbdProxy::NbdReply(int fd ,const void *block, size_t size)
{

    write_all(fd, (char *)block, size);
}


NbdProxy::NbdProxy(const char *path_, size_t block_size_, size_t number_of_blocks_ ,int socket_fd, int nbd):
m_path(path_),
m_bsize(block_size_),
m_bnum(number_of_blocks_),
nbd_socket_fd(socket_fd),
m_nbd(nbd),
run_thread(boost::bind(&NbdProxy::Connect, this))
{

} // may throw bad_alloc

NbdProxy::~NbdProxy() noexcept
{
    close(nbd_socket_fd);
    run_thread.join();
}


static void InitReply(struct nbd_request *request,CmdArgs *cmd_args, int socket)
{
    cmd_arg->rep.magic = htonl(NBD_REPLY_MAGIC);
    cmd_arg->rep.error = htonl(0);
    memcpy(cmd_arg->rep.handle, request->handle, sizeof(cmd_arg->rep.handle));
    cmd_arg->key = be64toh(request.from);
    cmd_arg->reply_func = NbdProxy::NbdReply;
    cmd_arg->fd = socket;
}

boost::shared_ptr<CmdData> NbdProxy::ParseData(fd_ty sk)
{
    ssize_t bytes_read;
    struct nbd_request request;
    CmdArgs cmd_arg; 
    boost::shared_ptr<CmdData> cmd_data;
    std::string type;

    if((bytes_read = read(sk, &request, sizeof(request))) > 0) 
    {
        InitReply(&request, &cmd_arg, sk);
        size_t len = ntohl(request.len);

        if(ntohl(request.type) == NBD_CMD_READ)
        {
            type = "ReadCmd";
        }
        else
        {
            read_all(sk, cmd_arg.block, len);
            type = "WriteCmd";
        }
        std::vector<char> args;
        args.resize(sizeof(cmd_arg));
        memcpy(&args[0], &cmd_arg, sizeof(cmd_arg));

        cmd_data = boost::shared_ptr<CmdData> ( new CmdData(type, args)); 
    }  
    return cmd_data;
}

boost::shared_ptr<Command> NbdProxy::CreateWriteCmd(std::vector<char> args)
{
    boost::shared_ptr<std::vector<char> > data(new std::vector<char>(args));
    return boost::make_shared<WriteCmd>(data);
}

boost::shared_ptr<Command> NbdProxy::CreateReadCmd(std::vector<char> args)
{
    boost::shared_ptr<std::vector<char> > data(new std::vector<char>(args));
    return boost::make_shared<ReadCmd>(data); 
}

void NbdProxy::Connect()
{

    int err = ioctl(m_nbd, NBD_DO_IT);
    if (err == -1)
    {
        std::cout << "NBD_DO_IT terminated with error\n";
    }
}

void __attribute__((constructor)) ConfigAndRegister();

void ConfigAndRegister()
{

    system ("sudo modprobe nbd");
    system("echo 4 | sudo tee /sys/block/nbd3/queue/max_sectors_kb");
    system("sudo chmod 777 /dev/nbd3");
    
    const char *dev_file = "/dev/nbd3";
    int sp[2];
    int nbd, err;

    err = socketpair(AF_UNIX, SOCK_STREAM, 0, sp);
    if (err == -1)
    {
        std::cout << "socketpair terminated with error\n";
    }

    std::cout << "socketpair\n";
    nbd = open(dev_file, O_RDWR);
    if(-1 == nbd)
    {
        std::cerr << errno;
    }

    err = ioctl(nbd, NBD_SET_BLKSIZE, 4096UL);
    if (err == -1)
    {
        std::cout << "NBD_SET_BLKSIZE terminated with error\n";
    }

    std::cout << "NBD_SET_BLKSIZE\n";
    size_t size = 134217728;
    err = ioctl(nbd, NBD_SET_SIZE, size);
    if (err == -1)
    {
        std::cout << "NBD_SET_SIZE terminated with error\n";
    }

    err = ioctl(nbd, NBD_CLEAR_SOCK);
    if (err == -1)
    {
        std::cout << "NBD_CLEAR_SOCK terminated with error\n";
    }
    //assert(err != -1);
    std::cout << "NBD_CLEAR_SOCK\n";


    
    err = ioctl(nbd, NBD_SET_SOCK, sp[1]);
    if (err == -1)
    {
        std::cout << "NBD_SET_SOCK terminated with error\n";
    }
    //assert(err != -1); 

    FrameWork *fw = Singleton<FrameWork>::GetInstance();

    fw->RegisterProxy(sp[0], boost::shared_ptr<ProxyBase>(new NbdProxy(dev_file, 4, 14000, sp[0], nbd)));


    fw->RegisterCmd("WriteCmd", NbdProxy::CreateWriteCmd);
    fw->RegisterCmd("ReadCmd", NbdProxy::CreateReadCmd);
    sleep(1);

    system("sudo mkfs.ext4 /dev/nbd3");
   
    std::cout << "mkfs ends\n";
    
    system("mkdir /home/may/Desktop/new_dir");
    system("sudo mount /dev/nbd3 /home/may/Desktop/new_dir");
    system("sudo chmod 777 /home/may/Desktop/new_dir");
    std::cout << "mount\n";

}
}//ilrd

int main()
{

return 0;
}
#include <sys/socket.h> // socketpair
#include <sys/types.h>  // AF_UNIX
#include <sys/ioctl.h> // ioctl
#include <arpa/inet.h> // htonl
#include <linux/nbd.h> // nbd
#include <sys/stat.h> // open
#include <string.h> // memset, memcpy
#include <stdlib.h> // system
#include <fcntl.h> // O_RDWR

#include <boost/unordered_map.hpp> // unordered map
#include <boost/make_shared.hpp> // make_shared
#include <boost/thread/mutex.hpp> // mutex
#include <boost/bind.hpp> //bind

#include "singleton_impl.hpp" // Singleton
#include "mediator.hpp" // Command
#include "logger.hpp" // Logger
#include "write_command.hpp" // Command
#include "read_command.hpp" // Logger

#include "nbd_proxy.hpp" 


namespace ilrd
{

NbdProxy::NbdProxy(const char *path_, size_t block_size_, size_t number_of_blocks_):
m_path(path_),
m_bsize(block_size_),
m_bnum(number_of_blocks_),
m_nbd_fd(SetupNbdFD()),
m_nbd_thread(boost::bind(&NbdProxy::RunThread, this))
{

}

int NbdProxy::SetupNbdFD()
{
    system("sudo modprobe nbd");

    std::string echo("echo 4 | sudo tee /sys/block/nbd");
    echo += m_path;
    echo += "/queue/max_sectors_kb";
    system(echo.c_str());

    std::string chmod_str("sudo chmod 777 /dev/nbd");
    chmod_str += m_path;
    system(chmod_str.c_str());
    
        
    if (-1 == socketpair(AF_UNIX, SOCK_STREAM, 0, m_sockets))
    {
        LOG_ERROR("nbd socketpair failed");
        sleep(1);
        throw NBDException();
    }
    std::string open_str("/dev/nbd");
    open_str += m_path;
    int nbd = open(open_str.c_str(), O_RDWR);
    if (-1 == nbd)
    {
        LOG_ERROR("nbd open failed");
        throw NBDException();
    }

    if (-1 == ioctl(nbd, NBD_SET_BLKSIZE, m_bsize))
    {
        perror("NBD_SET_BLKSIZE failed");
        LOG_ERROR("nbd NBD_SET_BLKSIZE failed");
        throw NBDException();
    }

    if (-1 == ioctl(nbd, NBD_SET_SIZE, m_bsize * m_bnum))
    {
        LOG_ERROR("nbd NBD_SET_SIZE failed");
        throw NBDException();
    }


    if (-1 == ioctl(nbd, NBD_CLEAR_SOCK))
    {
        LOG_ERROR("nbd NBD_CLEAR_SOCK failed");
        throw NBDException();
    }
    Singleton<Logger>::GetInstance()->SetLevel(Logger::DEBUG);

    if (-1 == ioctl(nbd, NBD_SET_SOCK, m_sockets[1]))
    {
        LOG_ERROR("nbd NBD_SET_SOCK failed");
        perror("NBD_SET_SOCK failed");
        throw NBDException();
    }
    /*
    if (-1 == ioctl(nbd, NBD_SET_TIMEOUT, 15))
    {
        perror("NBD_SET_TIMEOUT failed");
        LOG_ERROR("nbd NBD_SET_TIMEOUT failed");
        throw NBDException();
    }
    */
    //LOG_INFO("FINISHED NBD SETUP");
    return nbd;
}
    
NbdProxy::~NbdProxy() noexcept
{
    //LOG_INFO("NBD DTOR");
    if (-1 == ioctl(m_nbd_fd, NBD_DISCONNECT))
    {
        LOG_ERROR("error disconnecting");
    }
    close(m_nbd_fd);
    close(m_sockets[0]);
    close(m_sockets[1]);
    m_nbd_thread.join();
}

void NbdProxy::RunThread()
{
    //LOG_INFO("starting nbd do_it");
    if (-1 == ioctl(m_nbd_fd, NBD_DO_IT))
    {
        LOG_ERROR("error starting nbd do_it");
    }
    //LOG_INFO("ending do_it");
}
 
const char *NbdProxy::GetPath()
{
    return m_path;
}

size_t NbdProxy::GetBlockSize()
{
    return m_bsize;
}

size_t NbdProxy::GetNumOfBlocks()
{
    return m_bnum;
}

void NbdProxy::read_all(int fd, char* buf, size_t count)
{
    while (count > 0) 
    {
        int bytes_read = read(fd, buf, count);
        assert(bytes_read > 0);
        buf += bytes_read;
        count -= bytes_read;
    }
}

void NbdProxy::write_all(int fd, char* buf, size_t count)
{
    while (count > 0) 
    {
        int bytes_written = write(fd, buf, count);
        assert(bytes_written > 0);
        buf += bytes_written;
        count -= bytes_written;
    }
}

boost::shared_ptr<CmdData> NbdProxy::ParseData(fd_ty fd_)
{
    struct nbd_request request;
    CmdArgs args;
    args.reply.magic = htonl(NBD_REPLY_MAGIC);
    args.reply.error = htonl(0);
    std::string type;

    if (0 > read(fd_, &request, sizeof(request)))
    {
        LOG_ERROR("bad read in parse data");
        throw NBDException();
    }

    memcpy(args.reply.handle, request.handle, sizeof(args.reply.handle));
    args.fd = fd_;
    args.key = be64toh(request.from);
    args.reply_func = NbdProxy::write_all;

    if(NBD_CMD_READ == ntohl(request.type))
    {
        LOG_INFO("NBD READ CMD");
        type = "ReadCmd";
    }
    else // NBD_CMD_WRITE:
    {
        LOG_INFO("NBD WRITE CMD");
        read_all(fd_, args.block, ntohl(request.len));
        type = "WriteCmd";
    }

    std::vector<char> data(sizeof(args));
    memcpy(&data[0], &args, sizeof(args));
    return boost::make_shared<CmdData>(CmdData(type, data));
}

void NbdProxy::NbdReply(int fd ,const void *block, size_t size)
{

    write_all(fd, (char *)block, size);
}

int NbdProxy::GetFd()
{
    return m_sockets[0];
}

} // namespace ilrd

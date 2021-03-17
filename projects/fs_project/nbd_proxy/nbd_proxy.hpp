#ifndef __RD94_NBDPROXY_HPP__
#define __RD94_NBDPROXY_HPP__

#include <linux/nbd.h> // nbd_reply
#include <boost/shared_ptr.hpp> // shared_ptr
#include <boost/thread.hpp> //thread


namespace ilrd
{

class NbdProxy: public ProxyBase
{
public:
    
    NbdProxy(const char *path_, size_t block_size_, size_t number_of_blocks_); // may throw bad_resource_error 
    virtual ~NbdProxy() noexcept;

    const char *GetPath();
    size_t GetBlockSize();
    void SetBlockSize(size_t block_size_);
    size_t GetNumOfBlocks();
    void SetNumOfBlocks(size_t number_of_blocks_);
    void NbdReply(int fd ,const void *block, size_t size);
    int GetFd();

    virtual boost::shared_ptr<CmdData> ParseData(fd_ty fd_);

    int SetupNbdFD();
    typedef struct
    {
        int fd;
        u_int64_t key;
        boost::function<void (int, char *, size_t)> reply_func;
        struct nbd_reply reply;
        char block[4096];
    } CmdArgs;

private:

    const char *m_path;
    size_t m_bsize;
    size_t m_bnum;
    int m_nbd_fd;
    int m_sockets[2];
    boost::thread m_nbd_thread;

    void RunThread();
    static void read_all(int fd, char* buf, size_t count);
    static void write_all(int fd, char* buf, size_t count);

    struct NBDException : public std::runtime_error
    {
        NBDException(): std::runtime_error(""){}
        const char *what () const throw()
        {
            return "NBD Fail";
        }
    };

};

} // ilrd

#endif //__RD94_NBDPROXY_HPP__

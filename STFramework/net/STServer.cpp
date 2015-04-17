#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "net/STServer.h"
#include "thread/STThread.h"
#include "net/STFdListeenerBase.h"

const STString ServerListenConnectThreadPrefix  =  "ServerListenConnectThread_";
const STString ServerListenDataThreadPrefix     =  "ServerListenDataThread_";
const int ServerDefaultListenPort               =  9321;
const char FrameHeadTag[]                       =  "\0\0^*#*##*#*^\0\0";


class STServer::Impl
{
public:
    Impl(const STString& name)
        : m_serverName(name)
        , m_listenConnectThread(name, this)
        , m_listenDataThread(name, this)
    {}
    ~Impl(){}


    void setEventReceiver(STObject* receiver)
    {
        if (NULL != receiver && m_safeReceiver.receiver != receiver) {
            m_safeReceiver.receiver = receiver;
            m_safeReceiver.receiverGuard = receiver->getGuard();
        }
    }

    bool startListen(int port)
    {
        //TBD
        STUNUSED(port);
        return false;
    }

    bool sendToClient(const STNetIdentify& target, const STString& dataStr)
    {
        bool ret = false;

        STUNUSED(target);
        STUNUSED(dataStr);

        return ret;
    }

private:
    STNetIdentify getIdViaFd(int fd)
    {
        STNetIdentify ret;
        int port;

        struct sockaddr local_addr;
        socklen_t len = sizeof(sockaddr);
        if (getsockname(fd, &local_addr, &len) == 0) {
            struct sockaddr_in* sin = (struct sockaddr_in*)(&local_addr);
            port = sin->sin_port;

            char addr_buffer[INET_ADDRSTRLEN];
            void * tmp = &(sin->sin_addr);
            if (inet_ntop(AF_INET, tmp, addr_buffer, INET_ADDRSTRLEN) != NULL) {
                ret.setIp(addr_buffer);
                ret.setPort(port);
            }
            else {
                //"inet_ntop err";
            }
        }
        else {
            //"getsockname err";
        }

        return ret;
    }

private://call from the two listen threads
    void onClientConnected(int fd)
    {
        STUNUSED(fd);
    }
    void onClientDisConnected(int fd)
    {
        STUNUSED(fd);
    }
    void onReceivedClientData(const STString& dataStr)
    {
        STUNUSED(dataStr);
    }

private:
    struct SafeReceiver
    {
        STObject*   receiver;
        STGuard     receiverGuard;
        SafeReceiver()
            : receiver(NULL)
            , receiverGuard()
        {}
    };
    class ListenConnectThread : public STThread//listen client's connect action
    {
    public:
        ListenConnectThread(const STString& name, STServer::Impl* owner)
            : STThread(ServerListenConnectThreadPrefix + name)
            , m_owner(owner)
            , m_listenPort(-1)
        {}
        void setListenPort(int port)
        {
            m_listenPort = port;
        }
        virtual void main();
    private:
        STServer::Impl* m_owner;
        int             m_listenPort;
    };

    class ListenDataThread : public STFdListeenerBase//listen data sent from connected client
    {
    public:
        ListenDataThread(const STString& name, STServer::Impl* owner)
            : STFdListeenerBase(ServerListenConnectThreadPrefix + name)
            , m_owner(owner)
        {}

    private:
        virtual void fdChanged(int fd, FdChangeType changeType)
        {
            STUNUSED(fd);
            STUNUSED(changeType);
        }
    private:
        STServer::Impl* m_owner;
        //std::ma
    };

private:
    STString        m_serverName;
    int             m_listenPort;
    SafeReceiver    m_safeReceiver;

    ListenConnectThread m_listenConnectThread;
    ListenDataThread    m_listenDataThread;
};

void STServer::Impl::ListenConnectThread::main()
{
    int                 listenfd;
    int                 connfd;
    struct sockaddr_in  servaddr;

    if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
        //printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
        return;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(m_listenPort);

    if( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
        //printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        return;
    }

    if( listen(listenfd, 10) == -1){
        //printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
        return;
    }

    //"waiting for client's request"
    while (1) {
        if( (connfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) == -1){
            //printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
            continue;
        }
        m_owner->onClientConnected(connfd);
    }

    close(listenfd);
}






STServer::STServer(const STString& name)
    : m_impl(new Impl(name))
{}

STServer::~STServer()
{}

void STServer::setEventReceiver(STObject* receiver)
{
    m_impl->setEventReceiver(receiver);
}

bool STServer::startListen(int port)
{
    return m_impl->startListen(port);
}

bool STServer::sendToClient(const STNetIdentify& target, const STString& dataStr)
{
    return m_impl->sendToClient(target, dataStr);
}






















#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <signal.h>

#include "net/STNetDefine.h"
#include "net/STClient.h"
#include "net/STFdListeenerBase.h"
#include "SocketFdReader.h"

const STString ClientListenThreadPrefix = "ClientListenThread_";

class STClient::Impl : public STFdListeenerBase
{
public:
    Impl(const STString& name)
        : STFdListeenerBase(ClientListenThreadPrefix + name)
    {
        signal(SIGPIPE, SIG_IGN);
    }

    void setEventReceiver(STObject* receiver)
    {
        if (NULL != receiver && m_safeReceiver.receiver != receiver) {
            m_safeReceiver.receiver = receiver;
            m_safeReceiver.receiverGuard = receiver->getGuard();
        }
    }

    void disConnect()
    {
        if ( m_serverId.isValid() ) {
            this->stopListen();
            close(m_serverId.fd());
            m_serverId.clear();
        }
    }

    bool isConnected()
    {
        return m_serverId.isValid();
    }

    bool sendToServer(const STString& dataStr)
    {
        int dataSize = dataStr.size();
        int ret = write(m_serverId.fd(), FrameHeadTag, sizeof(FrameHeadTag));
        if (-1 != ret) {
            ret = write(m_serverId.fd(), &dataSize, sizeof(dataSize));
        }
        if (-1 != ret) {
            write(m_serverId.fd(), dataStr.c_str(), dataSize);
        }
        return -1 != ret;
    }

    bool connectToServer(const STString& ip, int port)
    {
        m_serverId.setIp(ip);
        m_serverId.setPort(port);

        struct sockaddr_in server_addr, client_addr;
        socklen_t socklen = sizeof(server_addr);

        int clientFd = -1;
        if((clientFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
                //printf("create socket error, exit!\n");
                return false;
        }
        srand(time(NULL));
        bzero(&client_addr, sizeof(client_addr));
        client_addr.sin_family = AF_INET;
        client_addr.sin_addr.s_addr = htons(INADDR_ANY);

        bzero(&server_addr, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        inet_aton(ip.c_str(), &server_addr.sin_addr);
        server_addr.sin_port = htons(port);

        if(connect(clientFd, (struct sockaddr*)&server_addr, socklen) < 0)
        {
                //printf("can not connect to %s, exit!\n", argv[1]);
                //printf("%s\n", strerror(errno));
                return false;
        }

        m_serverId.setFd(clientFd);

        this->addFd(m_serverId.fd());

        return true;
    }

protected:
    virtual void fdChanged(int fd, FdChangeType changeType)
    {
        if (STFdListeenerBase::FdChangeType_CanRead == changeType) {
            int readCount = m_fdReader.readData(fd);
            if (0 == readCount) {
                //client disconnected
                m_serverId.clear();
                this->stopListen();
            }
            while (m_fdReader.preparedFrameCount() != 0) {
                SocketFdReader::FrameInfo dataInfo = m_fdReader.getOneFrameData();
                //std::cout<<"STClient::ListenDataThread::fdChanged(),oneFrame, dataStr="<<dataInfo.dataStr<<std::endl;
                onReceivedClientData(dataInfo.fd, dataInfo.dataStr);
            }
        }
        else {
            //err!
        }
    }

private:
    void onReceivedClientData(int fd, const STString& dataStr)//call from listenDataThread
    {
        STEventCarrier e(new STNetEvent(getIdViaFd(fd), dataStr));
        if (NULL != m_safeReceiver.receiver && !m_safeReceiver.receiverGuard.isDeleted()) {
            STObject::postGlobalEvent(e, m_safeReceiver.receiver);
        }
        else {
            STObject::postGlobalEvent(e, NULL);
        }
    }

    STNetIdentify getIdViaFd(int fd)
    {
        STNetIdentify ret;
        ret.setFd(fd);

        int port;

        struct sockaddr local_addr;
        socklen_t len = sizeof(sockaddr);
        if (getpeername(fd, &local_addr, &len) == 0) {
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

private:
    STNetIdentify   m_serverId;
    SocketFdReader  m_fdReader;
    SafeReceiver    m_safeReceiver;
    int             m_clientFd;
};


STClient::STClient(const STString& clientName)
    : m_impl( new Impl(clientName) )
{
}

STClient::~STClient()
{
}

void STClient::setEventReceiver(STObject* receiver)
{
    m_impl->setEventReceiver(receiver);
}

bool STClient::connectToServer(const STString& ip, int port)
{
    return m_impl->connectToServer(ip, port);
}

void STClient::disConnect()
{
    m_impl->disConnect();
}

bool STClient::sendToServer(const STString& dataStr)
{
    return m_impl->sendToServer(dataStr);
}
























#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>

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
    {}

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
            m_reader.readData(fd);
            while (0 != m_reader.preparedFrameCount()) {
                SocketFdReader::FrameInfo info = m_reader.getOneFrameData();
                STEventCarrier e(new STNetEvent(m_serverId, info.dataStr));
                STObject::postGlobalEvent(e, m_safeReceiver.receiverGuard.isDeleted() ? NULL : m_safeReceiver.receiver);
            }
        }
        else if (STFdListeenerBase::FdChangeType_UnAvailable == changeType) {
            m_serverId.clear();
            this->stopListen();
        }
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
    SocketFdReader  m_reader;
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
























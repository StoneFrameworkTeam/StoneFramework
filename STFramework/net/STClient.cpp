#include "net/STClient.h"
#include "thread/STThread.h"

const STString ClientListenThreadPrefix = "ClientListenThread_";

class STClient::Impl : public STThread
{
public:
    Impl(const STString& name)
        : STThread(ClientListenThreadPrefix + name)
    {}

    void main()
    {
        //main listeen func
    }

private:
    STNetIdentify   m_serverId;
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
    STUNUSED(receiver);
}

bool STClient::connectToServer(const STString& ip, int port)
{
    bool ret = false;

    STUNUSED(ip);
    STUNUSED(port);

    return ret;
}

void STClient::disConnect()
{

}

bool STClient::sendToServer(const STString& dataStr)
{
    bool ret = false;

    STUNUSED(dataStr);

    return ret;
}
























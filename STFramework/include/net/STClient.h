#ifndef ST_CLIENT_H
#define ST_CLIENT_H

#include "core/STObject.h"
#include "base/STPtr.h"
#include "net/STNetEvent.h"


//the main sequence: connectToServer(...) -> sendToServer() and receive data from server
class STClient : public STObject
{
public:
    STClient(const STString& clientName);
    ~STClient();

    void setEventReceiver(STObject* receiver);//event will send as STNetEvent

    bool connectToServer(const STString& ip, int port);//will run another thread to listen
    void disConnect();
    bool sendToServer(const STString& dataStr);
private:
    class Impl;
    STScopePtr<Impl> m_impl;
};



#endif // ST_CLIENT_H

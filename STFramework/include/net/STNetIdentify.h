#ifndef ST_CONNECT_IDENTIFY_H
#define ST_CONNECT_IDENTIFY_H

#include "base/STCommonDefine.h"
#include "base/STPtr.h"


//the key is Ip and port
class STNetIdentify
{
public:
    STNetIdentify();
    ~STNetIdentify();

    bool operator ==(const STNetIdentify&other);

    STString ip() const;
    void setIp(const STString& value);
    int port() const;
    void setPort(int value);

    bool isValid();
    void clear();

    STString toSerialString();
    bool fromSerialString(const STString& serialString);

    int fd() const;
    void setFd(int fd);
private:
    STString    m_ip;
    int         m_port;
    int         m_fd;
};


#endif // ST_CONNECT_IDENTIFY_H

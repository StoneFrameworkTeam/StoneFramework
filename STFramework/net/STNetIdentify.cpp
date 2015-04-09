#include "net/STNetIdentify.h"
#include "tools/STDataItem.h"

STNetIdentify::STNetIdentify()
    : m_ip("")
    , m_port(-1)
{
}

STNetIdentify::~STNetIdentify()
{
}

bool STNetIdentify::operator ==(const STNetIdentify& other)
{
    return (m_ip == other.m_ip) && (m_port == other.m_port);
}

STString STNetIdentify::ip() const
{
    return m_ip;
}

void STNetIdentify::setIp(const STString& value)
{
    m_ip = value;
}

int STNetIdentify::port() const
{
    return m_port;
}
void STNetIdentify::setPort(int value)
{
    m_port = value;
}

bool STNetIdentify::isValid()
{
    return (m_ip != "") && (m_port > 0);
}

STString STNetIdentify::toSerialString()
{
    STString ret = "";

    if ( isValid() ) {
        STDataItem item("STNetIdentify");
        item.addChild(STDataItem("ip", m_ip));
        item.addChild(STDataItem("port", m_port));
        ret = item.serialToStr();
    }

    return ret;
}

bool STNetIdentify::fromSerialString(const STString& serialString)
{
    bool ret = true;

    STDataItem item("tmp");
    if (item.loadFromStr(serialString)) {
        bool transRet = true;
        STString ip = item.child("ip").toString(transRet);
        ret &= transRet;
        int port = item.child("port").toInt(transRet);
        ret &= transRet;

        if (ret) {
            m_ip = ip;
            m_port = port;
        }
    }
    else {
        ret = false;
    }

    return ret;
}



















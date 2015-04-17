#ifndef ST_FD_LISTEENER_BASE_H
#define ST_FD_LISTEENER_BASE_H

#include <vector>

#include "base/STPtr.h"

class STFdListeenerBase
{
public:
    enum FdChangeType
    {
        FdChangeType_CanRead,
        FdChangeType_CanWrite,
        FdChangeType_UnAvailable,

        FdChangeType_Count
    };

    typedef std::vector<int> FdArr;

public:
    STFdListeenerBase(const STString& name = "");
    virtual ~STFdListeenerBase();

    void addFd(int fd);
    void removeFd(int fd);
    bool startToListen();//not block, this action will start in a new thread.
    void stopListen();

protected:
    virtual void fdChanged(int fd, FdChangeType changeType) = 0;//will be called in listtenThread

private:
    class Impl;
    STScopePtr<Impl> m_impl;
};

#endif // ST_FD_LISTEENER_BASE_H

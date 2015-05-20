#ifndef ST_THREAD_H
#define ST_THREAD_H

#include "base/STCommonDefine.h"
#include "base/STPtr.h"

class STThread
{
public:
enum ExecRet
{
    ExecRet_Success = 0,
    ExecRet_AlreadyRunning,
    ExecRet_Err,

    ExecRet_Count
};

public:
    STThread(const STString& name = "");
    virtual ~STThread();

    STString name();
    virtual void main() = 0;//thread main function

    ExecRet exec();
    void join();

    bool isRunning() const;
    void forceThreadStop();//force stop thread, after that, dealAfterKill() would be called

    virtual void dealAfterKill() //if exit not normal, this function will be run
    {
        //you may need free you resource here when thread exit not normal
    }

private:
    STThread(const STThread&);
    STThread& operator=(const STThread&);
    bool operator==(const STThread&);

private:
    static void* runThread(void *thread);

private:
    class PrivateData;
    STScopePtr<PrivateData> m_data;
};

#endif // ST_THREAD_H

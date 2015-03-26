#ifndef ST_THREAD_H
#define ST_THREAD_H

#include "base/STCommonDefine.h"
#include "base/STPtr.h"

class STThread
{
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

    ExecRet exec();
    void join();

    bool isRunning() const;
    void askToStop();//ask thread to exit
    void askAndWaitToStop(int s=2);//ask and wait thread to exit

    bool isNeedStop() const;
    virtual void main() = 0;//thread main function
    virtual void dealAfterKill() {}//if exit not normal, this function will be run

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

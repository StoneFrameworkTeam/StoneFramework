#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#include <errno.h>
#include <signal.h>

#include "base/STWaiter.h"



//STWaiter
class STWaiter::PrivateData
{
public:
    sem_t semId;

    PrivateData()
    {
        if ( sem_init (&semId, 0, 0) == -1 ) {
            //"sem_init is err"
        }
    }
    ~PrivateData()
    {
        sem_destroy(&semId);
    }
};

STWaiter::STWaiter()
    : m_data(new PrivateData())
{
}

STWaiter::~STWaiter()
{
}

void STWaiter::wait()
{
    sem_wait(&m_data->semId);
}

void STWaiter::reset()
{
    if ( sem_init (&m_data->semId, 0, 0) == -1 ) {
        //"sem_init is err"
    }
}

STWaitResult STWaiter::wait(int s)
{
    STWaitResult ret = STWaitResult_Happen;

    timespec waitTime;
    waitTime.tv_sec = time(NULL) + s;
    waitTime.tv_nsec = 0;
    int status = -1;
    while ((status = sem_timedwait(&m_data->semId, &waitTime)) == -1 && errno == EINTR) {
        continue;       /* Restart if interrupted by handler */
    }
    if (-1 == status) {
        if (errno == ETIMEDOUT) {
            ret = STWaitResult_Timeout;
        }
        else {
            ret = STWaitResult_Err;
        }
    }
    else {
        ret = STWaitResult_Happen;
    }

    return ret;
}

void STWaiter::setHappen()
{
    sem_post(&m_data->semId);
}















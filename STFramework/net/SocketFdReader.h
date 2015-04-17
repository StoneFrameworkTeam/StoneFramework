#ifndef SOCKET_FD_READER_H
#define SOCKET_FD_READER_H


#include <map>
#include "base/STCommonDefine.h"


class SocketFdReader
{
public:
    SocketFdReader();

    void readData(int fd);
    int preparedFrameCount();
    STString getOneFrameData();

private:
    struct FdData
    {
        char* headBuf;
        char* bodyBuf;

        FdData()
            : headBuf(NULL)
            , bodyBuf(NULL)
        {}
        ~FdData()
        {
            if (NULL != headBuf) {
                delete[] headBuf;
            }
            if (NULL != bodyBuf) {
                delete[] bodyBuf;
            }
        }
    };

    std::map<int, FdData> m_fData;
};

#endif // SOCKET_FD_READER_H

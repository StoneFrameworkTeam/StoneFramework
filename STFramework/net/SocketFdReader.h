#ifndef SOCKET_FD_READER_H
#define SOCKET_FD_READER_H


#include <map>
#include <vector>
#include "base/STCommonDefine.h"


class SocketFdReader
{
public:
    struct FrameInfo
    {
        int fd;
        STString dataStr;
        FrameInfo(int fdNum, const STString& frameDataStr)
            : fd(fdNum)
            , dataStr(frameDataStr)
        {}
    };

public:
    SocketFdReader();
    ~SocketFdReader();

    int readData(int fd);
    void clearData(int fd);

    int preparedFrameCount();
    FrameInfo getOneFrameData();

private:
    struct FdData
    {
        int     headSize;
        char*   headBuf;

        int     bodySize;
        char*   bodyBuf;

        int     position;

        void releaseBodyBufAndClearHeadData()
        {
            if (NULL != bodyBuf) {
                delete[] bodyBuf;
                bodyBuf = NULL;
            }
            bodySize = 0;
            position = 0;
        }

        void releaseAllBuf()
        {
            if (NULL != headBuf) {
                delete[] headBuf;
                headBuf = NULL;
            }
            if (NULL != bodyBuf) {
                delete[] bodyBuf;
                bodyBuf = NULL;
            }
            headSize = 0;
            bodySize = 0;
            position = 0;
        }
        FdData()
            : headSize(0)
            , headBuf(NULL)
            , bodySize(0)
            , bodyBuf(NULL)
            , position(0)
        {}
    };

    std::map<int, FdData>   m_fdData;
    std::vector<FrameInfo>  m_frames;
};

#endif // SOCKET_FD_READER_H

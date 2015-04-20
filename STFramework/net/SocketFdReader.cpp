#include "base/STCommonDefine.h"
#include "SocketFdReader.h"
#include "net/STNetDefine.h"

SocketFdReader::SocketFdReader()
{
}

SocketFdReader::~SocketFdReader()
{
    std::map<int, FdData>::iterator itor = m_fdData.begin();
    for (; itor != m_fdData.end(); ++itor) {
        itor->second.releaseAllBuf();
    }
}

void SocketFdReader::readData(int fd)
{
    FdData data = m_fdData[fd];
    if (0 == data.headSize) {
        data.headSize = sizeof(FrameHeadTag) + sizeof(int);
        data.headBuf = new char[data.headSize];
        STDAssert(NULL != data.headBuf);
        data.position = 0;
    }

    while (1) {
        int readCount = 0;
        if (data.position < data.bodySize) {
            readCount = read(fd, data.headBuf+data.position, data.headSize-data.position);
            data.position += readCount;
        }
        else {
            if (0 >= data.bodySize) {
                data.bodySize = *( (int*)(data.headBuf + data.headSize-sizeof(int)) );
                data.bodyBuf = new char[data.bodySize];
                STDAssert(NULL != data.bodyBuf);
            }
            readCount = read(fd, data.bodyBuf + (data.position-data.headSize), data.bodySize - (data.position-data.headSize));
            data.position += readCount;
        }

        if (data.position == data.headSize + data.bodySize) {
            m_frames.push_back( FrameInfo(fd, std::string(data.bodyBuf, data.bodySize)) );
            data.releaseAllBuf();
        }

        if (readCount <= 0) {
            break;
        }
    }
}

void SocketFdReader::clearData(int fd)
{
    std::map<int, FdData>::iterator itor = m_fdData.find(fd);
    if (itor != m_fdData.end()) {
        itor->second.releaseAllBuf();
        m_fdData.erase(itor);
    }
}

int SocketFdReader::preparedFrameCount()
{
    return m_frames.size();
}

SocketFdReader::FrameInfo SocketFdReader::getOneFrameData()
{
    FrameInfo ret = m_frames.back();
    m_frames.pop_back();
    return ret;
}

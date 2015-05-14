#include <iostream>

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

int SocketFdReader::readData(int fd)
{
    int ret = 0;
    FdData& data = m_fdData[fd];
    if (0 == data.headSize) {
        data.headSize = sizeof(FrameHeadTag) + sizeof(int);
        data.headBuf = new char[data.headSize];
        STDAssert(NULL != data.headBuf);
        data.position = 0;
    }

    while (1) {
        int readCount = 0;
        if (data.position < (data.headSize-sizeof(int))) {//read head tag
            readCount = read(fd, data.headBuf+data.position, 1);
            if (1==readCount && data.headBuf[data.position] == FrameHeadTag[data.position]) {
                data.position += readCount;
            }
            else if (1==readCount && data.headBuf[data.position] != FrameHeadTag[data.position]) {
                data.position = 0;
                //unrecognize data!!! bug!!!
            }
        }
        else if(data.position < data.headSize) {//read body size
            readCount = read(fd, data.headBuf+data.position, data.headSize-data.position);
            if (readCount > 0) {
                data.position += readCount;
                if (data.headSize == data.position) {
                    data.bodySize = *( (int*)(data.headBuf + data.headSize-sizeof(int)) );
                    data.bodyBuf = new char[data.bodySize];
                    STDAssert(NULL != data.bodyBuf);
                }
            }
        }
        else {//read body
            readCount = read(fd, data.bodyBuf + (data.position-data.headSize), data.bodySize - (data.position-data.headSize));
            if (readCount > 0) {
                data.position += readCount;
            }
        }

        std::cout<<"SocketFdReader::readData(), data.position:"<<data.position<<"frameSize:"<<data.headSize + data.bodySize<<std::endl;
        if (data.position == data.headSize + data.bodySize) {
            m_frames.push_back( FrameInfo(fd, std::string(data.bodyBuf, data.bodySize)) );
            data.releaseBodyBufAndClearHeadData();
        }

        if (readCount <= 0) {
            break;
        }
        else {
            ret += readCount;
        }
    }

    return ret;
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

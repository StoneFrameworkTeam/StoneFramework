#include "base/STCommonDefine.h"
#include "SocketFdReader.h"

SocketFdReader::SocketFdReader()
{
}

void SocketFdReader::readData(int fd)
{
    STUNUSED(fd);
}

int SocketFdReader::preparedFrameCount()
{
    int ret = 0;
    return ret;
}

STString SocketFdReader::getOneFrameData()
{
    STString ret;

    return ret;
}

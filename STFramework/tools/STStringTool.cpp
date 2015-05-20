#include <sstream>

#include "tools/STStringTool.h"


int STStringTool::strToInt(const STString& src)
{
    bool ret;
    return strToInt(src, ret);
}

int STStringTool::strToInt(const STString& src, bool& transRet)
{
    std::stringstream stream;
    int ret = -1;

    stream << src;
    stream >> ret;

    transRet = !stream.fail() && stream.eof();

    return ret;
}

STString STStringTool::intToStr(int src)
{
    bool ret;
    return intToStr(src, ret);
}

STString STStringTool::intToStr(int src, bool& transRet)
{
    std::stringstream stream;
    STString ret("");

    stream << src;
    stream >> ret;

    transRet = !stream.fail() && stream.eof();

    return ret;
}


double STStringTool::strToDouble(const STString& src)
{
    bool ret;
    return strToDouble(src, ret);
}

double STStringTool::strToDouble(const STString& src, bool& transRet)
{
    std::stringstream stream;
    double ret = 0.0;

    stream << src;
    stream >> ret;

    transRet = !stream.fail() && stream.eof();

    return ret;
}

STString STStringTool::doubleToStr(double src)
{
    bool tmp;
    return doubleToStr(src, tmp);
}

STString STStringTool::doubleToStr(double src, bool& transRet)
{
    std::stringstream stream;
    STString ret("");

    stream << src;
    stream >> ret;

    transRet = !stream.fail() && stream.eof();

    return ret;
}
















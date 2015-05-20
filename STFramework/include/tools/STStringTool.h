#ifndef ST_STRING_TOOL_H
#define ST_STRING_TOOL_H

#include "base/STCommonDefine.h"

class STStringTool
{
public:
    static int strToInt(const STString& src);
    static int strToInt(const STString& src, bool& transRet);
    static STString intToStr(int src);
    static STString intToStr(int src, bool& transRet);

    static double strToDouble(const STString& src);
    static double strToDouble(const STString& src, bool& transRet);
    static STString doubleToStr(double src);
    static STString doubleToStr(double src, bool& transRet);//max decimals:5
};

#endif // ST_STRING_TOOL_H

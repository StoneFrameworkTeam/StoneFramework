#ifndef ST_NET_DEFINE_H
#define ST_NET_DEFINE_H

#include "base/STCommonDefine.h"

static const int ServerDefaultListenPort = 9321;

//one frame is frameHeadTag+int(sizeOfBody)+body
static const char FrameHeadTag[] = "\0\0^*#*&*#*^\0\0";


#endif // ST_NET_DEFINE_H

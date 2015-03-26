#ifndef ST_COMMON_DEFINE_H
#define ST_COMMON_DEFINE_H

#include <string>

enum STExitReason
{
    STExitReason_Normal = 0,
    STExitReason_Err,

    STExitReason_Count
};

#define STUNUSED(a) ((void)a)

typedef std::string STString;
//#define STVector std::vector;
//#define STList   std::list;


inline void STDAssert(bool a)
{
    if(!(a))
    {
        ((void(*)())(0x4444))();
    }
}



#endif // ST_COMMON_DEFINE_H

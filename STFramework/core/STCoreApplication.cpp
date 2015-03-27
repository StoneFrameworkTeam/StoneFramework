#include "core/STCoreApplication.h"


class STCoreApplication::CoreApplicationPrivateData
{
public:
};

STCoreApplication::STCoreApplication(int &argc, char **argv)
    : STObject()
    , m_data(new CoreApplicationPrivateData())
{
    STUNUSED(argc);
    STUNUSED(argv);

    registerGlobalEventDispatcher(this);
}

STCoreApplication::~STCoreApplication()
{
    unRegisterGlobalEventDispatcher(this);
}

STString STCoreApplication::applicationName() const
{
    STString ret = "";


    return ret;
}

void STCoreApplication::exec()
{
    this->startEventLoop();
}

void STCoreApplication::exit()
{
    this->exitEventLoop();
}









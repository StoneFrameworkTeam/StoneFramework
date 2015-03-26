#ifndef ST_CORE_APPLICATION_H
#define ST_CORE_APPLICATION_H

#include "base/STCommonDefine.h"
#include "core/STObject.h"
#include "event/STEvent.h"

class STCoreApplication : public STObject, EventDispatcher
{

public:
    STCoreApplication(int &argc, char **argv);
    ~STCoreApplication();

    STString applicationName() const;

    STExitReason exec();
    void exit(STExitReason reason=STExitReason_Normal);

    virtual void postEvent(STEventCarrier e, STObject* receiver);
    virtual bool sendEvent(STEventCarrier e, STObject* receiver);
    virtual void addEventFilter(STObject* filter, STEvent e);
    virtual void removeEventFilter(STObject* filter, STEvent e);

private:
    class PrivateData;
    STScopePtr<PrivateData> m_data;
};

#endif // ST_CORE_APPLICATION_H

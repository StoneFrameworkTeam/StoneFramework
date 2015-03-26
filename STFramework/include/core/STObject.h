#ifndef ST_OBJECT_H
#define ST_OBJECT_H

#include "event/STEvent.h"
#include "base/STGuard.h"


class EventDispatcher;
class STObject
{
public:
    STObject();
    virtual ~STObject();

    virtual void eventHappen(STEventCarrier e);
    void setReceiveEvent(STEvent e);
    void setUnReceiveEvent(STEvent e);

    STGuard getGuard() const;

private:
    STGuard m_guard;

 //static, for event
public:
    static void postEvent(STEventCarrier e, STObject* receiver = NULL);
    static bool sendEvent(STEventCarrier e, STObject* receiver = NULL);
protected:
    static void registerGlobalEventDispatcher(EventDispatcher* disPatcher);
    static void unRegisterGlobalEventDispatcher(EventDispatcher* disPatcher);
private:
    static EventDispatcher* s_eventDispatcher;
};



class EventDispatcher
{
public:
    virtual ~EventDispatcher(){}
    virtual void postEvent(STEventCarrier e, STObject* receiver = NULL) = 0;
    virtual bool sendEvent(STEventCarrier e, STObject* receiver = NULL) = 0;

    virtual void addEventFilter(STObject* filter, STEvent e) = 0;
    virtual void removeEventFilter(STObject* filter, STEvent e) = 0;
};






#endif // ST_OBJECT_H






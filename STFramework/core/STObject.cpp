#include "core/STObject.h"


STObject::STObject()
{
}

STObject::~STObject()
{

}

void STObject::eventHappen(STEventCarrier e)
{
    STUNUSED(e);
}

void STObject::setReceiveEvent(STEvent e)
{
    if ( NULL != s_eventDispatcher) {
        s_eventDispatcher->addEventFilter(this, e);
    }
}

void STObject::setUnReceiveEvent(STEvent e)
{
    if ( NULL != s_eventDispatcher) {
        s_eventDispatcher->removeEventFilter(this, e);
    }
}

STGuard STObject::getGuard() const
{
    return m_guard.getListeenGuard();
}



//static, for event
EventDispatcher* STObject::s_eventDispatcher = NULL;

void STObject::registerGlobalEventDispatcher(EventDispatcher* disPatcher)
{
    if (NULL != disPatcher) {
        s_eventDispatcher = disPatcher;
    }
}

void STObject::unRegisterGlobalEventDispatcher(EventDispatcher* disPatcher)
{
    if (disPatcher == s_eventDispatcher) {
        s_eventDispatcher = NULL;
    }
}

void STObject::postEvent(STEventCarrier e, STObject* receiver)
{
    if (NULL != s_eventDispatcher) {
        s_eventDispatcher->postEvent(e, receiver);
    }
}

bool STObject::sendEvent(STEventCarrier e, STObject* receiver)
{
    bool ret = false;
    if (NULL != s_eventDispatcher) {
        ret = s_eventDispatcher->sendEvent(e, receiver);
    }
    return ret;
}



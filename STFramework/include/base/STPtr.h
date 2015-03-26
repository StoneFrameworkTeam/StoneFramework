#ifndef ST_PTR_H
#define ST_PTR_H

#include "base/STAtom.h"
#include "base/STCommonDefine.h"

#ifndef NULL
    #define NULL 0
#endif

template <class T>
class STScopePtr
{
public:
    explicit STScopePtr(T* obj)
        : m_obj(obj)
    {}
    ~STScopePtr()
    {
        deleteObj();
    }

    void deleteObj()
    {
        if (NULL != m_obj) {
            delete m_obj;
            m_obj = NULL;
        }
    }

    T* get() const
    {
        return m_obj;
    }

    T* operator->() const
    {
        return m_obj;
    }

    operator bool()
    {
        return (m_obj != NULL);
    }
private:
    STScopePtr();
    STScopePtr(const STScopePtr& other);
    STScopePtr& operator=(const STScopePtr& other);
    bool operator==(const STScopePtr& other);

private:
    T* m_obj;
};

template <class T>
class STSharePtr
{
public:
    explicit STSharePtr(T* obj)
        : m_objRef(new ObjRef(obj))
    {}
    STSharePtr(const STSharePtr& other)
    {
        if (this != &other) {
            this->m_objRef = other.m_objRef;
            addRef();
        }
    }

    ~STSharePtr()
    {
        removeRef();
    }

    T* get() const
    {
        return m_objRef->obj;
    }

    T* operator->() const
    {
        return m_objRef->obj;
    }

    STSharePtr& operator=(const STSharePtr& other)
    {
        if (this != &other) {
            this->m_objRef = other.m_objRef;
            addRef();
        }
        return *this;
    }
    STSharePtr& operator=(T* obj)
    {
        if (this->m_objRef->obj != obj) {
            removeRef();

            this->m_objRef = new ObjRef(obj);
        }
        return *this;
    }
    bool operator==(const STSharePtr& other)
    {
        return (m_objRef->obj = other.m_objRef->obj);
    }
    operator bool()
    {
        return (NULL != m_objRef->obj);
    }

private:
    struct ObjRef
    {
        int count;
        T*  obj;
        ObjRef(T* obj)
            : count(1)
            , obj(obj)
        {}
    };

private:
    void addRef()
    {
        STAtom::Inc(&(m_objRef->count));
    }
    void removeRef()
    {
        STDAssert(m_objRef->count > 0);
        STAtom::Dec(&(m_objRef->count));
        if (0 == m_objRef->count && m_objRef->obj != NULL) {
            delete m_objRef->obj;
            m_objRef->obj = NULL;
        }
    }

private:
    ObjRef *m_objRef;
};




#endif // ST_PTR_H

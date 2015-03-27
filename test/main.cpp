#include "core/STCoreApplication.h"
#include <iostream>
#include "base/STWaiter.h"
#include "thread/STThread.h"
#include "base/STPtr.h"
#include <list>
#include "tools/STDataItem.h"

void fdwaiterTest()
{
    STFDWaiter waiter("/dev/test");


    char buf[100] = {0};
    while (1) {
        STWaitResult waitRet = waiter.wait(-1);
        if (STWaitResult_Happen == waitRet) {
            int readRet = read(waiter.fd(), buf, sizeof(buf));
            if (readRet > 0) {
                std::cout<<("waiter happen, read:")<< buf <<std::endl;
            }
            else {
                std::cout<<("waitError\n");
            }
        }
    }
}

class testThread : public STThread
{
public:
    STWaiter* m_testWaiter;
    void main()
    {
        sleep(7);
        m_testWaiter->setHappen();
    }
};

void waiterTest()
{
    STWaiter waiter;


    testThread thread;
    thread.m_testWaiter = &waiter;
    thread.exec();


    int t = time(NULL);
    STWaitResult ret = waiter.wait(10);
    t = time(NULL) -t;

    thread.join();
    //waiter.wait();

    std::cout<<"waiterTest, ret:"<< ret <<", t="<<t<<std::endl;
}

class TestSharePtrClass
{
public:
    TestSharePtrClass()
        : m_name("TestSharePtrClass")
    {
        std::cout<<("TestSharePtrClass() create");
    }
    ~TestSharePtrClass()
    {
        std::cout<<("~TestSharePtrClass() destroy");
    }

    STString m_name;
};

void testSharePtr()
{
    STSharePtr<TestSharePtrClass> ptr(new TestSharePtrClass);

    {
        STSharePtr<TestSharePtrClass> ptr2 = ptr;
        STSharePtr<TestSharePtrClass> ptr3 = ptr2;
    }
    STSharePtr<TestSharePtrClass> ptr4 = ptr;
    STSharePtr<TestSharePtrClass> ptr5 = ptr;

    std::cout<<("testSharePtr(), m_name:")<<( ptr->m_name.c_str())<<std::endl;
}



class TestCoreApplicationThread : public STThread
{
public:
    void main()
    {
        while (1) {
            STEventCarrier e1(new STEvent("eventType1"));
            STObject::postGlobalEvent(e1);

            STEventCarrier e2(new STEvent("eventType2"));
            STObject::postGlobalEvent(e2);

            STEventCarrier e3(new STEvent("eventType3"));
            STObject::postGlobalEvent(e3);


            STObject::postGlobalEvent(STEventCarrier(new STEvent("eventType1-1")));
            STObject::postGlobalEvent(STEventCarrier(new STEvent("forDel1")));

            STEventCarrier e5(new STEvent("eventType5"));
            STObject::postGlobalEvent(e5);

            usleep(1000*100);
        }

    }
};


class TestEventReceiver : public STObject
{
public:
    TestEventReceiver(const STString& name)
        : m_name(name)
    {}

    virtual void eventHappen(STEventCarrier e)
    {
        std::cout<< m_name << "->eventHappen, e.typeName()=" << e->typeName() << std::endl;
    }
private:
    STString m_name;
};

void testCoreApplication(int argc, char *argv[])
{
    STCoreApplication app(argc, argv);

    TestEventReceiver receiver1("receiver1");
    receiver1.setReceiveEvent(STEvent("eventType1"));
    receiver1.setReceiveEvent(STEvent("eventType1-1"));
    receiver1.setUnReceiveEvent(STEvent("eventType1-1"));

    TestEventReceiver receiver2("receiver2");
    receiver2.setReceiveEvent(STEvent("eventType2"));

    TestEventReceiver *forDel = new TestEventReceiver("forDel");
    forDel->setReceiveEvent(STEvent("forDel1"));
    delete forDel;

    TestEventReceiver receiver3("receiver3");
    receiver3.setReceiveEvent(STEvent("eventType3"));


    TestCoreApplicationThread thread;
    TestCoreApplicationThread thread2;
    TestCoreApplicationThread thread3;
    thread.exec();
    thread2.exec();
    thread3.exec();

    app.exec();
}

class TestGuardObj : public STObject
{

};

void testGuard()
{
    TestGuardObj* obj = new TestGuardObj();
    STGuard guard = obj->getGuard();
    std::cout<<"guard.isDeleted()="<< (guard.isDeleted() ? "true" : "false") <<std::endl;

    delete obj;
    std::cout<<"guard.isDeleted()="<< (guard.isDeleted() ? "true" : "false") <<std::endl;
}


void testListErase()
{
    int maxIdx = 10;
    int deleteIdx = 6;
    std::list<int> testList;
    for (int i=0; i<maxIdx; ++i) {
        testList.push_back(i);
    }

    std::cout<<"before erase:"<<std::endl;
    std::list<int>::iterator itor = testList.begin();
    for (; itor != testList.end(); ++itor) {
        std::cout<<"  "<<(*itor);
    }
    std::cout<<std::endl;

    itor = testList.begin();
    for (; itor != testList.end(); ++itor) {
        if ( deleteIdx == *itor) {
            testList.erase(itor--);
            //break;
        }
    }

    std::cout<<"after erase:"<<std::endl;
    itor = testList.begin();
    for (; itor != testList.end(); ++itor) {
        std::cout<<"  "<<(*itor);
    }
    std::cout<<std::endl;

}


/*
<root>
    <parent1>
        <child1><int>12</ing></child1>
        <child2><bool>true</bool></child2>
    </parent1>
    <child3><string>this is child3</string></child3>
</root>

*/

void testDataItem()
{
    STString xmlPath1("/home/wangl/myCode/STFramework/output/dataItem");
    STString xmlPath2("/home/wangl/myCode/STFramework/output/dataItem2");
    STDataItem root("root");
    STDataItem parent1("parent1");
    parent1.addChild(STDataItem("child1", 12));
    parent1.addChild(STDataItem("child22", true));

    root.addChild(parent1);
    root.addChild(STDataItem("child3", "this is child3"));

    root.saveToFile(xmlPath1);

    STDataItem root2("");
    root2.loadFromFile(xmlPath1);
    root2.saveToFile(xmlPath2);
}

int main(int argc, char *argv[])
{
    STUNUSED(argc);
    STUNUSED(argv);
    //fdwaiterTest();
    //waiterTest();
    //testSharePtr();
    //testGuard();
    //testListErase();
    //testCoreApplication(argc, argv);
    testDataItem();
    return 0;
}












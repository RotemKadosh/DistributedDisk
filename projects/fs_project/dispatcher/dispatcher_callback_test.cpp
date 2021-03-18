/*#include "dispatcher_callback.hpp"
#include "dispatcher_callback_impl.hpp"
#include <boost/shared_ptr.hpp>
#include "utils.hpp"

using namespace ilrd;
using namespace std;




class ControllerEvent
{
public:
    static int counter;
private:
};
int ControllerEvent::counter = 0;

class ViewWindow
{
public:
    ViewWindow():id(counter_id++){}
    void Notify(ControllerEvent ev)
    {
        ++ev.counter;
    }
    void Death()
    {
        std::cout<< "dispatcher died - observer:"<< id << "\n";
        --counter_id;
    }

private:
    int id;
    static int counter_id;
};

int ViewWindow::counter_id = 0; 


test_status_t OneObserverTest();

test_status_t TwoObserverTest();

test_status_t ThreeObserverTest();

test_status_t RemoveOneObserverTest();

test_status_t RemoveThreeObserverTest();

test_status_t AddOneObserverTest();

test_status_t AddTwoObserverTest();

test_status_t OneObserverTest()
{
    Dispatcher<ControllerEvent> dis;
    ViewWindow o1;

    Callback<ControllerEvent,ViewWindow> c1(&o1, &ViewWindow::Notify, &ViewWindow::Death);

    dis.Subscribe(&c1);
    ControllerEvent ev;
    dis.Broadcast(ev);
    REQUIRE(ev.counter == 1);
    return PASSED;

}

test_status_t TwoObserverTest() 
{
    Dispatcher<ControllerEvent> dis;
    ViewWindow o1;
    ViewWindow o2;
    Callback<ControllerEvent,ViewWindow> c1(&o1, &ViewWindow::Notify, &ViewWindow::Death);
    Callback<ControllerEvent,ViewWindow> c2(&o2, &ViewWindow::Notify, &ViewWindow::Death);

    dis.Subscribe(&c1);
    dis.Subscribe(&c2);
    ControllerEvent ev;
    ev.counter = 0;
    dis.Broadcast(ev);
    REQUIRE(ev.counter == 2);

    return PASSED;
}

test_status_t ThreeObserverTest() 
{
    Dispatcher<ControllerEvent> dis;
    ViewWindow o1;
    ViewWindow o2;
    ViewWindow o3;
    Callback<ControllerEvent,ViewWindow> c1(&o1, &ViewWindow::Notify, &ViewWindow::Death);
    Callback<ControllerEvent,ViewWindow> c2(&o2, &ViewWindow::Notify, &ViewWindow::Death);
    Callback<ControllerEvent,ViewWindow> c3(&o3, &ViewWindow::Notify, &ViewWindow::Death);
    dis.Subscribe(&c1);
    dis.Subscribe(&c2);
    dis.Subscribe(&c3);
    ControllerEvent ev;
    ev.counter = 0;
    dis.Broadcast(ev);
    REQUIRE(ev.counter == 3);

    return PASSED;
}

test_status_t RemoveOneObserverTest() 
{
    Dispatcher<ControllerEvent> dis;
    ViewWindow o1;
    ViewWindow o2;
    
    Callback<ControllerEvent,ViewWindow> c1(&o1, &ViewWindow::Notify, &ViewWindow::Death);
    Callback<ControllerEvent,ViewWindow> c2(&o2, &ViewWindow::Notify, &ViewWindow::Death);
    
    dis.Subscribe(&c1);
    dis.Subscribe(&c2);
    {
    ViewWindow o3;

    Callback<ControllerEvent,ViewWindow> c3(&o3, &ViewWindow::Notify, &ViewWindow::Death);
    dis.Subscribe(&c3);
    }
    ControllerEvent ev;
    ev.counter = 0;
    dis.Broadcast(ev);
    REQUIRE(ev.counter == 2);

    return PASSED;
}

test_status_t RemoveThreeObserverTest() 
{
    Dispatcher<ControllerEvent> dis;
    
    {
    ViewWindow o1;
    ViewWindow o2;
    
    Callback<ControllerEvent,ViewWindow> c1(&o1, &ViewWindow::Notify, &ViewWindow::Death);
    Callback<ControllerEvent,ViewWindow> c2(&o2, &ViewWindow::Notify, &ViewWindow::Death);
    
    dis.Subscribe(&c1);
    dis.Subscribe(&c2);
    ViewWindow o3;

    Callback<ControllerEvent,ViewWindow> c3(&o3, &ViewWindow::Notify, &ViewWindow::Death);
    dis.Subscribe(&c3);
    ControllerEvent ev;
    ev.counter = 0;
    dis.Broadcast(ev);
    REQUIRE(ev.counter == 3);
    }
    ControllerEvent ev;
    ev.counter = 0;
    dis.Broadcast(ev);
    REQUIRE(ev.counter == 0);

    return PASSED;
}

class Event
{
public:

private:

};

class Observer
{
public:
    Observer():m_cb(this, &Observer::Notify, &Observer::Death){}
    void Notify(Event){cout << "Notify" << endl;}
    void Death(){cout << "Death" << endl;}
    void SubscribeMe(Dispatcher<Event>& d_){d_.Subscribe(&m_cb);}
private:
    Callback<Event, Observer> m_cb;
};

class Mediator
{
public:
    Mediator(): 
    m_observers(), 
    m_cb(this, &Mediator::Notify, &Mediator::Death),
    m_d()
    {
        m_d.Subscribe(&m_cb);
    }
    void Notify(Event)
    {
    
        m_observers.remove(*m_observers.begin());
        m_observers.remove(*m_observers.begin());
        m_observers.remove(*m_observers.begin());
        m_observers.push_front(boost::shared_ptr<Observer>(new Observer()));
        m_observers.begin()->get()->SubscribeMe(m_d);
        m_observers.push_front(boost::shared_ptr<Observer>(new Observer()));
        m_observers.begin()->get()->SubscribeMe(m_d);
    }
    void Death(){}
    void Run()
    {
        m_observers.push_front(boost::shared_ptr<Observer>(new Observer()));
        m_observers.push_front(boost::shared_ptr<Observer>(new Observer()));
        m_observers.push_front(boost::shared_ptr<Observer>(new Observer()));
        m_observers.push_front(boost::shared_ptr<Observer>(new Observer()));

        for (iter i = m_observers.begin(); i != m_observers.end(); ++i)
        {
            (*i)->SubscribeMe(m_d);
        }

        m_d.Broadcast(Event());

    }
private:

    typedef std::list<boost::shared_ptr<Observer> >::iterator iter;
    std::list<boost::shared_ptr<Observer> > m_observers;
    Callback<Event, Mediator> m_cb;
    Dispatcher<Event> m_d;
};

test_status_t MediatorTest()
{
    Mediator m;
    m.Run();
   

    sleep(2);

    return PASSED;
}



int main()
{
    RUNTEST(OneObserverTest);
    RUNTEST(TwoObserverTest);
    RUNTEST(ThreeObserverTest);
    RUNTEST(RemoveOneObserverTest);
    RUNTEST(RemoveThreeObserverTest);
    RUNTEST(MediatorTest);

    return PASSED;
}

*/

#include "dispatcher_callback.hpp"
#include "dispatcher_callback_impl.hpp"

#include "utils.hpp"

using namespace ilrd;
using namespace std;




class Event
{
public:
    Event(int id_ = 0)
    {
        m_id = id_;
    }
    static int counter;
    int m_id;
private:
};
int Event::counter = 0;

class Observer
{
public:
    Observer():id(counter_id++){}
    void Notify(Event ev)
    {
        ++ev.counter;
    }
    void Death()
    {
        std::cout<< "dispatcher died - observer:"<< id << "\n";
        --counter_id;
    }

private:
    int id;
    static int counter_id;
};
int Observer::counter_id = 0; 


class Mediator
{
public:
    Mediator(Dispatcher<Event> *dis):
    m_dis(dis)
    {
        addObservers(3);
    }

    void Notify(Event ev)
    {
        if(1 == ev.m_id)
        {
            m_observer.pop_back();
            m_Callbacks.pop_back();
        }
        else if(2 == ev.m_id)
        {
            m_observer.clear();
            m_Callbacks.clear();

        }
        else if(3 == ev.m_id)
        {
            addObservers(1);
        }
        else if(4 == ev.m_id)
        {
            addObservers(2);
        }
    }
    void Death()
    {
        std::cout<< "dispatcher died - Mediator\n";
    }
private:
    void addObservers(int num)
    {
        for(int i = 0; i < num; ++i)
        {
            Observer o;
            Callback<Event,Observer> cb(&o, &Observer::Notify, &Observer::Death);
            m_observer.push_front(o);
            m_Callbacks.push_front(cb);

            m_dis->Subscribe(&(m_Callbacks.front()));
        }
    }
    typedef typename std::list < Callback<Event, Observer> >::iterator iter;
    std::list <Observer> m_observer;
    std::list < Callback<Event, Observer> > m_Callbacks;
    Dispatcher<Event> *m_dis;
};


test_status_t RemoveOneObserverTest();
test_status_t RemoveAllObserverTest();

test_status_t AddOneObserverTest();
test_status_t AddTwoObserverTest();

test_status_t BroadcastTest()
{
    Dispatcher<Event> dis;
    Observer o1;
    Callback<Event,Observer> c1(&o1, &Observer::Notify, &Observer::Death);
    Observer o2;
    Callback<Event,Observer> c2(&o2, &Observer::Notify, &Observer::Death);

    dis.Subscribe(&c1);
    dis.Subscribe(&c2);
    Event ev1;
    ev1.counter = 0;
    dis.Broadcast(ev1);
    REQUIRE(ev1.counter == 2);
    {
        Observer o3;
        Callback<Event,Observer> c3(&o3, &Observer::Notify, &Observer::Death);
        dis.Subscribe(&c3);
        Observer o4;
        Callback<Event,Observer> c4(&o4, &Observer::Notify, &Observer::Death);
        dis.Subscribe(&c4);
        Event ev2;
        dis.Broadcast(ev2);
        REQUIRE(ev2.counter == 6);
    }
    dis.Broadcast(ev1);
    REQUIRE(ev1.counter == 8);

    return PASSED; 
}

test_status_t RemoveOneObserverTest()
{
    Dispatcher<Event> dis;
    Mediator m(&dis);

    Callback<Event,Mediator> c1(&m, &Mediator::Notify, &Mediator::Death);
    dis.Subscribe(&c1);

    Event ev(1);
    ev.counter = 0;
    dis.Broadcast(ev);
    REQUIRE(ev.counter == 2);

    dis.Broadcast(ev);
    REQUIRE(ev.counter == 3);

    return PASSED; 
}

test_status_t RemoveAllObserverTest()
{
    Dispatcher<Event> dis;

    Mediator m(&dis);

    Callback<Event,Mediator> c1(&m, &Mediator::Notify, &Mediator::Death);
    dis.Subscribe(&c1);

    Event ev;
    ev.counter = 0;
    dis.Broadcast(ev);
    REQUIRE(ev.counter == 3);

    Event ev2(2);
    dis.Broadcast(ev2);
    REQUIRE(ev2.counter == 3);

    return PASSED; 
}

test_status_t AddOneObserverTest()
{
    Dispatcher<Event> dis;
    Mediator m(&dis);

    Callback<Event,Mediator> c1(&m, &Mediator::Notify, &Mediator::Death);
    dis.Subscribe(&c1);

    Event ev(3);
    ev.counter = 0;
    dis.Broadcast(ev);
    REQUIRE(ev.counter == 3);

    Event ev2;
    dis.Broadcast(ev2);
    REQUIRE(ev2.counter == 7);
    
    return PASSED; 
}

test_status_t AddTwoObserverTest()
{
    Dispatcher<Event> dis;
    Mediator m(&dis);

    Callback<Event,Mediator> c1(&m, &Mediator::Notify, &Mediator::Death);
    dis.Subscribe(&c1);

    Event ev(4);
    ev.counter = 0;
    dis.Broadcast(ev);
    REQUIRE(ev.counter == 3);

    Event ev2;
    dis.Broadcast(ev2);
    REQUIRE(ev2.counter == 8);
    
    return PASSED; 
}

int main()
{
    RUNTEST(BroadcastTest);
    RUNTEST(RemoveOneObserverTest);
    RUNTEST(RemoveAllObserverTest);
    RUNTEST(AddOneObserverTest);
    RUNTEST(AddTwoObserverTest);

    return PASSED;
}
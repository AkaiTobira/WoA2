#ifndef OBSERVER_H
#define OBSERVER_H

#include "EntityMessage.h"
#include "iostream"
#include "iomanip"
#include "set"


struct TwoFloats
{
    float m_x;
    float m_y;
};


using MessageType = unsigned int;

struct Message
{
    Message(const MessageType& l_type) : m_type(l_type) {}
    MessageType m_type;
    int m_sender;

    std::set<unsigned int> m_receivers;

    union
    {
        TwoFloats m_2f;
        bool m_bool;
        int m_int;
    };
};


class Observer
{
public:
    virtual ~Observer() {}    
    virtual void Notify(const Message& l_message) = 0;
};



using ObserverContainer = std::vector<Observer*>;

class Communicator
{
public:
    ~Communicator()
    {
        m_observers.clear();
    }
    bool AddObserver(Observer* l_observer)
    {
        if (HasObserver(l_observer))
        {
            return false;
        }
        m_observers.emplace_back(l_observer);
        return true;
    }
    bool RemoveObserver(Observer* l_observer)
    {
        auto observer = std::find_if(m_observers.begin(),      m_observers.end(), [&l_observer](Observer* o)
        {
            return o == l_observer;
        });
        if (observer == m_observers.end())
        {
            return false;
        }
        m_observers.erase(observer);
        return true;
    }
    bool HasObserver(const Observer* l_observer)
    {
        return (std::find_if(m_observers.begin(), m_observers.end(),      [&l_observer](Observer* o)
        {
            return o == l_observer;
        }) != m_observers.end());
    }
    void Broadcast(const Message& l_msg)
    {

        for(auto& itr : m_observers)
        {       
            itr->Notify(l_msg);

        }

    }
private:
    ObserverContainer m_observers;
};

using Subscribtions = std::unordered_map<EntityMessage,Communicator>;

class MessageHandler
{
public:
    bool Subscribe(const EntityMessage& l_type, Observer* l_observer)
    {
        return m_communicators[l_type].AddObserver(l_observer);
    }
    bool Unsubscribe(const EntityMessage& l_type,    Observer* l_observer)
    {
        return m_communicators[l_type].RemoveObserver(l_observer);
    }
    void Dispatch(const Message& l_msg)
    {

        auto itr = m_communicators.find((EntityMessage)l_msg.m_type);
        if (itr == m_communicators.end())
        {
            return;
        }

        itr->second.Broadcast(l_msg);
    } 
private:
    Subscribtions m_communicators;
};



#endif // OBSERVER_H

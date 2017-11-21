#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include "SharedElements.h"
#include "ResourceManager.h"
#include "EventManager.h"
#include "Utilities.h"
#include <functional>

class StateManager;

class BaseState
{
    friend class StateManager;
public:
    BaseState(StateManager * l_stateManager) : m_stateMgr(l_stateManager), m_transparent(false), m_transcendent(false) {}
    virtual ~BaseState() {}

    virtual void OnCreate() = 0;
    virtual void OnDestroy() = 0;

    virtual void Activate() = 0;
    virtual void Deactivate() = 0;

    virtual void Update(const sf::Time& l_time) = 0;
    virtual void Draw() = 0;

    bool IsTransparent() const
    {
        return m_transparent;
    }
    void SetTranscendent(const bool & l_transcendent)
    {
        m_transcendent = l_transcendent;
    }

    void SetTransparent(const bool & l_transparent)
    {
        m_transparent = l_transparent;
    }

    bool IsTranscendent() const
    {
        return m_transcendent;
    }
    StateManager* GetStateManager()
    {
        return m_stateMgr;
    }
    sf::View& GetView()
    {
        return m_view;
    }
protected:
    StateManager* m_stateMgr;
    sf::View m_view;
    bool m_transparent;
    bool m_transcendent;
    bool m_fullscreen;

};


#include "EntityManager.h"



using StateContainer = std::vector<std::pair<StateType,BaseState*> >;
using TypeContainer = std::vector<StateType>;
using StateFactory = std::unordered_map<StateType, std::function<BaseState*(void)>,EnumClassHash >;

class StateManager
{
public:
    StateManager( SharedContext* l_shared);
    virtual ~StateManager();
    void Draw();
    void Update(const sf::Time & l_time);
    void ProcessRequests();

        SharedContext* GetContext();
        bool HasState(const StateType& l_type);

        void SwitchTo(const StateType& l_type);
        void Remove(const StateType& l_type);
protected:

private:

    void CreateState(const StateType& l_type);
    void RemoveState(const StateType& l_type);

    template<class T>
    void RegisterState(const StateType& l_type)
    {
        m_stateFactory[l_type] = [this]()->BaseState*
        {
            return new T(this);
        };


    }

    SharedContext* m_shared;
    StateContainer m_states;
    TypeContainer m_toRemove;
    StateFactory m_stateFactory;
};


#endif // STATEMANAGER_H

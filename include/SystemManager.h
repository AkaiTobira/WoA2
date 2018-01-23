#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include "ECS_Types.h"
#include "Observer.h"

#include "Window.h"
#include "Bitmask.h"
#include "EventQueue.h"
#include "set"
//#include "S_Base.h"



class SystemManager;
class Map;

using EntityId = unsigned int;
using EntityList = std::vector<EntityId>;
using Requirements = std::vector<Bitmask>;

class S_Base : public Observer
{
public:
    S_Base(const System& l_id, SystemManager* l_systemMgr);
    virtual ~S_Base();
    bool AddEntity(const EntityId& l_entity);
    bool HasEntity(const EntityId& l_entity);
    void SortEntities();
    virtual bool RemoveEntity(const EntityId& l_entity) = 0;
    System GetId();
    bool FitsRequirements(const Bitmask& l_bits);
    void Purge();
    virtual void Update(float l_dT) = 0;
    virtual void HandleEvent(
        const std::set<unsigned int>& participians,
        const EntityEvent& l_events
    ) = 0;

    EntityList GiveList();

    virtual void HandleEvent(const EntityId& l_entity, const EntityEvent& l_event) = 0;
protected:
    System m_id;
    Requirements m_requiredComponents;
    EntityList m_entities;
    SystemManager* m_systemManager;
    Map* m_gameMap;
};

using SystemContainer = std::unordered_map<System,S_Base*>;
using EntityEventContainer = std::unordered_map<  EntityId, EventQueue>;

class EntityManager;

#include "S_Renderer.h"
#include "S_State.h"
#include "S_Control.h"
#include "S_Movement.h"
#include "S_SheetAnimation.h"
#include "S_Collision.h"

class SystemManager
{
public:
    SystemManager();
    ~SystemManager();
    void SetEntityManager(EntityManager* l_entityMgr);
    EntityManager* GetEntityManager();
    MessageHandler* GetMessageHandler();
    template<class T>  T* GetSystem(const System& l_system)
    {
        auto itr = m_systems.find(l_system);
        return(itr != m_systems.end() ?    dynamic_cast<T*>(itr->second) : nullptr);
    }
    void AddEvent(const EntityId& l_entity, const EventID& l_event);
    void Update(float l_dT);
    void HandleEvents();
    void Draw(Window* l_wind, unsigned int l_elevation);
    void EntityModified(const EntityId& l_entity,    const Bitmask& l_bits);
    void RemoveEntity(const EntityId& l_entity);
    void PurgeEntities();
    void PurgeSystems();
    bool CheckEntityLists();

 EntityList GetList();
    
private:
    SystemContainer m_systems;
    EntityManager* m_entityManager;
    EntityEventContainer m_events;
    MessageHandler m_messages;

    std::set<unsigned int> events[(int)EntityEvent::END];
    unsigned int m_partOfHandling; 
};

#endif // SYSTEMMANAGER_H

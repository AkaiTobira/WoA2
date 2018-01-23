#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H


#include "C_Position.h"
#include "C_SpriteSheet.h"
#include "C_Movable.h"
#include "C_Controller.h"
#include "C_State.h"
#include "C_Collidable.h"
#include "Bitmask.h"
#include "EventQueue.h"
#include "SystemManager.h"
#include "Window.h"


#include <functional>

using EntityId = unsigned int;
using ComponentContainer = std::vector<C_Base*>;
using EntityData = std::pair<Bitmask,ComponentContainer>;
using EntityContainer = std::unordered_map<EntityId,EntityData>;
using ComponentFactory = std::unordered_map<Component,std::function<C_Base*(void)>>;


class TextureManager;

class EntityManager
{
public:
    EntityManager(SystemManager* l_sysMgr, TextureManager* l_textureMgr);
    virtual ~EntityManager();

    int AddEntity(const Bitmask& l_mask);
    int AddEntity(const std::string& l_entityFile);
    bool RemoveEntity(const EntityId& l_id);
    bool AddComponent(const EntityId& l_entity, const Component& l_component);
    template<class T>
    T* GetComponent(const EntityId& l_entity, const Component& l_component)
    {

        auto itr = m_entities.find(l_entity);
        if (itr == m_entities.end())
        {

            return nullptr;
        }
            // Found the entity.
        if (!itr->second.first.GetBit((unsigned int)l_component))
        {

            return nullptr;
        }
            // Component exists.
        auto& container = itr->second.second;
        auto component = std::find_if(container.begin(),container.end(),[&l_component](C_Base* c)
        {

            #ifdef DEBUGG_RUN

            std::cout <<"EntityManager :: GETCOMPONENT :: " << (unsigned int)l_component << std::endl;

            #endif
            return c->GetType() == l_component;
        });
        return (component != container.end() ? dynamic_cast<T*>(*component) : nullptr);
    }
    bool RemoveComponent(const EntityId& l_entity, const Component& l_component);
    bool HasComponent(const EntityId& l_entity, const Component& l_component);
    void Purge();
    
protected:

private:
    EntityContainer m_entities;
    template<class T>
    void AddComponentType(const Component& l_id)
    {
        m_cFactory[l_id] = []()->C_Base* { return new T(); };
    }
    unsigned int m_idCounter;

    ComponentFactory m_cFactory;
    SystemManager* m_systems;
    TextureManager* m_textureManager;

  //  EntityContainer m_entities;

};



#endif // ENTITYMANAGER_H

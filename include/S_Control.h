
#ifndef S_CONTROL
#define S_CONTROL

#include "EntityManager.h"

class S_Control : public S_Base{
public:
    S_Control(SystemManager* l_systemMgr);
    void HandleEvent(const EntityId& l_entity,const EntityEvent& l_event);
    void MoveEntity(const EntityId& l_entity,const Direction& l_dir);
    void Notify(const Message& l_message );
    void Update(float l_dt);

    bool RemoveEntity(const EntityId& l_entity);
    void ActivateUnit(const EntityId& l_entity);
    void ReleaseUnit(const EntityId& l_entity);
    bool FindUnit( float l_x, float l_y );
    bool FindUnit( sf::Vector2f& l_x, sf::Vector2f& l_y );
    void ReleaseUnits( );
    unsigned int IsActive( const EntityId& l_entity);
    void HandleEvent(
        const std::set<unsigned int>& participians,
        const EntityEvent& l_events
    );
};

#endif
#ifndef S_MOVEMENT
#define S_MOVEMENT


#include "EntityManager.h"


enum class Axis{ x, y };



class S_Movement : public S_Base{
public:
    S_Movement(SystemManager* l_systemMgr);
    ~S_Movement();

    void Update(float l_dT);
    void HandleEvent(const EntityId& l_entity,const EntityEvent& l_event);
    void Notify(const Message& l_message);
    void SetMap(Map* l_gameMap);
    void StopEntity(const EntityId& l_entity);
    void SetTargetPosition(const EntityId& l_entity, float l_x, float l_y);
    void SetPosition(float l_x, float l_y, const EntityId& l_entity);
    void SetTargetPosition( float l_x, float l_y);
    void HandleEvent(
        const std::set<unsigned int>& participians,
        const EntityEvent& l_events
    );
    void SetDirection(
        const std::set<unsigned int>& participians,
        const Direction& l_dir
    );
private:

    std::stack<SE>& CalculateRoad(std::pair<unsigned int, unsigned int>** previous, sf::Vector2u& start, sf::Vector2u& target);
    std::stack<SE>& AStar(unsigned int& l_entity);

    void SendMessage( 
        const  EntityMessage& l_type,
        int information,
        std::set<unsigned int>& recivers 
    );
    void StopEntity(const EntityId& l_entity, const Axis& l_axis);
    void SetDirection(const EntityId& l_entity, const Direction& l_dir);
    const sf::Vector2f& GetTileFriction(unsigned int l_elevation, unsigned int l_x, unsigned int l_y);
    void MovementStep(float l_dT, C_Movable* l_movable, C_Position* l_position);
    void CalculatePath(C_Position* position, C_Movable* movable);
        
    sf::Vector2u m_mapSize;
};

struct QE{
    QE(const unsigned int o, const unsigned int d,const unsigned int t):
    u1(o),u2(d),u3(t){}
    unsigned int u1;
    unsigned int u2;
    unsigned int u3;
};




#endif
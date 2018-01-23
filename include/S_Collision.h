#ifndef S_COLLISION_H
#define S_COLLISION_H

//#include "EntityManager.h"
#include "Map.h"
//#include "stdlib.h"

struct CollisionElement{
    CollisionElement(float l_area, TileInfo* l_info,const sf::FloatRect& l_bounds):
    m_area(l_area),
    m_tile(l_info), 
    m_tileBounds(l_bounds){ }

    float m_area;
    TileInfo* m_tile;
    sf::FloatRect m_tileBounds;

};

using Collisions = std::vector<CollisionElement>;

class S_Collision : public S_Base{
public:
    S_Collision(SystemManager* l_systemMgr);
    ~S_Collision();
 //   void HandleEvent(const EntityId& l_entity,const EntityEvent& l_event);
 //   void MoveEntity(const EntityId& l_entity,const Direction& l_dir);
    void Notify(const Message& l_message );
    void Update(float l_dt);

 //   void ActivateUnit(const EntityId& l_entity);
  //  void ReleaseUnit(const EntityId& l_entity);
 //   bool FindUnit( float l_x, float l_y );
 //   bool FindUnit( sf::Vector2f& l_x, sf::Vector2f& l_y );
  //  void ReleaseUnits( );

    bool RemoveEntity(const EntityId& l_entity);

    void ReleaseUnit(const unsigned int & l_entity);
    void HandleEvent(
        const std::set<unsigned int>& participians,
        const EntityEvent& l_events
    );

    void HandleEvent(const EntityId& l_entity, const EntityEvent& l_event);
    

    void SetMap(Map* l_map);
    void CheckOutOfBounds(C_Position* l_pos,C_Collidable* l_col);
    void MapCollisions(const EntityId& l_entity,C_Position* l_pos, C_Collidable* l_col);
    void EntityCollisions();

    void PlaceUnit( float l_x, float l_y, unsigned int l_entity);
    void ReplaceUnit(float l_x, float l_y, unsigned int& l_entity);

    


 //   void DeleteFromSquare( sf::Vector2u l_set, unsigned int & l_entity);

private:
    void SwitchSquare(sf::Vector2f l_pos, sf::Vector2u l_set, unsigned int&l_entity);
    void DeleteFromSquare( sf::Vector2u l_set, unsigned int & l_entity);
    bool MovingTheSameDirection( sf::Vector2f& p1, sf::Vector2f& p2 );

    std::set<unsigned int>** m_squares;
    sf::Vector2u m_setsSizes;
    sf::Vector2u mapSize;

    const int SQUARE_SIZE = 320;
};



#endif
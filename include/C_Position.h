#ifndef C_POSITION_H
#define C_POSITION_H

#include "ECS_Types.h"
#include <iostream>


class C_Position: public C_Base
{
public:
    C_Position(): 
        C_Base(Component::Position), 
        m_elevation(0),
        m_moving(false), 
        m_movePause(false) {};

    ~C_Position() {};

    void ReadIn(std::stringstream& l_stream)
    {
        l_stream >> m_fPosition.x >> m_fPosition.y >> m_elevation;

        m_uPosition = sf::Vector2u( m_fPosition.x/Tile_Size, m_fPosition.y/Tile_Size);
        
        m_uPositionOld = m_uPosition;
        m_fPositionOld = m_fPosition;
    }

    const sf::Vector2f& GetFPosition(){
        return m_fPosition;
    }

    const sf::Vector2f& GetOldPosition()
    {
        return m_fPositionOld;
    }  

    unsigned int GetElevation()
    {
        return m_elevation;
    }

    const sf::Vector2u& GetUPosition(){
        return m_uPosition;
    }


    void SetPosition(float l_x, float l_y)
    {
        m_fPositionOld = m_fPosition;
        m_fPosition = sf::Vector2f( l_x         ,  l_y          );

 
        m_uPositionOld = m_uPosition;
        m_uPosition = sf::Vector2u( m_fPosition.x/Tile_Size, m_fPosition.y/Tile_Size);

 
    }

    void SetPosition(const sf::Vector2f& l_vec)
    {
        m_fPositionOld = m_fPosition;
        m_fPosition = l_vec;

        m_uPositionOld = m_uPosition;
        m_uPosition = sf::Vector2u( static_cast<unsigned int>(m_fPosition.x/Tile_Size), static_cast<unsigned int>(m_fPosition.y/Tile_Size ));
  }

    void SetElevation(unsigned int l_elevation)
    {
        m_elevation = l_elevation;
    }

    void MoveBy(float l_x, float l_y)
    {
        if( !m_movePause ){
            m_fPositionOld = m_fPosition;
            m_fPosition += sf::Vector2f(l_x,l_y);

            m_uPositionOld = m_uPosition;
            m_uPosition = sf::Vector2u( static_cast<unsigned int>(m_fPosition.x/Tile_Size), static_cast<unsigned int>(m_fPosition.y/Tile_Size)) ;
            }
    }
    
    void MoveBy(const sf::Vector2f& l_vec)
    {
        if( !m_movePause ){

            m_fPositionOld = m_fPosition;
            m_fPosition += l_vec;

            m_uPositionOld = m_uPosition;
            m_uPosition = sf::Vector2u( m_fPosition.x/Tile_Size, m_fPosition.y/Tile_Size);

       }
    }

    sf::Vector2u& GetBlockTile(){
        return m_uPosition;
    }

    sf::Vector2u& GetBlockTileOld(){
        return m_uPositionOld;
    }

    bool BlockChanged(){
        return m_uPosition != m_uPositionOld;
    }

    bool isMoving(){
        return m_moving;
    }

    void StopMoving(){
        m_moving = false;
    }

    void Moving(){
        m_moving = true;
    }

    void Play(){
        m_movePause = false;
    }

    void Pause(){
        m_movePause = true;
    }

    bool IsPaused(){
        return m_movePause;
    }

private:
    const int Tile_Size = 32;

    sf::Vector2f m_fPosition;
    sf::Vector2f m_fPositionOld;
 
    sf::Vector2u m_uPosition; // Is also block position
    sf::Vector2u m_uPositionOld;

    unsigned int m_elevation;
    unsigned int m_pauseCounter;

    bool m_moving;
    bool m_movePause;
};

#endif // C_POSITION_H

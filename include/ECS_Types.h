
#pragma once
#ifndef ECSTYPES_H
#define ECSTYPES_H

#include <vector>
#include <unordered_map>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "Map.h"
#include <queue>
#include <utility>
#include <stack>


using ComponentType = unsigned int;
#define N_COMPONENT_TYPES 32


enum class Component
{
    Position = 0,
    SpriteSheet,
    State,
    Movable,
    Controller,
    Collidable
};

enum class System
{
    Renderer = 0,
    Movement,
    Collision,
    Control,
    State,
    SheetAnimation

};

enum class EntityEvent
{
    Spawned, 
    Moving_Left, 
    Despawned, 

    Colliding_X, 
    Moving_Right, 
    Elevation_Change, 

    Colliding_Y,
    Moving_Up, 
    Became_Idle, 

    Moving_Down,
    Began_Moving,

    END
};


class C_Base
{
public :
    C_Base(const Component& l_type): m_type(l_type) {}
    virtual ~C_Base() {};

    Component GetType()
    {
        return m_type;
    }

    friend std::stringstream& operator >>( std::stringstream& l_stream, C_Base& b)
    {
        b.ReadIn(l_stream);
        return l_stream;
    }

    virtual void ReadIn(std::stringstream& l_stream) = 0;
protected:
    Component m_type;

};
class C_Drawable : public C_Base{
     public:  
        C_Drawable(const Component& l_type) : C_Base(l_type){}  
        virtual ~C_Drawable(){}
        virtual void UpdatePosition(const sf::Vector2f& l_vec) = 0;  
        virtual const sf::Vector2i GetSize() = 0;  
        virtual void Draw(sf::RenderWindow* l_wind) = 0; 
    private:
};

struct SE{
    SE( Direction l_dir , const unsigned int d, const unsigned int t):m_dir(l_dir), m_vec(sf::Vector2u(d,t)){}
    SE( Direction l_dir, sf::Vector2u d): m_dir(l_dir),m_vec(d){}

    Direction m_dir;
    sf::Vector2u m_vec;
};

#endif // ECSTYPES_H

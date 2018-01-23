
#pragma once
#ifndef ECSTYPES_H
#define ECSTYPES_H

#include <vector>
#include <unordered_map>
#include <sstream>
#include <SFML/Graphics.hpp>

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
    
    Colliding_Left,
    Moving_Left, 
    Moving_LeftDown,
    Despawned, 

    Colliding_Right, 
    Moving_Right, 
    Moving_RightUp,
    Elevation_Change, 

    Colliding_Up,
    Moving_Up, 
    Moving_LeftUp,
    Became_Idle, 

    Colliding_Down,
    Moving_Down,
    Moving_RightDown,
    Began_Moving,

    Spawned, 
    ForceStop,
    Colliding_Move,

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



#endif // ECSTYPES_H

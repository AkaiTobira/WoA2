#ifndef C_COLLIS_H
#define C_COLLIS_H

#include "ECS_Types.h"


enum class Origin{ Top_Left, Abs_Centre, Mid_Bottom };

class C_Collidable : public C_Base{
public:

    C_Collidable(): C_Base(Component::Collidable),
    m_origin(Origin::Mid_Bottom), 
    m_collidingOnX(false),
    m_collidingOnY(false)
    {}
    
    void ReadIn(std::stringstream& l_stream){
        unsigned int origin = 0;
        l_stream >> m_AABB.width >> m_AABB.height >> m_offset.x >> m_offset.y >> origin;
        
        m_onceStoped = false;
        switch(origin){
            case 0:
                m_origin = Origin::Abs_Centre;
            break;
            case 1:
                m_origin = Origin::Mid_Bottom;
            break;
            case 2:
                m_origin = Origin::Top_Left;
            break;
            default:
            break;
        }

    }

    void MarkStop(){
        m_onceStoped = true;
    }

    void Unmark(){
        m_onceStoped = false;
    }

    bool wasStoped(){
        return m_onceStoped;
    }


    void CollideOnX(){ m_collidingOnX = true; }

    void CollideOnY(){ m_collidingOnY = true; }

    void ResetCollisionFlags(){
        m_collidingOnX = false;
        m_collidingOnY = false;
    }

    void SetSize(const sf::Vector2f& l_vec){
        m_AABB.width = l_vec.x;
        m_AABB.height = l_vec.y;
    }

    void SetPosition(const sf::Vector2f& l_vec){
        switch(m_origin){
            case Origin::Top_Left:
                m_AABB.left = l_vec.x + m_offset.x;
                m_AABB.top = l_vec.y + m_offset.y;
            break;
            case Origin::Abs_Centre:
                m_AABB.left = l_vec.x - (m_AABB.width / 2) + m_offset.x;
                m_AABB.top = l_vec.y - (m_AABB.height / 2) + m_offset.y;
            break;
            case Origin::Mid_Bottom:
                m_AABB.left = l_vec.x - (m_AABB.width / 2) + m_offset.x;
                m_AABB.top = l_vec.y - m_AABB.height + m_offset.y;
            break;
            }
        }
    
    sf::FloatRect GetCollidable(){
        return m_AABB;
    }

    sf::Vector2f GetPosition(){
        return sf::Vector2f(m_AABB.left, m_AABB.top);
    }

    sf::Vector2f GetSize(){
        return sf::Vector2f(m_AABB.width, m_AABB.height);
    }

private:
    sf::FloatRect m_AABB;
    sf::Vector2f m_offset;
    Origin m_origin;
    bool m_collidingOnX;
    bool m_collidingOnY;
    bool m_onceStoped;
};

#endif
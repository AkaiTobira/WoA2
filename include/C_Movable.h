#ifndef C_MOVABLE_H
#define C_MOVABLE_H

#include "ECS_Types.h"
#include "cmath"


class C_Movable : public C_Base{
public:
    C_Movable() : C_Base(Component::Movable), 
    m_velocity(0.f,0.f),
    m_modificator(0.f,0.f),
    m_direction((Direction)0){}

    void ReadIn(std::stringstream& l_stream){
        int m_velocityMax;
        l_stream >> m_velocityMax >> m_speed.x >> m_speed.y;

        unsigned int dir = 0;
        l_stream >> dir;
        m_direction = (Direction)dir;
    }


    void AddSpeed(const sf::Vector2f& l_vec){
        m_modificator += l_vec;
    }

    void SetModificator(float l_x, float l_y){
        m_modificator = sf::Vector2f(l_x, l_y);
    }

    void SetModificator(const sf::Vector2f& l_vec){
        m_modificator = l_vec;
    }

    void Move(const Direction& l_dir){


        #ifdef SDR_RUN
            std::cout << "Entity "  << " :: " << m_modificator.x <<" " << m_modificator.y <<std::endl;
        #endif


        if(l_dir == Direction::Up){
            m_velocity.y = - (m_modificator.y);

        } else if (l_dir == Direction::Down){
            m_velocity.y = (m_modificator.y);
                
        } else if (l_dir == Direction::Left){
            m_velocity.x = -(m_modificator.x);

        } else if (l_dir == Direction::Right){
            m_velocity.x = (m_modificator.x);
        }

        #ifdef SDR_RUN
            std::cout << "Entity "  << " :: " << m_velocity.x <<" " << m_velocity.y <<std::endl;
        #endif

    }

    sf::Vector2f GetVelocity(){
        return m_velocity;
    }

    sf::Vector2f GetSpeed(){
        return m_speed;
    }

    void SetVelocity( sf::Vector2f l_vel){
        m_velocity = l_vel;
    }

    void SetDirection( Direction l_dir){
        m_direction = l_dir;
    }

    Direction GetDirection( ){
        return m_direction;
    }

private:

    sf::Vector2f m_velocity;
    sf::Vector2f m_speed;
    sf::Vector2f m_modificator;
    Direction m_direction;
};

#endif
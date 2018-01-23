#ifndef C_MOVABLE_H
#define C_MOVABLE_H

#include "ECS_Types.h"
#include "cmath"
#include "ResourceManager.h"
#include <stack>

struct SE{
    SE( Direction l_dir , const unsigned int d, const unsigned int t):m_dir(l_dir), m_vec(sf::Vector2u(d,t)){}
    SE( Direction l_dir, sf::Vector2u d): m_dir(l_dir),m_vec(d){}

    Direction m_dir;
    sf::Vector2u m_vec;
    
};

class C_Movable : public C_Base{
public:
    C_Movable() : C_Base(Component::Movable), 
    m_velocity(0.f,0.f),
    m_modificator(0.f,0.f),
    m_dir((Direction)0){}

    void ReadIn(std::stringstream& l_stream){
        int m_velocityMax;
        l_stream >> m_velocityMax >> m_speed.x >> m_speed.y;

        unsigned int dir = 0;
        l_stream >> dir;
        m_dir = (Direction)dir;
        m_movePause = false;
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

    void CalculateSpeed(){
        switch( m_dir){
            case Direction::Up:
                m_velocity.y = -m_modificator.y;
                m_velocity.x = 0;
                break;     
            case Direction::Down:
                m_velocity.y =  m_modificator.y;
                m_velocity.x = 0;
                break;
            case Direction::Left:
                m_velocity.x = -m_modificator.x;
                m_velocity.y = 0;
                break;
            case Direction::Right:
                m_velocity.x =  m_modificator.x;
                m_velocity.y = 0;
                break;
            case Direction::RightUp:
                m_velocity.x =  m_modificator.x;
                m_velocity.y = -m_modificator.y;
                break;
            case Direction::RightDown:
                m_velocity.x =  m_modificator.x;
                m_velocity.y =  m_modificator.y;
                break;
            case Direction::LeftUp:
                m_velocity.y = -m_modificator.y;
                m_velocity.x = -m_modificator.x;
                break;
            case Direction::LeftDown:
                m_velocity.x = -m_modificator.x;
                m_velocity.y =  m_modificator.y;
                break;
            default:
                m_velocity.x = 0;
                m_velocity.y = 0;
                break;
        }
    }

    void CalculateSpeed(const Direction& l_dir){

        m_dir = l_dir;

        switch( l_dir){
            case Direction::Up:
                m_velocity.y = -m_modificator.y;
                m_velocity.x = 0;
                break;     
            case Direction::Down:
                m_velocity.y =  m_modificator.y;
                m_velocity.x = 0;
                break;
            case Direction::Left:
                m_velocity.x = -m_modificator.x;
                m_velocity.y = 0;
                break;
            case Direction::Right:
                m_velocity.x =  m_modificator.x;
                m_velocity.y = 0;
                break;
            case Direction::RightUp:
                m_velocity.x =  m_modificator.x;
                m_velocity.y = -m_modificator.y;
                break;
            case Direction::RightDown:
                m_velocity.x =  m_modificator.x;
                m_velocity.y =  m_modificator.y;
                break;
            case Direction::LeftUp:
                m_velocity.y = -m_modificator.y;
                m_velocity.x = -m_modificator.x;
                break;
            case Direction::LeftDown:
                m_velocity.x = -m_modificator.x;
                m_velocity.y =  m_modificator.y;
                break;
            default:
                m_velocity.x = 0;
                m_velocity.y = 0;
                break;
        }
    }

    sf::Vector2f GetVelocity(){

        return m_velocity;
    }

    sf::Vector2f GetSpeed(){
        return m_speed;
    }

    void SetDirection( Direction l_dir){
        m_dir = l_dir;
    }

    void StartMove(){
        m_walking = true;
    }


    void StopMove(){
        m_walking = false;
    }

    bool isWalking(){
        return m_walking;
    }

    Direction GetDirection( ){
        return m_dir;
    }

    sf::Vector2f& GetFTargetPosition(){
        return m_fTarget;
    }

    sf::Vector2u& GetUTargetPosition(){
        return m_uTarget;
    }

    Direction DirectionCheck(){
        if( m_steps.empty() ){
            return Direction::None;
        }
        return m_steps.top().m_dir;
    }

    Direction GetNextStep(){

        if( m_steps.empty() ){
             return Direction::None; 
        }

        m_steps.pop();
        SE temp  = m_steps.top();
        m_checkpoint = temp.m_vec;
    
        return temp.m_dir;
    }

    bool MoreSteps(){
        return m_steps.empty();
    }

    int GetStackSize(){
        return m_steps.size();
    }

    void SetTargetPosition(float l_x, float l_y){
        m_fTarget = sf::Vector2f( l_x, l_y);
        m_uTarget = sf::Vector2u(
            static_cast<unsigned int>(l_x/Tile_Size),
            static_cast<unsigned int>(l_y/Tile_Size)
        );
    }

    void SetTargetPosition(sf::Vector2f& l_vec){
        m_fTarget = l_vec;
        m_uTarget = sf::Vector2u(
            static_cast<unsigned int>(l_vec.x/Tile_Size),
            static_cast<unsigned int>(l_vec.y/Tile_Size)
        );
    }

    void SetCheckpoint(sf::Vector2u& l_vec){
        m_checkpoint = l_vec;
    }

    sf::Vector2u& GetCheckpoint(){
        return m_checkpoint;
    }


    void SetVelocity( const sf::Vector2f l_vec){
        m_velocity = l_vec;
    }

    void SetStackSteps(std::stack<SE> l_steps){
        m_steps = l_steps;
        if( m_steps.empty() ) {
            m_checkpoint = m_uTarget;
        }else{
            m_checkpoint = m_steps.top().m_vec;
        }
    }

    void SetCurrentReservation( sf::Vector2u l_vec ){
        m_reservedPrevPosition = m_reservedCurrPosition;
        m_reservedCurrPosition = l_vec;
    }

    sf::Vector2u& GetPrevReservation(){
        return m_reservedPrevPosition;
    }

    sf::Vector2u& GetCurrentReservation(){
        return m_reservedCurrPosition;
    }


private:
    const float Tile_Size = 32;

    sf::Vector2f m_velocity;
    sf::Vector2f m_speed;
    sf::Vector2f m_modificator;

    sf::Vector2u m_checkpoint;

    sf::Vector2f m_fTarget;
    sf::Vector2u m_uTarget;

    sf::Vector2u m_reservedCurrPosition;
    sf::Vector2u m_reservedPrevPosition;

    std::stack<SE> m_steps;

    Direction m_dir;
    bool m_walking;
    bool m_movePause;
};

#endif
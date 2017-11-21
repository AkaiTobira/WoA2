#ifndef C_POSITION_H
#define C_POSITION_H

#include "ECS_Types.h"


class C_Position: public C_Base
{
public:
    C_Position(): C_Base(Component::Position), m_elevation(0) {};
    ~C_Position() {};

    void ReadIn(std::stringstream& l_stream)
    {
        l_stream >> m_position.x >> m_position.y >> m_elevation;
        m_targetPosition = m_position;
        flag = false;
    }

    const sf::Vector2f& GetPosition(){
        return m_position;
    }

    const sf::Vector2f& GetOldPosition()
    {
        return m_positionOld;
    }  

    unsigned int GetElevation()
    {
        return m_elevation;
    }

    void SetPosition(float l_x, float l_y)
    {
        m_positionOld = m_position;
        m_position = sf::Vector2f(l_x,l_y);
    }

    void SetPosition(const sf::Vector2f& l_vec)
    {
        m_positionOld = m_position;
        m_position = l_vec;
    }

    void SetElevation(unsigned int l_elevation)
    {
        m_elevation = l_elevation;
    }

    void MoveBy(float l_x, float l_y)
    {
        m_positionOld = m_position;
        m_position += sf::Vector2f(l_x,l_y);
    }
    
    void MoveBy(const sf::Vector2f& l_vec)
    {
        
        m_positionOld = m_position;
        m_position += l_vec;
    }

    sf::Vector2f& GetTargetPosition(){
        return m_targetPosition;
    }

    void SetTargetPosition(float l_x, float l_y){
        m_targetPosition = sf::Vector2f( l_x, l_y);
    }

    void SetTargetPosition(sf::Vector2f& l_vec){
        m_targetPosition = l_vec;
    }

    void SetTempTargetPosition(sf::Vector2u& l_vec){
        m_tempTargetPosition = l_vec;
    }

    sf::Vector2u& GetTempTargetPosition(){
        return m_tempTargetPosition;
    }

    Direction GetNextStep(){
        SE temp = m_steps.top();
        m_tempTargetPosition = temp.m_vec;
        m_steps.pop();
        return temp.m_dir;
    }

    void SetStackSteps(std::stack<SE>& l_steps){
        m_steps = l_steps;
    }


    bool MoreSteps(){
        return m_steps.size();
    }

    bool flag;
private:
    sf::Vector2f m_position;
    sf::Vector2f m_positionOld;

    sf::Vector2u m_tempTargetPosition;
    sf::Vector2f m_targetPosition;
    unsigned int m_elevation;

    std::stack<SE> m_steps;
    
};

#endif // C_POSITION_H

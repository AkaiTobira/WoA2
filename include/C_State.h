#ifndef C_STATE_H
#define C_STATE_H

enum class EntityState{ 
    Idle, 
    Walking, 
    Attacking, 
    Hurt, 
    Dying,
    
    END
};

class C_State : public C_Base{
public:
    C_State(): C_Base(Component::State){}
    
    void ReadIn(std::stringstream& l_stream){
        unsigned int state = 0;
        l_stream >> state;

        switch(state){
            case 0 :
                m_state = EntityState::Idle;
            break;
            case 1 :
                m_state = EntityState::Walking;
            break;
            case 4 :
                m_state = EntityState::Dying;
            break;
            default:
                m_state = EntityState::Idle;
            break;
        }
    }

    EntityState GetState(){ return m_state; }
    
    void SetState(const EntityState& l_state){
        m_state = l_state;
    }

private:
    EntityState m_state;
};

#endif
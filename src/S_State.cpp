#include "S_State.h"

S_State::S_State(SystemManager* l_systemMgr)
: S_Base(System::State, l_systemMgr) {
    Bitmask req;
    req.TurnOnBit((unsigned int)Component::State);
    m_requiredComponents.push_back(req);
    m_systemManager->GetMessageHandler()->Subscribe(EntityMessage::Move,this);
    m_systemManager->GetMessageHandler()->Subscribe(EntityMessage::Switch_State,this);
    m_systemManager->GetMessageHandler()->Subscribe(EntityMessage::Is_Idling,this);
 //   m_systemManager->GetMessageHandler()->Subscribe(EntityMessage::Spawned, this);
}


void S_State::Update(float l_dT __attribute((unused))){

    EntityManager* entities = m_systemManager->GetEntityManager();
    for(auto &entity : m_entities){
        C_State* state = entities->GetComponent<C_State>(entity, Component::State);
        if(state->GetState() == EntityState::Walking){
            Message msg((MessageType)EntityMessage::Is_Moving);
            msg.m_receivers.insert(entity);
     //       m_systemManager->GetMessageHandler()->Dispatch(msg);
        }
        /*
        else if( state->GetState() == EntityState::Idle ){
            Message msg((MessageType)EntityMessage::Waiting);
            msg.m_receivers.push_back(entity);
            m_systemManager->GetMessageHandler()->Dispatch(msg);
        }
        */
    }

}



void S_State::Notify(const Message& l_message){

    std::set<unsigned int> movRight;
    std::set<unsigned int> movLeft;
    std::set<unsigned int> movUp;
    std::set<unsigned int> movDown;
    std::set<unsigned int> moving;
   // EntityManager* entities = m_systemManager->GetEntityManager();
    for( auto &reciver : l_message.m_receivers ){

        if (!HasEntity(reciver)){ return; }
        
        EntityMessage m = (EntityMessage)l_message.m_type;
        switch(m){
            case EntityMessage::Move:
                {
           //     std::cout << "ENTITY MOVE :: " << reciver << std::endl; 
                C_State* state = m_systemManager->GetEntityManager()->GetComponent<C_State>(reciver,Component::State);
                if (state->GetState() == EntityState::Dying){ continue; }
                EntityEvent e;
                if (l_message.m_int == (int)Direction::Up){
                    e = EntityEvent::Moving_Up;
       //             movUp.push_back(reciver);

                } else if (l_message.m_int == (int)Direction::Down){
                    e = EntityEvent::Moving_Down;
       //             movDown.push_back(reciver);

                } else if(l_message.m_int == (int)Direction::Left){
                    e = EntityEvent::Moving_Left;
      //              movLeft.push_back(reciver);
     //               moving.push_back(reciver);
                } else if (l_message.m_int == (int)Direction::Right){
                    e = EntityEvent::Moving_Right;
     //               movRight.push_back(reciver);
      //              moving.push_back(reciver);
                }
                
                moving.insert(reciver);
                m_systemManager->AddEvent(reciver, (EventID)e);
                
        }
        break;
        case EntityMessage::Switch_State:

            ChangeState(reciver,(EntityState)l_message.m_int,false);
        break;
        case EntityMessage::Is_Idling:

            KeepState(reciver,(EntityState)l_message.m_int);

        break;

   //     case EntityMessage::Spawned:
            // Narazie mysl ....

     //   break;
        default:
        break;

    }
    }

    if( moving.size() ){
       ChangeState(moving,EntityState::Walking,false);
       moving.clear();
    }

}



void S_State::KeepState(const EntityId& l_entity,const EntityState& l_state ){

    EntityManager* entities = m_systemManager->GetEntityManager();
    C_State* state = entities->GetComponent<C_State>(l_entity, Component::State);
    if ( state->GetState() == EntityState::Dying){ return;}
    state->SetState(l_state);

}


S_State::~S_State(){
    
}



void S_State::HandleEvent(
    const std::set<unsigned int>& participians,
    const EntityEvent& l_event
){



    switch(l_event){
        case EntityEvent::Became_Idle:
            ChangeState(participians,EntityState::Idle,false );
        break;
        case EntityEvent::Spawned:
            for( auto reciver : participians )
                m_systemManager->AddEvent(reciver, (EventID)EntityEvent::Became_Idle );
        break;
        default:
        break;
    }

}


/*
void S_State::ChangeState(const std::set<unsigned int>& reciver,
    const EntityState& l_state)
{

    Message msg((MessageType)EntityMessage::State_Changed);
    msg.m_receivers = reciver;
    msg.m_int = (int)l_state;
    
    m_systemManager->GetMessageHandler()->Dispatch(msg);
}*/

void S_State::ChangeState(
    const std::set<unsigned int>& participians,
    const EntityState& l_state, 
    const bool& l_force
){

    EntityManager* entities = m_systemManager->GetEntityManager();

    std::set<unsigned int> reciver;

    for( auto l_entity : participians){

    C_State* state = entities->GetComponent<C_State>(l_entity, Component::State);
    if (!l_force && state->GetState() == EntityState::Dying){ continue;}
        state->SetState(l_state);
        reciver.insert(l_entity);
    
    }

    Message msg((MessageType)EntityMessage::State_Changed);
    msg.m_receivers = reciver;
    msg.m_int = (int)l_state;
    m_systemManager->GetMessageHandler()->Dispatch(msg);
    
}


////////////////////////////////////////////////////////////////////////////////


void S_State::HandleEvent(const EntityId& l_entity,const EntityEvent& l_event)
{


switch(l_event){
    case EntityEvent::Became_Idle:
    std ::cout << "EEE?" << std::endl;
    ChangeState(l_entity,EntityState::Idle,false);
    break;
    default:
    break;
}
}


void S_State::ChangeState(const EntityId& l_entity,const EntityState& l_state, const bool& l_force)
{
    EntityManager* entities = m_systemManager->GetEntityManager();
    C_State* state = entities->GetComponent<C_State>(l_entity, Component::State);
    if (!l_force && state->GetState() == EntityState::Dying){ return;}
    state->SetState(l_state);

  //  Message msg((MessageType)EntityMessage::State_Changed);
  //  msg.m_receivers.push_back(l_entity);
  //  msg.m_int = (int)l_state;
    
  //  m_systemManager->GetMessageHandler()->Dispatch(msg);
}
#include "S_Control.h"

S_Control::S_Control(SystemManager* l_systemMgr)
:S_Base(System::Control,l_systemMgr)
{
    Bitmask req;
    req.TurnOnBit((unsigned int)Component::Position);
    req.TurnOnBit((unsigned int)Component::Movable);
    req.TurnOnBit((unsigned int)Component::Controller);
    m_requiredComponents.push_back(req);
    req.Clear();
}



void S_Control::MoveEntity(const EntityId& l_entity,const Direction& l_dir)
    {
    C_Movable* mov = m_systemManager->GetEntityManager()->
    GetComponent<C_Movable>(l_entity, Component::Movable);
    mov->Move(l_dir);
}


void S_Control::Notify(const Message& l_message __attribute((unused))){

}

void S_Control::Update(float l_dt __attribute((unused))){
    
}

void S_Control::ActivateUnit(const EntityId& l_entity){
    C_Controller* contr = m_systemManager->GetEntityManager()->
    GetComponent< C_Controller>(l_entity, Component::Controller);

    contr->SetActive(true);
}

void S_Control::ReleaseUnit(const EntityId& l_entity){
    C_Controller* contr = m_systemManager->GetEntityManager()->
    GetComponent< C_Controller>(l_entity, Component::Controller);

    contr->SetActive(false);
}

bool S_Control::FindUnit( sf::Vector2f & l_x, sf::Vector2f & l_y ){
    int cout = 0;




    float low_x = 0;
    float low_y = 0;

    float hig_x = 0;
    float hig_y = 0;


    if( l_x.x < l_y.x ){
        low_x = l_x.x;
        hig_x = l_y.x;
    }else{
        low_x = l_y.x;
        hig_x = l_x.x;
    }

    
    if( l_x.y < l_y.y ){
        low_y = l_x.y;
        hig_y = l_y.y;
    }else{
        low_y = l_y.y;
        hig_y = l_x.y;
    }




    for(auto &entity : m_entities){
        C_Controller* contr = m_systemManager->GetEntityManager()->
        GetComponent< C_Controller>(entity, Component::Controller);
        if( contr->GetControllingPerson() != ControllingPerson::Player){
            continue;
        }
        C_Position* position = m_systemManager->GetEntityManager()->
        GetComponent<C_Position>(entity,Component::Position);
        
        auto pos = position->GetPosition();
    
        C_SpriteSheet* sp = m_systemManager->GetEntityManager()->
        GetComponent<C_SpriteSheet>(entity,Component::SpriteSheet);
        
        auto size = sp->GetSize();

    
        if( (pos.x + size.x/2 >= low_x  and pos.x - size.x/2 <= hig_x)
            and
            (pos.y + size.y/2 >= low_y  and pos.y - size.y/2 <= hig_y) 
        ){
            cout++;
            #ifdef POSDEBUG
    
             std::cout << entity << " SIZE :: " << size.x << " " << size.y << std::endl;
        
            std::cout << entity << " POS :: " << pos.x << " " <<  pos.y << std::endl;
            std::cout << entity << " CLICK :: " << l_x << " " << l_y << std::endl;
            
            std::cout << entity << " Targets :: " 
            << pos.x + size.x/2 << " " << pos.x - size.x/2 << " "
            << pos.y + size.y/2 << " " << pos.y - size.y/2 << std::endl;
            
            std::cout << entity << " Targets2 :: " 
            << (pos.x + size.x/2 >=l_x) << " " << (pos.x - size.x/2<=l_x) << " "
            << (pos.y + size.y/2 >=l_y)<< " " << (pos.y - size.y/2<=l_y) << std::endl;
            
            std::cout << entity << " IN RANGE" << std::endl;
            #endif
            C_State* state = m_systemManager->GetEntityManager()->GetComponent<C_State>(entity, Component::State);
            
            if( state->GetState() == EntityState::Idle){
              //  std::cout << "ENTITY :" << entity << " STATE  :: IDLE" << std::endl;
                
            }else if(state->GetState() == EntityState::Walking){
            //    std::cout << "ENTITY :" << entity << " STATE  :: WALK" << std::endl;
                
            }
            contr->SetActive(true);
        }
    
        }
    
        return (bool)cout;

}

bool S_Control::FindUnit( float l_x, float l_y ){
    int cout = 0;

    for(auto &entity : m_entities){
    C_Controller* contr = m_systemManager->GetEntityManager()->
    GetComponent< C_Controller>(entity, Component::Controller);
    if( contr->GetControllingPerson() != ControllingPerson::Player){
        continue;
    }
    C_Position* position = m_systemManager->GetEntityManager()->
    GetComponent<C_Position>(entity,Component::Position);
    
    auto pos = position->GetPosition();

    C_SpriteSheet* sp = m_systemManager->GetEntityManager()->
    GetComponent<C_SpriteSheet>(entity,Component::SpriteSheet);
    
    auto size = sp->GetSize();


    if( (pos.x + size.x/2 >= l_x  and pos.x - size.x/2 <= l_x)
        and
        (pos.y + size.y/2 >= l_y  and pos.y - size.y/2 <= l_y) 
    ){
        cout++;
        #ifdef POSDEBUG

         std::cout << entity << " SIZE :: " << size.x << " " << size.y << std::endl;
    
        std::cout << entity << " POS :: " << pos.x << " " <<  pos.y << std::endl;
        std::cout << entity << " CLICK :: " << l_x << " " << l_y << std::endl;
        
        std::cout << entity << " Targets :: " 
        << pos.x + size.x/2 << " " << pos.x - size.x/2 << " "
        << pos.y + size.y/2 << " " << pos.y - size.y/2 << std::endl;
        
        std::cout << entity << " Targets2 :: " 
        << (pos.x + size.x/2 >=l_x) << " " << (pos.x - size.x/2<=l_x) << " "
        << (pos.y + size.y/2 >=l_y)<< " " << (pos.y - size.y/2<=l_y) << std::endl;
        
        std::cout << entity << " IN RANGE" << std::endl;
        #endif
       
        contr->SetActive(true);
    }

    }

    return (bool)cout;

}

void S_Control::ReleaseUnits( ){

    for(auto &entity : m_entities){
        C_Controller* contr = m_systemManager->GetEntityManager()->
        GetComponent< C_Controller>(entity, Component::Controller);
        if( contr->GetControllingPerson() != ControllingPerson::Player){
            continue;
        }
        contr->SetActive(false);
    }
}


void S_Control::HandleEvent(const EntityId& l_entity,const EntityEvent& l_event)
{ 

    //OBSTAWIAM ZE TO BEDE EDYTOWAC
switch(l_event){
case EntityEvent::Moving_Left:
    MoveEntity(l_entity,Direction::Left); 
break;
case EntityEvent::Moving_Right:
    MoveEntity(l_entity, Direction::Right); 
break;
case EntityEvent::Moving_Up:
    MoveEntity(l_entity, Direction::Up); 
break;
case EntityEvent::Moving_Down:
    MoveEntity(l_entity, Direction::Down); 
break;
default:
break;
}
}

void S_Control::HandleEvent(
    const std::set<unsigned int>& participians,
    const EntityEvent& l_events
){
        switch(l_events){
        case EntityEvent::Moving_Left:
            for( auto l_entity : participians ){
                MoveEntity(l_entity, Direction::Left);
        //        std::cout << "GET LEFT " << std::endl;
            }
        break;
        case EntityEvent::Moving_Right:
            for( auto l_entity : participians )
                MoveEntity(l_entity, Direction::Right);
        break;
        case EntityEvent::Moving_Down:
            for( auto l_entity : participians )
                MoveEntity(l_entity, Direction::Down);
        break;
        case EntityEvent::Moving_Up:
            for( auto l_entity : participians )
                MoveEntity(l_entity, Direction::Up);
        break;
        default:
        break;        
        }
}
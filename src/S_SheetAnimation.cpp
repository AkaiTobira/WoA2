
#include "S_SheetAnimation.h"

S_SheetAnimation::S_SheetAnimation(SystemManager* l_systemMgr)
: S_Base(System::SheetAnimation,l_systemMgr)
{
    Bitmask req;
    req.TurnOnBit((unsigned int)Component::SpriteSheet);
    req.TurnOnBit((unsigned int)Component::State);
    m_requiredComponents.push_back(req);
    m_systemManager->GetMessageHandler()->Subscribe(EntityMessage::State_Changed,this);

}

void S_SheetAnimation::Update(float l_dT){
    EntityManager* entities = m_systemManager->GetEntityManager();
  //  std::cout << "SHEETUPDATE" << std::endl;
    for(auto &entity : m_entities){
    C_SpriteSheet* sheet = entities->GetComponent<C_SpriteSheet>(entity, Component::SpriteSheet);
   // C_State* state = entities->GetComponent<C_State>(entity, Component::State);
    sheet->GetSpriteSheet()->Update(l_dT);

 //   const std::string& animName = sheet-> GetSpriteSheet()->GetCurrentAnim()->GetName();
 //   std::cout << "ENTITY :: " << entity << ":: is :: " << animName << std::endl;

/*
 if(animName == "Attack"){
        if(!sheet->GetSpriteSheet()->GetCurrentAnim()->IsInAction()){
       //     std::cout << "ENTITY :: " << entity << "IDLING" << std::endl; 
            Message msg((MessageType)EntityMessage::Switch_State);
            msg.m_receiver = entity;
            msg.m_int = (int)EntityState::Idle;

      //      m_systemManager->GetMessageHandler()->Dispatch(msg);
        } else if(sheet->GetSpriteSheet()->GetCurrentAnim()->IsInAction()){
            Message msg((MessageType)EntityMessage::Attack_Action);
            msg.m_sender = entity;
            m_systemManager->GetMessageHandler()->Dispatch(msg);
        }
    } else if(
        animName == "Death" &&
        ! sheet->GetSpriteSheet()->GetCurrentAnim()->IsInAction()){
     //       std::cout << "ENTITY :: " << entity << "DIED" << std::endl; 
            Message msg((MessageType)EntityMessage::Dead);
            msg.m_receiver = entity;
            m_systemManager->GetMessageHandler()->Dispatch(msg);
        }
    
*/
    }
}


void S_SheetAnimation::Notify(const Message& l_message){


    std::set<unsigned int> recivers[(int)EntityState::END];
   // std::cout << "S_SHEETANIMATION :: NOTIFY" << std::endl; 

    for( auto reciver : l_message.m_receivers){

        if(HasEntity(reciver)){
        //  std::cout << "LOL WORK" << std::endl;
            EntityMessage m = (EntityMessage)l_message.m_type;
            switch(m){
                case EntityMessage::State_Changed:
                    {
                    EntityState s = (EntityState)l_message.m_int;
                    switch(s){
                        case EntityState::Idle:
                            recivers[(int)EntityState::Idle].insert(reciver);
                            

                        //    ChangeAnimation(reciver,"Idle",true,true);
                        break;
                        case EntityState::Walking:
                    //      std::cout << "WALK" << std::endl;

                            recivers[(int)EntityState::Walking].insert(reciver);
                    
  
                   //         ChangeAnimation(reciver,"Walk",true,true);
        
                        break;
                        case EntityState::Attacking:
                    //        std::cout << "ATAK" << std::endl;
                     
                            recivers[(int)EntityState::Attacking].insert(reciver);
                    
                           // ChangeAnimation(reciver,"Attack",true,false);
                        break;
                        case EntityState::Hurt: 
                            recivers[(int)EntityState::Hurt].insert(reciver);
                        
                        break;
                        case EntityState::Dying:
                            recivers[(int)EntityState::Dying].insert(reciver);
                        
                //          std::cout << "DIE" << std::endl;
                           // ChangeAnimation(reciver,"Death",true,false);
                        break;
                        }

                    
                    }

                //    recivers.insert(reciver);

                break;
                default:
                break;
                }


                if( recivers[(int)EntityState::Idle].size()      ) ChangeAnimation(recivers[(int)EntityState::Idle],"Idle", true, true);
                if( recivers[(int)EntityState::Walking].size()   ) ChangeAnimation(recivers[(int)EntityState::Walking],"Walk", true, true);
                if( recivers[(int)EntityState::Attacking].size() ) ChangeAnimation(recivers[(int)EntityState::Attacking],"Atack",true,false);
                if( recivers[(int)EntityState::Hurt].size()      );// ChangeAnimation(recivers[(int)EntityState::Hurt],"Hurt",true,false);
                if( recivers[(int)EntityState::Dying].size()     ) ChangeAnimation(recivers[(int)EntityState::Dying],"Death",true,false);


                /*
                for( auto reciver : recivers ){

                Message msg((MessageType)EntityMessage::Is_Idling);
                msg.m_receivers = recivers;
                msg.m_int = (int)EntityState::Walking;
                
                m_systemManager->GetMessageHandler()->Dispatch(msg);

                //INFORMACJE WIĄDĄ TUTAJ -> Powinno nadawać że Czeka na zmianę następną.
                }
                */
        }

    }
}

void S_SheetAnimation::ChangeAnimation(
    const std::set<unsigned int>& l_entities,
    const std::string& l_anim,
    bool l_play,
    bool l_loop
){


    for( auto l_entity : l_entities ){

        C_SpriteSheet* sheet = m_systemManager->GetEntityManager()->GetComponent<C_SpriteSheet>(l_entity,Component::SpriteSheet);
        if(
            sheet-> GetSpriteSheet()->GetCurrentAnim()->GetName() 
            != 
            l_anim
        ) { 
        sheet->GetSpriteSheet()->SetAnimation(l_anim,l_play,l_loop);
        }
    }




}



void S_SheetAnimation::ChangeAnimation(
    const EntityId& l_entity,
    const std::string& l_anim,
    bool l_play,
    bool l_loop
){
    clock_t t; clock_t t1;
    t = clock();
    C_SpriteSheet* sheet = m_systemManager->GetEntityManager()->GetComponent<C_SpriteSheet>(l_entity,Component::SpriteSheet);
    if(
        sheet-> GetSpriteSheet()->GetCurrentAnim()->GetName() 
        != 
        l_anim
    ) { sheet->GetSpriteSheet()->SetAnimation(l_anim,l_play,l_loop);

  //  sheet->


    t1 = clock();
    std::cout << std::fixed << std::setprecision(7) <<  static_cast<float>( static_cast<float>(t1-t) /CLOCKS_PER_SEC ) << std::endl;
    
}

}
S_SheetAnimation::~S_SheetAnimation(){}

void S_SheetAnimation::HandleEvent(
    const std::set<unsigned int>& participians __attribute((unused)),
    const EntityEvent& l_events __attribute((unused))
){


}


void S_SheetAnimation::HandleEvent(const EntityId& l_entity __attribute((unused)),const EntityEvent& l_event __attribute((unused)))
{ 

}

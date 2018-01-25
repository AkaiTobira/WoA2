#include "S_Movement.h"

S_Movement::S_Movement(SystemManager* l_systemMgr)
: S_Base(System::Movement,l_systemMgr)
{
    #ifdef DEBUGG_RUN
        std::cout << "S_MOVEMENT :: START :: BEGIN" << std::endl;
    #endif

    Bitmask req;
    req.TurnOnBit((unsigned int)Component::Position);
    req.TurnOnBit((unsigned int)Component::Movable);
    m_requiredComponents.push_back(req);
    req.Clear();
    m_systemManager->GetMessageHandler()->Subscribe(EntityMessage::Is_Moving,this);
    m_gameMap = nullptr;

    #ifdef DEBUGG_RUN
        std::cout << "S_MOVEMENT :: START :: DONE" << std::endl;
    #endif
}

bool S_Movement::RemoveEntity(const EntityId& l_entity){

    StopEntity(l_entity);
    EntityManager* entities = m_systemManager->GetEntityManager();
    C_Position* position = entities->GetComponent<C_Position>(l_entity, Component::Position);
    m_gameMap->UnBlockTile( position->GetBlockTile().x, position->GetBlockTile().y);
    m_gameMap->UnBlockTile( position->GetBlockTileOld().x, position->GetBlockTileOld().y);


    C_Movable* movable = entities->GetComponent<C_Movable>(l_entity, Component::Movable);


    toRelease.push_back(movable->GetCurrentReservation());
    toRelease.push_back(movable->GetPrevReservation());
    toRelease.push_back(position->GetUPosition() );
    toRelease.push_back(position->GetBlockTileOld() );


    for( auto it = m_entities.begin(); it != m_entities.end(); it++){
        if( l_entity == *it ){
            m_entities.erase(it);
            return true;
        }
    }
    return false;

}

S_Movement::~S_Movement(){
    #ifdef DEBUGG_RUN
    std::cout << "S_MOVEMENT :: DESTRUCTOR" << std::endl;
    #endif


    for( unsigned int i = 0; i<m_mapSize.x; i++ ){
        delete [] m_ReservedTiles[i];
    }
    delete  m_ReservedTiles;

    m_gameMap = nullptr;

    #ifdef DEBUGG_RUN
    std::cout << "S_MOVEMENT :: END" << std::endl;
    #endif
}


void S_Movement::ReserveTile(C_Position* position, C_Movable* movable){
    sf::Vector2u pos     = position->GetUPosition();

    sf::Vector2u Nextpos = { 
      0,
      0
    };

    const float MINUS = 0.001;

    switch(movable->GetDirection()){
        case Direction::Left:
            pos     = { 
                static_cast<unsigned int>((position->GetFPosition().x - Tile_Size/2)/Tile_Size), 
                static_cast<unsigned int>((position->GetFPosition().y)/Tile_Size)
            };

            Nextpos = { 
                static_cast<unsigned int>((position->GetFPosition().x - Tile_Size/2 + movable->GetVelocity().x)/Tile_Size), 
                static_cast<unsigned int>((position->GetFPosition().y + movable->GetVelocity().y)/Tile_Size)
            };

        break;
        case Direction::Right:
            pos     = { 
                static_cast<unsigned int>((position->GetFPosition().x -MINUS  + Tile_Size/2)/Tile_Size), 
                static_cast<unsigned int>((position->GetFPosition().y)/Tile_Size)
            };


            Nextpos = { 
                static_cast<unsigned int>((position->GetFPosition().x -MINUS + Tile_Size/2 + movable->GetVelocity().x)/Tile_Size), 
                static_cast<unsigned int>((position->GetFPosition().y + movable->GetVelocity().y)/Tile_Size)
            };

        break;
        case Direction::LeftDown:
            pos     = { 
                static_cast<unsigned int>((position->GetFPosition().x - Tile_Size/2)/Tile_Size), 
                static_cast<unsigned int>((position->GetFPosition().y + Tile_Size/2)/Tile_Size)
            };

            Nextpos = { 
                 static_cast<unsigned int>((position->GetFPosition().x - Tile_Size/2 + movable->GetVelocity().x)/Tile_Size), 
                 static_cast<unsigned int>((position->GetFPosition().y + Tile_Size/2 + movable->GetVelocity().y)/Tile_Size)
            };
        break;
        case Direction::LeftUp:
            pos     = { 
                static_cast<unsigned int>((position->GetFPosition().x - Tile_Size/2)/Tile_Size), 
                static_cast<unsigned int>((position->GetFPosition().y - Tile_Size/2)/Tile_Size)
            };

            Nextpos = { 
                static_cast<unsigned int>((position->GetFPosition().x - Tile_Size/2 + movable->GetVelocity().x)/Tile_Size), 
                static_cast<unsigned int>((position->GetFPosition().y - Tile_Size/2 + movable->GetVelocity().y)/Tile_Size)
            };
        break;
        case Direction::RightDown:
            pos     = { 
                static_cast<unsigned int>((position->GetFPosition().x -MINUS  + Tile_Size/2 )/Tile_Size), 
                static_cast<unsigned int>((position->GetFPosition().y -MINUS  + Tile_Size/2 )/Tile_Size)
            };

            Nextpos = { 
                static_cast<unsigned int>((position->GetFPosition().x -MINUS  + Tile_Size/2 + movable->GetVelocity().x)/Tile_Size), 
                static_cast<unsigned int>((position->GetFPosition().y -MINUS  + Tile_Size/2 + movable->GetVelocity().y)/Tile_Size)
            };
        break;
        case Direction::RightUp:
            pos     = { 
                static_cast<unsigned int>((position->GetFPosition().x -MINUS + Tile_Size/2)/Tile_Size), 
                static_cast<unsigned int>((position->GetFPosition().y - Tile_Size/2)/Tile_Size)
            };

            Nextpos = { 
                static_cast<unsigned int>((position->GetFPosition().x -MINUS + Tile_Size/2 + movable->GetVelocity().x)/Tile_Size), 
                static_cast<unsigned int>((position->GetFPosition().y - Tile_Size/2 + movable->GetVelocity().y)/Tile_Size)
            };
        break;
        case Direction::Up:
            pos     = { 
                static_cast<unsigned int>((position->GetFPosition().x)/Tile_Size),
                static_cast<unsigned int>((position->GetFPosition().y - Tile_Size/2)/Tile_Size)
            };

            Nextpos = { 
                static_cast<unsigned int>((position->GetFPosition().x + movable->GetVelocity().x)/Tile_Size), 
                static_cast<unsigned int>((position->GetFPosition().y - Tile_Size/2 + movable->GetVelocity().y)/Tile_Size)
            };
        break;
        case Direction::Down:
            pos     = { 
                static_cast<unsigned int>((position->GetFPosition().x)/Tile_Size),
                static_cast<unsigned int>((position->GetFPosition().y -MINUS  + Tile_Size/2)/Tile_Size)
            };

            Nextpos = { 
                static_cast<unsigned int>((position->GetFPosition().x + movable->GetVelocity().x)/Tile_Size), 
                static_cast<unsigned int>((position->GetFPosition().y -MINUS  + Tile_Size/2 + movable->GetVelocity().y)/Tile_Size)
            };
        break;
        default:
            return;
        break;


    }

    if( pos.x == Nextpos.x and pos.y == Nextpos.y ) {
        return;
        }
 
    if( m_ReservedTiles[Nextpos.x][Nextpos.y] or !m_gameMap->IsWalkable(Nextpos.x, Nextpos.y) ) {
         position->Pause(); 
         return;
        }

    if( m_gameMap->IsWalkable(Nextpos.x, Nextpos.y)){

        movable->SetCurrentReservation( Nextpos );
        m_ReservedTiles[pos.x][pos.y] = false;
        m_ReservedTiles[Nextpos.x][Nextpos.y] = true;

        for( auto itr = toRelease.begin(); itr != toRelease.end();){
            if( Nextpos == *itr ){ 
                itr = toRelease.erase(itr);
            }else if ( pos == *itr){
                itr = toRelease.erase(itr);
            }else{
                ++itr;
            }
        }
    }
}

void S_Movement::Update(float l_dT){
   
 
    #ifdef DEBUGG_RUN
        std::cout << "S_MOVEMENT :: UPDATE :: BEGIN" << std::endl;
    #endif

    #ifdef DEBUGG_RUN
        std::cout << "S_MOVEMENT :: UPDATE" << std::endl;
    #endif

    if (!m_gameMap){ return; }
    
     for(unsigned int i = 0; i < toRelease.size(); i++){
         m_ReservedTiles[toRelease[i].x][toRelease[i].y] = false;
     }

     toRelease.clear();

    EntityManager* entities = m_systemManager->GetEntityManager();

    std::set<unsigned int> m_walking[(int)Direction::ENDMARK];

    for(auto &entity : m_entities){
        
        C_Position* position = entities->GetComponent<C_Position>(entity, Component::Position);


        
        C_Movable* movable = entities->GetComponent<C_Movable>(entity, Component::Movable);
        
        #ifdef SDR_RUN
            std::cout << "Entity " << entity << " :: Position :: "<< position->GetFPosition().x<< " "<< position->GetFPosition().y << std::endl;
        #endif


        #ifdef SDR_RUN
            std::cout << "Entity " << entity << " :: MoveBy :: "<< movable->GetVelocity().x<< " "<< movable->GetVelocity().y << std::endl;
        #endif
        
        if( movable->isWalking() ){
           
            CheckDirection( position, movable, entity);  
            MovementStep  ( position, movable, l_dT  );
            ReserveTile   ( position, movable        );          

            if( position->IsPaused() ){ 
                position->Play() ; 
                continue;
            }

             Direction temp = movable->GetDirection();

            if(  temp <= Direction::None  or  temp >= Direction::ENDMARK ){ 
                continue; 
            }else{
                m_walking[(unsigned int)temp].insert(entity);
            }        

            position->MoveBy(movable->GetVelocity());   


            if( position->BlockChanged() ){
                m_gameMap->BlockTile  (    position->GetBlockTile().x,    position->GetBlockTile().y); 
                m_gameMap->UnBlockTile( position->GetBlockTileOld().x, position->GetBlockTileOld().y);
            }
        }




        #ifdef SDR_RUN
            std::cout << "Entity " << entity << " :: MoveBy :: "<< movable->GetVelocity().x<< " "<< movable->GetVelocity().y << std::endl;
        #endif
    }

    for( int i = 1; i < (int)Direction::ENDMARK ; i++ ){
        if( m_walking[i].size() )
            SendMessage( EntityMessage::Direction_Changed, i, m_walking[i]);
    }
  //  std::cout << "S_MOVEMENT :: UPDATE :: END" << std::endl;

  //  for( unsigned int i = 0; i < m_mapSize.x; i++){
  //      for( unsigned int j =0; j< m_mapSize.y; j++){
   //        std::cout << !m_gameMap->IsWalkable(j,i) or m_ReservedTiles[j][i];
    //       m_ReservedTiles[j][i] = false;
  //  }
  //      std::cout << std::endl;
  //  }
   // std::cout << std::endl;




}

void S_Movement::Notify(const Message& l_message){
        #ifdef DEBUGG_RUN
            std::cout << "S_MOVEMENT :: NOTIFY :: BEGIN" << std::endl;
        #endif
    
            EntityManager* eMgr = m_systemManager->GetEntityManager();
            EntityMessage m = (EntityMessage)l_message.m_type;
            switch(m){
                case EntityMessage::Is_Moving:
                for( auto &reciver : l_message.m_receivers ){
                    if (!HasEntity(reciver)){ continue; }
                    
                    C_Movable* movable = eMgr->GetComponent<C_Movable>(reciver, Component::Movable);
                    if (movable->isWalking()){ continue; }
                        m_systemManager->AddEvent(
                            reciver,
                            (EventID)EntityEvent::Became_Idle
                        );
                    }
                break; 
                default:
                break;
        }
        #ifdef DEBUGG_RUN
            std::cout << "S_MOVEMENT :: NOTIFY :: DONE" << std::endl;
        #endif
}

void S_Movement::HandleEvent(const std::set<unsigned int>& participians , const EntityEvent& l_event){
    #ifdef DEBUGG_RUN
        std::cout << "S_MOVEMENT :: HANDLEEVENT :: START" << std::endl;
    #endif

    C_Movable* movable;

    switch(l_event){
        case EntityEvent::Colliding_Left:
        {
            for( auto l_entity : participians ){
                movable = m_systemManager->GetEntityManager()->GetComponent<C_Movable>(l_entity,Component::Movable);
                switch(movable->GetDirection()){
                    case Direction::LeftUp:
                    StopEntity(l_entity,Axis::x); 
                        break;
                    case Direction::LeftDown:
                    StopEntity(l_entity,Axis::x); 
                        break;
                    case Direction::Left:
                    StopEntity(l_entity,Axis::x); 
                        break;
                    default:
                    break;
                }
            }
        }
                
            break;
        case EntityEvent::Colliding_Up:
        { 
            for( auto l_entity : participians ){    
                movable = m_systemManager->GetEntityManager()->GetComponent<C_Movable>(l_entity,Component::Movable);
                
                switch(movable->GetDirection()){
                    case Direction::LeftUp:
                    StopEntity(l_entity,Axis::y); 
                        break;
                    case Direction::Up:
                    StopEntity(l_entity,Axis::y); 
                        break;
                    case Direction::RightUp:
                    StopEntity(l_entity,Axis::y); 
                        break;
                        default:
                    break;
                }
            }
        }   
            break;

        case EntityEvent::Colliding_Down:
        { 
            for( auto l_entity : participians ){
                movable = m_systemManager->GetEntityManager()->GetComponent<C_Movable>(l_entity,Component::Movable);
                switch(movable->GetDirection()){
                    case Direction::Down:
                    StopEntity(l_entity,Axis::y); 
                        break;
                    case Direction::LeftDown:
                    StopEntity(l_entity,Axis::y); 
                        break;
                    case Direction::RightDown:
                    StopEntity(l_entity,Axis::y); 
                        break;
                        default:
                    break;
                } 
            }  
        }
            break;

        case EntityEvent::Colliding_Right:
        { 
        for( auto l_entity : participians ){
            movable = m_systemManager->GetEntityManager()->GetComponent<C_Movable>(l_entity,Component::Movable);
                    switch(movable->GetDirection()){
                case Direction::RightUp:
                StopEntity(l_entity,Axis::x); 
                    break;
                case Direction::RightDown:
            StopEntity(l_entity,Axis::x); 
                    break;
                case Direction::Right:
            StopEntity(l_entity,Axis::x); 
                    break;
                    default:
                break;
                }
            }
        }   
        break;

        case EntityEvent::Moving_Left:
            SetDirection(participians, Direction::Left); 
            break;

        case EntityEvent::Moving_Right:
            SetDirection(participians, Direction::Right); 
            break;

        case EntityEvent::Moving_LeftDown:
            SetDirection(participians, Direction::LeftDown); 
            break;

        case EntityEvent::Moving_LeftUp:
            SetDirection(participians, Direction::LeftUp); 
            break;
        case EntityEvent::Moving_RightDown:
            SetDirection(participians, Direction::RightDown); 
            break;
        case EntityEvent::Moving_RightUp:
            SetDirection(participians, Direction::RightUp); 
            break;

        case EntityEvent::Moving_Up:
        {
            std::set<unsigned int> temp;
            for( auto l_entity : participians ){
                C_Movable* mov = m_systemManager->GetEntityManager()->GetComponent<C_Movable>(l_entity,Component::Movable);
                if(mov->GetVelocity().x == 0){
                    temp.insert(l_entity);
                }
            }
            if( temp.size() ) SetDirection(temp, Direction::Up);            
        }
        break;
        case EntityEvent::Moving_Down:
        {
            std::set<unsigned int> temp;
            for( auto l_entity : participians ){
                C_Movable* mov = m_systemManager->GetEntityManager()->GetComponent<C_Movable>(l_entity,Component::Movable);
                if(mov->GetVelocity().x == 0){
                    temp.insert(l_entity);
                }
            }
            if( temp.size()) SetDirection(temp, Direction::Down);
        }
        break;


        case EntityEvent::ForceStop:
        {
        for( auto l_entity : participians ){
            movable = m_systemManager->GetEntityManager()->GetComponent<C_Movable>(l_entity,Component::Movable);
            StopEntity(l_entity);
            }
        }

        default:
        break;
    }

    #ifdef DEBUGG_RUN
        std::cout << "S_MOVEMENT :: HANDLEEVENT :: DONE" << std::endl;
    #endif
}

bool S_Movement::onPosition( const sf::Vector2u& l_where, const sf::Vector2f& l_isNow, const sf::Vector2f& offset){
    
    float XCenter =  l_where.x*Tile_Size + Tile_Size/2;
    float YCenter =  l_where.y*Tile_Size + Tile_Size/2;

    if( ! ( (l_isNow.x >= ( XCenter - fabs(offset.x) ) ) and l_isNow.x <= ((XCenter + fabs(offset.x)) )) ) {
        
        return false;
    }

    if( ! ( l_isNow.y >= ( YCenter - fabs(offset.y) ) and l_isNow.y <= (YCenter + fabs(offset.y)) )) {
        return false;
    }
    
    return true;
}



void S_Movement::CheckDirection(C_Position* position, C_Movable* movable, const EntityId& entity){
    
    #ifdef MOVABLE_DEBBUG
        sf::Vector2f target   = movable->GetFTargetPosition();
        sf::Vector2u nextStep = movable->GetCheckpoint();
    #endif

    if( onPosition( movable->GetCheckpoint() , position->GetFPosition() , movable->GetVelocity() )){


       #ifdef MOVABLE_DEBBUG
            std::cout << " I REACH POSITION :: " << nextStep.x << " | " << nextStep.y << std::endl;
            std::cout << "CURRENT_VCTOR :: " << now.x << " " << now.y << std::endl;
            std::cout << "TARGET_VECTOR :: " << (int)target.x/32 << " " << (int)target.y/32 << std::endl;
            std::cout << " Current Direction :: " << (int)movable->GetDirection() << std::endl;
            std::cout << "Stack size  ::" << position->SetSize() << std::endl;
            std::cout << " Condition :: " << onPosition( t2 , now,40) << " " << (position->SetSize() == 1)  << " "  << (onPosition( t2 , now,40) and position->SetSize() == 1) << std::endl;
        #endif

        if( onPosition( movable->GetUTargetPosition(), position->GetFPosition(), movable->GetVelocity()) and movable->GetStackSize()){

            toRelease.push_back(movable->GetCurrentReservation());
            toRelease.push_back(movable->GetPrevReservation());
            toRelease.push_back(position->GetUPosition() );
            toRelease.push_back(position->GetBlockTileOld() );

        //    m_ReservedTiles[movable->GetCurrentReservation().x][movable->GetCurrentReservation().y] = false;
        //    m_ReservedTiles[movable->GetPrevReservation().x]   [movable->GetPrevReservation().y   ] = false;
            StopEntity(entity);
            movable->GetNextStep();

            #ifdef MOVABLE_DEBBUG
                std::cout << "Shoudl Stop here " << std::endl;
            #endif

        }else{

            sf::Vector2f centerOfTargetTile = { 
                static_cast<float>(movable->GetCheckpoint().x*Tile_Size + Tile_Size/2),
                static_cast<float>(movable->GetCheckpoint().y*Tile_Size + Tile_Size/2)
                };
            position->SetPosition(centerOfTargetTile);

            Direction dir = movable->GetNextStep();

            #ifdef MOVABLE_DEBBUG
                std::cout << " Next Navi-Point :: " << movable->GetCheckpoint().x << " | " << movable->GetCheckpoint().y << std::endl;
                std::cout << "Change Direction to : " << (int)dir << std::endl;
                std::cout << "Current Next Direction :: " << movable->GetCheckpoint().x << " | " << movable->GetCheckpoint().y << std::endl; 
            #endif

        //    m_ReservedTiles[movable->GetCurrentReservation().x][movable->GetCurrentReservation().y] = false;
        //    m_ReservedTiles[movable->GetPrevReservation().x]   [movable->GetPrevReservation().y   ] = false;
           
            toRelease.push_back(movable->GetCurrentReservation());
            toRelease.push_back(movable->GetPrevReservation());
            toRelease.push_back(position->GetUPosition() );
            toRelease.push_back(position->GetBlockTileOld() );

        //    movable->SetDirection(dir);
              movable->CalculateSpeed(dir);
            
        }
    }else{
            movable->CalculateSpeed();
        }
}

void S_Movement::MovementStep( C_Position* l_position, C_Movable* l_movable, float l_dT){

    //GETTING HOW LONG STEP IS
    sf::Vector2f f_coefficient =GetTileFriction(
        l_position->GetElevation(),
        l_position->GetUPosition().x  ? l_position->GetUPosition().x : 0 ,
        l_position->GetUPosition().y  ? l_position->GetUPosition().y : 0 
    );

    l_movable->SetModificator(
        (f_coefficient.x * l_movable->GetSpeed().x) *l_dT,
        (f_coefficient.y * l_movable->GetSpeed().y) *l_dT
    );
}

const sf::Vector2f& S_Movement::GetTileFriction(
    unsigned int l_elevation __attribute__((unused)) , unsigned int l_x, unsigned int l_y)
    {
  //   if( l_y >  100000 or l_x > 100000  ) { return m_gameMap->GetTerrianFriction( 0,0 ) ;}

        if( l_x > m_gameMap->GetMapSize().x  or l_y > m_gameMap->GetMapSize().y  ){

            return m_gameMap->GetTerrianFriction(0,0);
        }

    #ifdef DEBUGG_RUN
        std::cout << "S_MOVEMENT :: GETTLEFRICTION :: START" << std::endl;
    #endif
        return m_gameMap->GetTerrianFriction(l_x,l_y);
}

void S_Movement::StopEntity(const EntityId& l_entity){
    C_Movable* movable = m_systemManager->GetEntityManager()->GetComponent<C_Movable>(l_entity,Component::Movable);
    C_Position* position = m_systemManager->GetEntityManager()->GetComponent<C_Position>(l_entity,Component::Position);
    
    position->SetPosition((int)position->GetFPosition().x/Tile_Size*Tile_Size +Tile_Size/2,(int)position->GetFPosition().y/Tile_Size*Tile_Size+Tile_Size/2);
    movable ->SetTargetPosition( position->GetFPosition().x, position->GetFPosition().y);

    #ifdef SDR_RUN
        std::cout << "Entity " << l_entity << " :: STOP" << std::endl;
    #endif

//    m_ReservedTiles[(int)position->GetFPosition().x/Tile_Size][(int)position->GetFPosition().y/Tile_Size] = false;

    position->StopMoving();
    movable ->StopMove();
    movable ->SetDirection(Direction::None);
    movable ->CalculateSpeed();

}

void S_Movement::StopEntity(const EntityId& l_entity,const Axis& l_axis)
    {
    #ifdef DEBUGG_RUN
        std::cout << "S_MOVEMENT :: STOP :: BEGIN" << std::endl;
    #endif
    C_Position* position = m_systemManager->GetEntityManager()->GetComponent<C_Position>(l_entity,Component::Position);
    
    C_Movable* movable = m_systemManager->GetEntityManager()->GetComponent<C_Movable>(l_entity,Component::Movable);

    Direction dir = movable->GetDirection();

    if(l_axis == Axis::x){

        switch( dir ){
            case Direction::Left:
                movable->SetDirection(Direction::None);
            break;
            case Direction::LeftDown:
                movable->SetDirection(Direction::Down);
            break;
            case Direction::LeftUp:
                movable->SetDirection(Direction::Up);
            break;
            case Direction::Right:
                movable->SetDirection(Direction::None);
            break;
            case Direction::RightDown:
                movable->SetDirection(Direction::Down);
            break;
            case Direction::RightUp:
                movable->SetDirection(Direction::Up);
            break;
            default:
            break;
        }


        movable->SetVelocity(sf::Vector2f(0.f, movable->GetVelocity().y));
        movable->SetTargetPosition( position->GetFPosition().x, movable->GetFTargetPosition().y);
    } else if(l_axis == Axis::y){


        switch( dir ){
            case Direction::Up:
                movable->SetDirection(Direction::None);
            break;
            case Direction::LeftDown:
                movable->SetDirection(Direction::Left);
            break;
            case Direction::LeftUp:
                movable->SetDirection(Direction::Left);
            break;
            case Direction::Down:
                movable->SetDirection(Direction::None);
            break;
            case Direction::RightDown:
                movable->SetDirection(Direction::Right);
            break;
            case Direction::RightUp:
                movable->SetDirection(Direction::Right);
            break;

            default:
            break;

        }

        movable->SetVelocity(sf::Vector2f(movable->GetVelocity().x, 0.f));
        movable->SetTargetPosition( movable->GetFTargetPosition().x, position->GetFPosition().y); 
    }

    #ifdef DEBUGG_RUN
        std::cout << "S_MOVEMENT :: STOP :: DONE" << std::endl;
    #endif
}

void S_Movement::PauseMove(unsigned int entity){
    C_Position* position = m_systemManager->GetEntityManager()->GetComponent<C_Position>(entity,Component::Position);
    position->Pause();
}

void S_Movement::TestTargetPosition( ){

    std::set<unsigned int> m_walking[(int)Direction::ENDMARK];

    for(auto &entity : m_entities){
        C_Controller* contr = m_systemManager->GetEntityManager()->GetComponent<C_Controller>(entity,Component::Controller);
        if(! contr->IsActive()){ continue; }       
        C_Position* position = m_systemManager->GetEntityManager()->GetComponent<C_Position>(entity,Component::Position);
        C_Movable* movable = m_systemManager->GetEntityManager()->GetComponent<C_Movable>(entity,Component::Movable);
           
        StopEntity(entity);

        position->Moving();
        position->SetPosition( 1*Tile_Size, 1*Tile_Size );
        position->SetPosition( ((int)position->GetFPosition().x/32) * 32+16, ((int)position->GetFPosition().y/32) * 32+16 );
        std::stack<SE> road;
    
        road.push( SE( Direction::LeftUp    , { 1, 1 } ) );
        road.push( SE( Direction::Down      , { 8, 8 } ) );
        road.push( SE( Direction::RightUp   , { 8, 1 } ) );
        road.push( SE( Direction::Left      , { 1, 8 } ) );
        road.push( SE( Direction::RightDown , { 8, 8 } ) );
        road.push( SE( Direction::Up        , { 1, 1 } ) );
        road.push( SE( Direction::LeftDown  , { 1, 8 } ) );
        road.push( SE( Direction::Right     , { 8, 1 } ) );

        //position->SetStackSteps(AStar(entity));
        movable->SetStackSteps(road);

        Direction dir = movable->DirectionCheck();

        m_walking[(int)dir].insert(entity);

        movable->SetTargetPosition( 1.5*Tile_Size, 1.5*Tile_Size);
        
        #ifdef MOVABLE_DEBBUG

        std::cout << " COMPLETE PACK OF INFORMATIONS :: "<< std::endl << " TARGET VECTOR :: " << position->GetFTargetPosition().x << " | " <<  position->GetFTargetPosition().y<< std::endl <<
        " TEMP VECTOR  :: " << position->GetCheckpoint().x << " | " <<position->GetCheckpoint().y << std::endl <<
        " DIRECTION :: " << (int)dir 
        << std::endl;

        #endif
    }

    for( int i = 1; i < (int)Direction::ENDMARK ; i++ ){
        if( m_walking[i].size() )
            SendMessage( EntityMessage::Move, i, m_walking[i]);
    }

    #ifdef SDR_RUN
        std::cout << "Entity " << l_entity << " :: Postion Set to :: " << l_x << " " << l_y  << std::endl;
    #endif
}

void S_Movement::CallAStarAgain(unsigned int entity __attribute__((unused))){



  //  C_Movable* movable = m_systemManager->GetEntityManager()->GetComponent<C_Movable>(entity,Component::Movable);

   // movable->SetStackSteps(AStar(entity));

    //position->SetTargetPosition( l_x , l_y);



}

void S_Movement::SetTargetPosition( float l_x, float l_y){
    
    std::set<unsigned int> m_walking[(int)Direction::ENDMARK];

    // auto pos = position->GetFPosition();

    //C_Movable* movable;
    
    #pragma omp parallel for
    for(unsigned int i = 0; i < m_entities.size(); i++){
    //    std::cout << "teraz dziala watek nr: " << omp_get_thread_num() << "\n";
    auto &entity = m_entities[i];
    C_Controller* contr = m_systemManager->GetEntityManager()->GetComponent<C_Controller>(entity,Component::Controller);
    if(! contr->IsActive()){ continue; }       
    C_Position* position = m_systemManager->GetEntityManager()->GetComponent<C_Position>(entity,Component::Position);
    C_Movable* movable = m_systemManager->GetEntityManager()->GetComponent<C_Movable>(entity,Component::Movable);
    

    StopEntity(entity);
    m_ReservedTiles[movable->GetCurrentReservation().x][movable->GetCurrentReservation().y] = false;
    

    movable->SetTargetPosition( l_x, l_y      );
    movable->SetStackSteps(     AStar(entity) );

    Direction dir = movable->DirectionCheck();
    movable ->StartMove();
    position->Moving();
    position->Play();
    
    #pragma omp critical 
    {
        m_walking[(int)dir].insert(entity);
    }
   // position->SetTargetPosition( l_x , l_y);

    }


    for( int i = 1; i < (int)Direction::ENDMARK ; i++ ){
        if( m_walking[i].size() )
            SendMessage( EntityMessage::Move, i, m_walking[i]);
    }


    #ifdef SDR_RUN
    std::cout << "Entity " << l_entity << " :: Postion Set to :: " << l_x << " " << l_y  << std::endl;
    #endif
}

void S_Movement::SendMessage( const  EntityMessage& l_type, int information, std::set<unsigned int>& recivers ){

    Message msg((MessageType)l_type);
    msg.m_int = information; //(int)movable->GetDirection();
    msg.m_receivers = recivers;
    m_systemManager->GetMessageHandler()->Dispatch(msg);

}

void S_Movement::SetPosition(float l_x, float l_y, const EntityId& l_entity){
    C_Position* position = m_systemManager->GetEntityManager()->GetComponent<C_Position>(l_entity,Component::Position);
    C_Movable* movable = m_systemManager->GetEntityManager()->GetComponent<C_Movable>(l_entity,Component::Movable);
    position->SetPosition(l_x,l_y);
    movable->SetTargetPosition(l_x,l_y);
}

void S_Movement::SetMap(Map* l_gameMap){
    #ifdef DEBUGG_RUN
    std::cout << "S_MOVEMENT :: SETMAP :: BEGIN" << std::endl;
    #endif
    
    m_gameMap = l_gameMap; 

    m_mapSize = l_gameMap->GetMapSize();

    m_ReservedTiles = new bool*[m_mapSize.x];

    for( unsigned int i = 0; i < m_mapSize.x; i++){
        m_ReservedTiles[i] = new bool[m_mapSize.y];
        for( unsigned int j =0; j< m_mapSize.y; j++){
            m_ReservedTiles[i][j] = false;
        }
    }

    #ifdef DEBUGG_RUN
    std::cout << "S_MOVEMENT :: SETMAP :: DONE" << std::endl;
    #endif
}

std::stack<SE> S_Movement::AStar(unsigned int& l_entity){
    C_Position* position = m_systemManager->GetEntityManager()->GetComponent<C_Position>(l_entity,Component::Position);
    C_Movable* movable = m_systemManager->GetEntityManager()->GetComponent<C_Movable>(l_entity,Component::Movable);
    

    sf::Vector2u pos = { 
        ((unsigned int)position->GetFPosition().x)/Tile_Size,
        ((unsigned int)position->GetFPosition().y)/Tile_Size
    };

    sf::Vector2u targ = { 
        (unsigned int)movable->GetFTargetPosition().x/Tile_Size,
        (unsigned int)movable->GetFTargetPosition().y/Tile_Size
    };    

    while( !m_gameMap->IsWalkable(targ.x, targ.y )){

        if( pos == targ ) { break; }


        sf::Vector2u xRange = {
            ( targ.x > 0 ) ? targ.x-1 : targ.x,
            ( targ.x < m_mapSize.x-1 ) ? targ.x+1 :targ.x
         };

         sf::Vector2u yRange = {
            ( targ.y > 0 ) ? targ.y-1 : targ.y,
            ( targ.y < m_mapSize.y-1 ) ? targ.y+1 : targ.y
         };

         float best = 99999;
         for( unsigned int i = xRange.x; i <= xRange.y; i++ ){
            for( unsigned int j = yRange.x; j<= yRange.y; j++){
                float t1 = (float)i - (float)pos.x ;
                float t2 = (float)j - (float)pos.y ;
                
                float sq = sqrt((t1*t1) + (t2*t2));
                if( best > sq ){
                    targ.x = i;
                    targ.y = j;
                    best = sq; 

                } 
            }
        }
    }

   movable->SetTargetPosition( targ.x*Tile_Size + Tile_Size/2 , targ.y*Tile_Size + Tile_Size/2);

   targ = { 
        (unsigned int)movable->GetFTargetPosition().x/Tile_Size,
        (unsigned int)movable->GetFTargetPosition().y/Tile_Size
    };  

    
    using my_container_t = std::vector<QE>;

    auto my_comp = [](const QE& e1, const QE& e2){
        return e1.u1 > e2.u1;
    };

    std::priority_queue< 
        QE,
        my_container_t,
        decltype(my_comp)
    >Que(my_comp);

    //const int MAX_MAP_SIZE = 5000; 

    bool**checked      = new bool*[m_mapSize.x];
    bool**waitToCheck     = new bool*[m_mapSize.x];
    unsigned int **heuristicValueLowestInQueue = new unsigned int *[m_mapSize.x];
    unsigned int **heuristicValueLowestEver = new unsigned int *[m_mapSize.x];
    
    std::pair<unsigned int, unsigned int>** previousStep = new  std::pair<unsigned int, unsigned int>*[m_mapSize.x];

    unsigned int hm = 0;

    for( unsigned int i = 0; i < m_mapSize.x; i++ ){
             checked[i] = new bool[m_mapSize.y];
         waitToCheck[i] = new bool[m_mapSize.y];
      heuristicValueLowestInQueue[i] = new unsigned int[m_mapSize.y];
      heuristicValueLowestEver[i] = new unsigned int[m_mapSize.y];
        previousStep[i] = new  std::pair<unsigned int, unsigned int>[m_mapSize.y];
    }

    for( unsigned int i = 0; i < m_mapSize.x; i++){
        for( unsigned int j =0; j< m_mapSize.y; j++){
            checked[i][j] = false;
            waitToCheck[i][j] = false;

            heuristicValueLowestInQueue[i][j] = 0;
            heuristicValueLowestEver[i][j] = 0;
            //heuristicValue[i][j] = INT_MAX;
       }
    }

    Que.push(QE(0, pos.x, pos.y));


   // std::cout << Que.size() << std::endl;

   // heuristicValue[pos.x][pos.y] = 0; 
       waitToCheck[pos.x][pos.y] = true;
           
      previousStep[pos.x][pos.y] = std::pair<unsigned int, unsigned int>( pos.x,pos.y );

    bool forceEnd = false;


    while( Que.size() > 0 and !forceEnd ){

        hm++;
        
        QE current = Que.top();

        if(( current.u2 == targ.x) and ( current.u3 == targ.y)){
        //std::cout << hm << std::endl;

            for( unsigned int i = 0; i<m_mapSize.x; i++ ){
                delete [] checked[i];
                delete [] waitToCheck[i];
                delete [] heuristicValueLowestInQueue[i];
                delete [] heuristicValueLowestEver[i];

                // delete [] previousStep[i];
            }
            delete [] checked;
            delete [] waitToCheck;
            delete [] heuristicValueLowestInQueue;
            delete [] heuristicValueLowestEver;
            
            //delete [] previousStep;
            
        //    std::cout << hm <<" Found" << std::endl;

            return CalculateRoad(previousStep, pos, targ);

            
        }

        Que.pop();

        sf::Vector2u xRange = {
            ( current.u2 > 0 ) ? current.u2-1 : current.u2,
            ( current.u2 < m_mapSize.x-1 ) ? current.u2+1 : current.u2
         };

         sf::Vector2u yRange = {
            ( current.u3 > 0 ) ? current.u3-1 : current.u3,
            ( current.u3 < m_mapSize.y-1 ) ? current.u3+1 : current.u3
         };


         for( unsigned int i = xRange.x; i <= xRange.y; i++ )
            {for( unsigned int j = yRange.x; j<= yRange.y; j++){


                if(!m_gameMap->IsWalkable(i, j)) { continue; }

                float distanceX = std::abs(static_cast<int>(i - targ.x));
                float distanceY = std::abs(static_cast<int>(j - targ.y));

            //    distanceX = distanceX*distanceX;
            //    distanceY = distanceY*distanceY;

                float max = distanceX > distanceY ? distanceX*(1.0-GetTileFriction(0,i,j).x) : distanceY *(1.0-GetTileFriction(0,i,j).y); 

                float newHeVal = heuristicValueLowestEver[current.u2][current.u3] 
                + max*1000;//+ sqrt( distanceX + distanceY );

                if( j - current.u3 == 0 or i - current.u2 == 0  ){ newHeVal -=40.0*(1.0-GetTileFriction(0,i,j).x); }
                /*

                h = max{ abs(curent_cell.x - goal.x),abs(curent_cell.y - goal.y) };

                */


                if( waitToCheck[i][j] ){

                    if( heuristicValueLowestInQueue[i][j] > newHeVal ){
                         heuristicValueLowestInQueue[i][j] = newHeVal;
                        Que.push( QE(newHeVal, i, j));
                        waitToCheck[i][j] = true;
                        previousStep[i][j] = std::make_pair(current.u2,current.u3);
                    }

                }else{

                    if( ! checked[i][j] ){
                        heuristicValueLowestEver[i][j] = newHeVal;
                        
                        Que.push( QE(newHeVal, i, j));
                        waitToCheck[i][j] = true;
                        previousStep[i][j] = std::make_pair(current.u2,current.u3);
                    }else if( heuristicValueLowestEver[i][j] > newHeVal ){
                        heuristicValueLowestEver[i][j] = newHeVal;
                        Que.push( QE(newHeVal, i, j));
                        waitToCheck[i][j] = true;
                        previousStep[i][j] = std::make_pair(current.u2,current.u3);

                    }

                }


                //Jeśli wezeł z taką samo pozycją jak current jest w OPEN 
                // i ma mniejszą heureze niz current to go pomiń.

                //Jeśli wezeł z taką samą pozycją jak current jest w Closed
                // i ma mniejszy mniejszą heureze pomin go wprzeciwnym przypadku 
                // dodać do listy.

            }}

            checked[current.u2][current.u3] = true;


            if( hm >  m_mapSize.x * m_mapSize.y ) {
                
                forceEnd = true;

            }
    }


    for( unsigned int i = 0; i<m_mapSize.x; i++ ){
            delete [] checked[i];
            delete [] waitToCheck[i];
            delete [] heuristicValueLowestInQueue[i];
            delete [] heuristicValueLowestEver[i];
            delete [] previousStep[i];
        }
    delete [] checked;
    delete [] waitToCheck;
    delete [] heuristicValueLowestInQueue;
    delete [] heuristicValueLowestEver;
    delete [] previousStep;

    std::stack<SE> road = std::stack<SE>(); 

    road.push(SE( Direction::None, pos.x, pos.y ));

    return  road;
}

void S_Movement::SetDirection( const std::set<unsigned int>& participians, const Direction& l_dir){

    #ifdef DEBUGG_RUN
        std::cout << "S_MOVEMENT :: SETDIRECTION :: BEGIN" << std::endl;
    #endif

    Message msg((MessageType)EntityMessage::Direction_Changed);
    msg.m_int = (int)l_dir;

    for( auto l_entity : participians){
        C_Movable* movable = m_systemManager->GetEntityManager()->
        GetComponent<C_Movable>(l_entity,Component::Movable);
        movable->SetDirection(l_dir);
        msg.m_receivers.insert(l_entity);
    }

    m_systemManager->GetMessageHandler()->Dispatch(msg);

    #ifdef DEBUGG_RUN
        std::cout << "S_MOVEMENT :: SETDIRECTION :: DONE" << std::endl;
    #endif

}

Direction S_Movement::CalculateDirection( sf::Vector2u& in, std::pair<unsigned int, unsigned int>& out  ){
    int x_dist = in.x - out.first; 
    int y_dist = in.y - out.second;



    switch(x_dist){
            case 0:
            switch(y_dist){
                case 1:
                        return Direction::Down;
                break;
                case -1:
                        return Direction::Up;  
                break;
                default:
                break;
            }
            break;
            case 1:
            switch(y_dist){
                case 0:
                    return Direction::Right;
                break;
                case 1:
                    return Direction::RightDown;
                break;
                case -1:
                    return Direction::RightUp;
                break;
                default:
                break;
            }
            break;
            case -1:
            switch(y_dist){
                case 0:
                    return Direction::Left;   
                break;
                case 1:
                    return Direction::LeftDown;
                break;
                case -1:
                    return Direction::LeftUp;
                break;
                default:
                break;
            }
            break;
        default:
        break;
    }

    return Direction::Up;

}

std::stack<SE> S_Movement::CalculateRoad(std::pair<unsigned int, unsigned int>** previous, sf::Vector2u& start, sf::Vector2u& target){
    std::stack<SE> road;
    
    sf::Vector2u cor = {target.x, target.y};

    Direction dir = CalculateDirection( cor, previous[cor.x][cor.y]);

    road.push( SE( dir, target ) );

    sf::Vector2u holder = cor;

    while ( !(cor.x == start.x  && cor.y == start.y) ){

        cor = { previous[cor.x][cor.y].first, previous[cor.x][cor.y].second };
        
        Direction newdir = CalculateDirection( cor, previous[cor.x][cor.y] );

        if( dir != newdir ){
            holder = cor;

            if(!(cor.x == start.x  && cor.y == start.y) ) road.push( SE(newdir, holder) );
            
        }
        dir = newdir;
    }

    for( unsigned int i = 0; i< m_mapSize.x; i++ ){
        delete [] previous[i];
    }
    delete [] previous;

    return road;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void S_Movement::SetTargetPosition(const EntityId& l_entity __attribute__((unused)), float l_x __attribute__((unused)), float l_y __attribute__((unused))){
  //  C_Position* position = m_systemManager->GetEntityManager()->GetComponent<C_Position>(l_entity,Component::Position);
   // auto pos = position->GetFPosition();

    //OUT OF DATE

  //  position->SetTargetPosition( l_x , l_y);

    #ifdef SDR_RUN
    std::cout << "Entity " << l_entity << " :: Postion Set to :: " << l_x << " " << l_y  << std::endl;
    #endif
}

void S_Movement::SetDirection(const EntityId& l_entity,const Direction& l_dir){

    #ifdef DEBUGG_RUN
        std::cout << "S_MOVEMENT :: SETDIRECTION :: BEGIN" << std::endl;
    #endif

    C_Movable* movable = m_systemManager->GetEntityManager()->
    GetComponent<C_Movable>(l_entity,Component::Movable);


    movable->SetDirection(l_dir);
    
    
    Message msg((MessageType)EntityMessage::Direction_Changed);
    msg.m_receivers.insert(l_entity);
    msg.m_int = (int)l_dir;
    //  m_systemManager->GetMessageHandler()->Dispatch(msg);

    #ifdef DEBUGG_RUN
        std::cout << "S_MOVEMENT :: SETDIRECTION :: DONE" << std::endl;
    #endif

}

void S_Movement::HandleEvent(const EntityId& l_entity __attribute__((unused)),const EntityEvent& l_event __attribute__((unused)))
{

    #ifdef DEBUGG_RUN
        std::cout << "S_MOVEMENT :: HANDLEEVENT :: START" << std::endl;
    #endif
    /*
    switch(l_event){
    case EntityEvent::Colliding_Left:
        C_Movable* movable = m_systemManager->GetEntityManager()->GetComponent<C_Movable>(l_entity,Component::Movable);
        switch(movable->GetDirection()){
            case Direction::LeftUp:
                 StopEntity(l_entity,Axis::x); 
                break;
            case Direction::LeftDown:
                 StopEntity(l_entity,Axis::x); 
                break;
            case Direction::Left:
                 StopEntity(l_entity,Axis::x); 
                break;
        }
        

        break;
    case EntityEvent::Colliding_Up:
        C_Movable* movable = m_systemManager->GetEntityManager()->GetComponent<C_Movable>(l_entity,Component::Movable);
        
        switch(movable->GetDirection()){
            case Direction::LeftUp:
                 StopEntity(l_entity,Axis::y); 
                break;
            case Direction::Up:
              StopEntity(l_entity,Axis::y); 
                break;
            case Direction::RightUp:
              StopEntity(l_entity,Axis::y); 
                break;
        }
        
        break;
    case EntityEvent::Colliding_Down:
        C_Movable* movable = m_systemManager->GetEntityManager()->GetComponent<C_Movable>(l_entity,Component::Movable);
                switch(movable->GetDirection()){
            case Direction::Down:
                StopEntity(l_entity,Axis::y); 
                break;
            case Direction::LeftDown:
              StopEntity(l_entity,Axis::y); 
                break;
            case Direction::RightDown:
              StopEntity(l_entity,Axis::y); 
                break;
        }
        
       
        break;
    case EntityEvent::Colliding_Right:
        C_Movable* movable = m_systemManager->GetEntityManager()->GetComponent<C_Movable>(l_entity,Component::Movable);
                switch(movable->GetDirection()){
            case Direction::RightUp:
            StopEntity(l_entity,Axis::x); 
                break;
            case Direction::RightDown:
             StopEntity(l_entity,Axis::x); 
                break;
            case Direction::Right:
             StopEntity(l_entity,Axis::x); 
                break;
        }
        break;

    case EntityEvent::Moving_Left:
        SetDirection(l_entity, Direction::Left); 
        break;

    case EntityEvent::Moving_Right:
        SetDirection(l_entity, Direction::Right); 
        break;

    case EntityEvent::Moving_LeftDown:
        SetDirection(l_entity, Direction::LeftDown); 
        break;

    case EntityEvent::Moving_LeftUp:
        SetDirection(l_entity, Direction::LeftUp); 
        break;
    case EntityEvent::Moving_RightDown:
        SetDirection(l_entity, Direction::RightDown); 
        break;
    case EntityEvent::Moving_RightUp:
        SetDirection(l_entity, Direction::RightUp); 
        break;

    case EntityEvent::Moving_Up:
    {
    C_Movable* mov = m_systemManager->GetEntityManager()->GetComponent<C_Movable>(l_entity,Component::Movable);
    if(mov->GetVelocity().x == 0){
        SetDirection(l_entity, Direction::Up);
        }
    }
    break;
    case EntityEvent::Moving_Down:
    {
    C_Movable* mov = m_systemManager->GetEntityManager()->GetComponent<C_Movable>(l_entity,Component::Movable);
    if(mov->GetVelocity().x == 0){
        SetDirection(l_entity, Direction::Down);
        }
    }
    break;
    default:
    break;
    }
    */
    #ifdef DEBUGG_RUN
        std::cout << "S_MOVEMENT :: HANDLEEVENT :: DONE" << std::endl;
    #endif
}

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

S_Movement::~S_Movement(){
    #ifdef DEBUGG_RUN
    std::cout << "S_MOVEMENT :: DESTRUCTOR" << std::endl;
    #endif

    m_gameMap = nullptr;

    #ifdef DEBUGG_RUN
    std::cout << "S_MOVEMENT :: END" << std::endl;
    #endif
}

void S_Movement::Update(float l_dT){
    #ifdef DEBUGG_RUN
        std::cout << "S_MOVEMENT :: UPDATE :: BEGIN" << std::endl;
    #endif

    #ifdef DEBUGG_RUN
        std::cout << "S_MOVEMENT :: UPDATE" << std::endl;
    #endif

    if (!m_gameMap){ return; }
    EntityManager* entities = m_systemManager->GetEntityManager();


    std::set<unsigned int> m_leftWalkers;
    std::set<unsigned int> m_rightWalkers;
    std::set<unsigned int> m_upWalkers;
    std::set<unsigned int> m_downWalkers;
    std::set<unsigned int> m_walkers;


    for(auto &entity : m_entities){
        
        C_Position* position = entities->GetComponent<C_Position>(entity, Component::Position);
        if( position->GetTargetPosition() == position->GetPosition()){
            continue;
        }
        
        #ifdef SDR_RUN
            std::cout << "Entity " << entity << " :: Position :: "<< position->GetPosition().x<< " "<< position->GetPosition().y << std::endl;
        #endif

        C_Movable* movable = entities->GetComponent<C_Movable>(entity, Component::Movable);

        MovementStep(l_dT, movable, position);
        CalculatePath(position, movable);  

        #ifdef SDR_RUN
            std::cout << "Entity " << entity << " :: MoveBy :: "<< movable->GetVelocity().x<< " "<< movable->GetVelocity().y << std::endl;
        #endif
        
        bool CloseX = std::abs( position->GetTargetPosition().x -  position->GetPosition().x ) <= movable->GetSpeed().x* l_dT;
        bool CloseY = std::abs( position->GetTargetPosition().y -  position->GetPosition().y ) <= movable->GetSpeed().y* l_dT;

     //   if( position->GetTargetPosition().x != position->GetPosition().x){
        if( CloseX ){
           StopEntity(entity, Axis::x);
           position->SetPosition(position->GetTargetPosition().x, position->GetPosition().y);
           
        }
            
        if( CloseY ){
            StopEntity(entity, Axis::y);
            position->SetPosition(position->GetPosition().x,position->GetTargetPosition().y);
            
        }

         if(CloseX and CloseY){

            position->SetPosition( position->GetTargetPosition() );
            StopEntity(entity);

            m_systemManager->AddEvent(
                entity,
                (EventID)EntityEvent::Became_Idle);
            
        
        

        }else{
            position->MoveBy(movable->GetVelocity());
        }


        #ifdef SDR_RUN
            std::cout << "Entity "
             << entity << " :: STEP BY  :: "
             << movable->GetVelocity().x << " "
             << movable->GetVelocity().y << std::endl;
        #endif
    

        if( (movable->GetVelocity().x != 0) 
            and 
            (movable->GetVelocity().y != 0)
         ){
        if( movable->GetDirection() == Direction::Left ){
        //    movable->SetDirection(Direction::Left);
            m_leftWalkers.insert(entity);
        }else if( movable->GetDirection() == Direction::Right ) {
        //    movable->SetDirection(Direction::Right);
            m_rightWalkers.insert(entity);
        }else if( movable->GetDirection() == Direction::Up ){
          //      movable->SetDirection(Direction::Down);
                m_downWalkers.insert(entity);
        }else if( movable->GetDirection() == Direction::Down ){
            //    movable->SetDirection(Direction::Up);
                m_upWalkers.insert(entity);
        }

            
    }
        m_walkers.insert(entity);
            
        #ifdef SDR_RUN
            std::cout << "Entity " << entity << " :: MoveBy :: "<< movable->GetVelocity().x<< " "<< movable->GetVelocity().y << std::endl;
        #endif

    }



    if( m_leftWalkers.size()  ) SendMessage(EntityMessage::Direction_Changed, (int)Direction::Left,  m_leftWalkers);
    if( m_rightWalkers.size() ) SendMessage(EntityMessage::Direction_Changed, (int)Direction::Right, m_rightWalkers);
    if( m_upWalkers.size()    ) SendMessage(EntityMessage::Direction_Changed, (int)Direction::Up,    m_upWalkers);
    if( m_downWalkers.size()  ) SendMessage(EntityMessage::Direction_Changed, (int)Direction::Down,  m_downWalkers);
   // if( m_walkers.size()      ) SendMessage(EntityMessage::Is_Moving, -1 ,m_walkers);

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
                    if (movable->GetVelocity() != sf::Vector2f(0.0f, 0.0f)){ continue; }

                        m_systemManager->AddEvent(
                            reciver,
                            (EventID)EntityEvent::Became_Idle);
                        }
                break;
                default:
                break;
            
        
        }
        #ifdef DEBUGG_RUN
            std::cout << "S_MOVEMENT :: NOTIFY :: DONE" << std::endl;
        #endif
}

void S_Movement::CalculatePath(C_Position* position, C_Movable* movable){
    sf::Vector2f tartget = position->GetTargetPosition();
    sf::Vector2f now     = position->GetPosition();

    if( position->flag ){

    if( tartget.x <= now.x ){
        movable->SetDirection(Direction::Left);
        movable->Move(Direction::Left);
    }else{
        movable->SetDirection(Direction::Up);
        movable->Move(Direction::Right);
    }
    position->flag = false;
    }else{

    if( tartget.y <= now.y){
        movable->SetDirection(Direction::Up);
        movable->Move(Direction::Up);
        
    }else{
        movable->SetDirection(Direction::Down);
        movable->Move(Direction::Down);
        
    }
    position->flag = true;
    }
}

void S_Movement::HandleEvent(const std::set<unsigned int>& participians __attribute((unused)), const EntityEvent& l_event){
    #ifdef DEBUGG_RUN
        std::cout << "S_MOVEMENT :: HANDLEEVENT :: START" << std::endl;
    #endif

    switch(l_event){
        case EntityEvent::Colliding_X:
            for( auto entity : participians )
                StopEntity(entity,Axis::x); 
            break;

        case EntityEvent::Colliding_Y:
            for( auto entity : participians )
                StopEntity(entity, Axis::y); 
            break;

        case EntityEvent::Moving_Left:
                SetDirection(participians, Direction::Left); 
            break;

        case EntityEvent::Moving_Right:
                SetDirection(participians, Direction::Right); 
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
            if( temp.size()) SetDirection(temp, Direction::Up);            
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
        default:
        break;
    }

    #ifdef DEBUGG_RUN
        std::cout << "S_MOVEMENT :: HANDLEEVENT :: DONE" << std::endl;
    #endif
}


void S_Movement::MovementStep(float l_dT __attribute__((unused)), C_Movable* l_movable,C_Position* l_position){

    //GETTING HOW LONG STEP IS
    sf::Vector2f f_coefficient =GetTileFriction(
        l_position->GetElevation(),
        floor(l_position->GetPosition().x / Sheet::Tile_Size) > 0 ? floor(l_position->GetPosition().x / Sheet::Tile_Size) : 0 ,
        floor(l_position->GetPosition().y / Sheet::Tile_Size) > 0 ? floor(l_position->GetPosition().y / Sheet::Tile_Size) : 0 
    );
    /*
    float magnitude = sqrt(
        (l_movable->GetVelocity().x * l_movable->GetVelocity().x) +
        (l_movable->GetVelocity().y * l_movable->GetVelocity().y)
    );
    */
    #ifdef SDR_RUN
        std::cout << "Entity "
         << " :: cof :: " << f_coefficient.x << " " << f_coefficient.y  << std::endl
         << " :: speed :: " << l_movable->GetSpeed().x << " " << l_movable->GetSpeed().y << std::endl
         << " :: l_dt :: " << l_dT 
         << std::endl ;
    #endif


    l_movable->SetModificator(
        (f_coefficient.x * l_movable->GetSpeed().x) *l_dT,
        (f_coefficient.y * l_movable->GetSpeed().y) *l_dT
    );

    /*Mo

    float magnitude = sqrt(
    (l_movable->GetVelocity().x * l_movable->GetVelocity().x) +
    (l_movable->GetVelocity().y * l_movable->GetVelocity().y));

    if (magnitude <= l_movable->GetMaxVelocity()){ return; }
    float max_V = l_movable->GetMaxVelocity();

    l_movable->SetVelocity(sf::Vector2f(
    (l_movable->GetVelocity().x / magnitude) * max_V,
    (l_movable->GetVelocity().y / magnitude) * max_V));

    */

}

const sf::Vector2f& S_Movement::GetTileFriction(
    unsigned int l_elevation __attribute__((unused)) , unsigned int l_x, unsigned int l_y)
    {
  //   if( l_y >  100000 or l_x > 100000  ) { return m_gameMap->GetTerrianFriction( 0,0 ) ;}

    #ifdef DEBUGG_RUN
        std::cout << "S_MOVEMENT :: GETTLEFRICTION :: START" << std::endl;
    #endif
        return m_gameMap->GetTerrianFriction(l_x,l_y);
}

void S_Movement::StopEntity(const EntityId& l_entity){
    C_Movable* movable = m_systemManager->GetEntityManager()->GetComponent<C_Movable>(l_entity,Component::Movable);
    C_Position* position = m_systemManager->GetEntityManager()->GetComponent<C_Position>(l_entity,Component::Position);
    
    position->SetTargetPosition( position->GetPosition().x, position->GetPosition().y);

    #ifdef SDR_RUN
        std::cout << "Entity " << l_entity << " :: STOP" << std::endl;
    #endif

    movable->SetModificator(sf::Vector2f(0.f,0.f));
}

void S_Movement::StopEntity(const EntityId& l_entity,const Axis& l_axis)
    {
    #ifdef DEBUGG_RUN
        std::cout << "S_MOVEMENT :: STOP :: BEGIN" << std::endl;
    #endif
    C_Position* position = m_systemManager->GetEntityManager()->GetComponent<C_Position>(l_entity,Component::Position);
    
    C_Movable* movable = m_systemManager->GetEntityManager()->GetComponent<C_Movable>(l_entity,Component::Movable);
    if(l_axis == Axis::x){
        movable->SetVelocity(sf::Vector2f(0.f, movable->GetVelocity().y));

        position->SetTargetPosition( position->GetPosition().x, position->GetTargetPosition().y);
    } else if(l_axis == Axis::y){
        movable->SetVelocity(sf::Vector2f(movable->GetVelocity().x, 0.f));
    }

    #ifdef DEBUGG_RUN
        std::cout << "S_MOVEMENT :: STOP :: DONE" << std::endl;
    #endif
}

void S_Movement::SetTargetPosition( float l_x, float l_y){
    
    std::set<unsigned int> m_leftWalkers;
    std::set<unsigned int> m_rightWalkers;
    std::set<unsigned int> m_upWalkers;
    std::set<unsigned int> m_downWalkers;

    // auto pos = position->GetPosition();

    C_Movable* movable;

    for(auto &entity : m_entities){
    C_Controller* contr = m_systemManager->GetEntityManager()->GetComponent<C_Controller>(entity,Component::Controller);
    if(! contr->IsActive()){ continue; }       
    C_Position* position = m_systemManager->GetEntityManager()->GetComponent<C_Position>(entity,Component::Position);
    movable = m_systemManager->GetEntityManager()->GetComponent<C_Movable>(entity,Component::Movable);
    

   

    auto pos = position->GetPosition();

    if( l_x < pos.x ){
        movable->SetDirection(Direction::Left);
        m_leftWalkers.insert(entity);
    }else if( l_x > pos.x ) {
        movable->SetDirection(Direction::Right);
        m_rightWalkers.insert(entity);
    }else{
        if( l_y < pos.y){
            movable->SetDirection(Direction::Down);
            m_downWalkers.insert(entity);
        }else{
            movable->SetDirection(Direction::Up);
            m_upWalkers.insert(entity);
        }
    }

    position->SetTargetPosition( l_x , l_y);

    
    position->SetStackSteps(AStar(entity));

    }


    if( m_leftWalkers.size()  ) SendMessage(EntityMessage::Move, (int)Direction::Left,  m_leftWalkers);
    if( m_rightWalkers.size() ) SendMessage(EntityMessage::Move, (int)Direction::Right, m_rightWalkers);
    if( m_upWalkers.size()    ) SendMessage(EntityMessage::Move, (int)Direction::Up,    m_upWalkers);
    if( m_downWalkers.size()  ) SendMessage(EntityMessage::Move, (int)Direction::Down,  m_downWalkers);


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
    position->SetPosition(l_x,l_y);
    position->SetTargetPosition(l_x,l_y);
}

void S_Movement::SetMap(Map* l_gameMap){
    #ifdef DEBUGG_RUN
    std::cout << "S_MOVEMENT :: SETMAP :: BEGIN" << std::endl;
    #endif
    
    m_gameMap = l_gameMap; 

    m_mapSize = l_gameMap->GetMapSize();

    #ifdef DEBUGG_RUN
    std::cout << "S_MOVEMENT :: SETMAP :: DONE" << std::endl;
    #endif
}

std::stack<SE>& S_Movement::AStar(unsigned int& l_entity){
    C_Position* position = m_systemManager->GetEntityManager()->GetComponent<C_Position>(l_entity,Component::Position);


    sf::Vector2u pos = { 
        ((unsigned int)position->GetPosition().x)/32,
        ((unsigned int)position->GetPosition().y)/32
    };



    sf::Vector2u targ = { 
        (unsigned int)position->GetTargetPosition().x/32,
        (unsigned int)position->GetTargetPosition().y/32
    };    

    std::cout << pos.x << " " << pos.y << "  :  " << targ.x << " " << targ.y << std::endl;

    using my_container_t = std::vector<QE>;

    auto my_comp = [](const QE& e1, const QE& e2){
        return e1.u1 > e2.u1;
    };

   // const sf::Vector2u m_mapSize = m_gameMap->GetMapSize();

    /////////////////////////////////////////////////////////////////////////////////

    std::priority_queue< 
        QE,
        my_container_t,
        decltype(my_comp)
    >unseen(my_comp);

    bool**seen       = new bool*[m_mapSize.x];
    bool**unSeen     = new bool*[m_mapSize.x];
    unsigned int **g = new unsigned int *[m_mapSize.x];
    std::pair<unsigned int, unsigned int>** previous = new  std::pair<unsigned int, unsigned int>*[m_mapSize.x];

    for( unsigned int i = 0; i < m_mapSize.x; i++ ){
           seen[i] = new bool[m_mapSize.y];
         unSeen[i] = new bool[m_mapSize.y];
              g[i] = new unsigned int[m_mapSize.y];
       previous[i] = new  std::pair<unsigned int, unsigned int>[m_mapSize.y];
    }



    for( unsigned int i = 0; i < m_mapSize.x; i++){
        for( unsigned int j =0; j< m_mapSize.y; j++){
            seen[i][j] = false;
            unSeen[i][j] = true;
            g[i][j] = 2000;
       }
    }

    // CALCULATE AT THE BEGIIGNING OF MOVE :
    //  FIND PATH:
    //  SAVE PATH : as list of directions 
    //  IF COLLISION ON PATH : RECALCULATE
    //  IF NO PATH IN COLLISION EVENT : STOP   

    unseen.push( 
        QE(0, pos.x, pos.y)
    );

    g[pos.x][pos.y] = 0; 
    unSeen[pos.x][pos.y] = false;

    int howM = 0;

    while( !unseen.empty() ){
        howM ++;
        
        const QE x = unseen.top();
        if( (x.u2 == targ.x) and (x.u3 == targ.y) ) {

        for( unsigned int i = 0; i<m_mapSize.x; i++ ){
            delete [] seen[i];
            delete [] unSeen[i];
            delete [] g[i];
        }
        delete [] seen;
        delete [] unSeen;
        delete [] g; 
   
             return  CalculateRoad(previous, pos, targ);
        } // CALCULATE BACK:
        
        seen[x.u2][x.u3] = true;
        
        unseen.pop();

        sf::Vector2u xs = {
            ( x.u2 != 0 ) ? x.u2-1 : x.u2,
            ( x.u2 != m_mapSize.x-1 ) ? x.u2+1 : x.u2
         };

         sf::Vector2u ys = {
            ( x.u3 != 0 ) ? x.u3-1 : x.u3,
            ( x.u3 != m_mapSize.y-1 ) ? x.u3+1 : x.u3
         };

        for( unsigned int i = xs.x;i <= xs.y;i++ )
        {
            for( unsigned int j = ys.x;j <= ys.y;j++ )
            {

                
                if( seen[i][j] ) { continue; }

                unsigned int geoX = i - targ.x;
                unsigned int geoY = j - targ.y; 

                geoX = geoX*geoX;
                geoY = geoY*geoY;

                unsigned int newG = g[x.u2][x.u3] + std::round(sqrt(geoX+geoY))*10 + (100 * ( 1.0f - m_gameMap->GetTerrianFriction(i,j).x )) ;

                bool isBetter = false;
                unseen.push( 
                    QE(newG, i, j)
                );
                if( ! unSeen[i][j]  ){
                    unSeen[i][j] = false;
                    isBetter = true; 
                }else if( newG < g[i][j] ) {
                    isBetter = true;
                }

                if(isBetter){
                    
                    previous[i][j] = std::make_pair(x.u2, x.u3);
                    g[i][j] = newG;
                }
            }
        }
    }

    for( unsigned int i = 0; i<m_mapSize.x; i++ ){
        delete [] seen[i];
        delete [] unSeen[i];
        delete [] g[i];
        delete [] previous[i];
    }
    delete [] seen;
    delete [] unSeen;
    delete [] g; 
    delete [] previous;

    std::stack<SE> temp;
    std::stack<SE>& t = temp;

    return t;
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

std::stack<SE>& S_Movement::CalculateRoad(std::pair<unsigned int, unsigned int>** previous, sf::Vector2u& start, sf::Vector2u& target){
    std::stack<SE> t;
    std::stack<SE>& road = t;

    sf::Vector2u cor = target;


    //First Check
    
    int x_dist = cor.x - previous[cor.x][cor.y].first ; 
    int y_dist = cor.y - previous[cor.x][cor.y].second;

    Direction dir;

   switch(x_dist){
            case 0:
            switch(y_dist){
                case 1:
                        dir = Direction::Down;
                break;
                case -1:
                        dir = Direction::Up;  
                break;
                default:
                break;
            }
            break;
            case 1:
            switch(y_dist){
                case 0:
                    dir = Direction::Right;
                break;
                case 1:
                    dir = Direction::RightDown;
                break;
                case -1:
                    dir = Direction::RightUp;
                break;
                default:
                break;
            }
            break;
            case -1:
            switch(y_dist){
                case 0:
                    dir = Direction::Left;   
                break;
                case 1:
                    dir = Direction::LeftDown;
                break;
                case -1:
                    dir = Direction::LeftUp;
                break;
                default:
                break;
            }
            break;
        default:
        break;
    }


    while ( !(cor.x == start.x  && cor.y == start.y) ){

        sf::Vector2u newTarget = { previous[cor.x][cor.y].first, previous[cor.x][cor.y].second };
    
        x_dist = cor.x - previous[cor.x][cor.y].first ; 
        y_dist = cor.y - previous[cor.x][cor.y].second;

        switch(x_dist){
            case 0:
            switch(y_dist){
                case 1:
                    if(dir != Direction::Down){
                        road.push( SE( dir, cor ) );
                        dir = Direction::Down;
                    }
                break;
                case -1:
                    if(dir != Direction::Up){
                        road.push( SE( dir, cor ) );
                        dir = Direction::Up;                        
                    }
                break;
                default:
                break;
            }
            break;
            case 1:
            switch(y_dist){
                case 0:
                    if(dir != Direction::Right){
                        road.push( SE( dir, cor ) );
                        dir = Direction::Right;
                    }
                break;
                case 1:
                    if(dir != Direction::RightDown){
                        road.push( SE( dir,cor ) );
                        dir = Direction::RightDown;
                    }
                break;
                case -1:
                    if(dir != Direction::RightUp){
                        road.push( SE( dir,cor ) );
                        dir = Direction::RightUp;
                    }
                break;
                default:
                break;
            }
            break;
            case -1:
            switch(y_dist){
                case 0:
                    if(dir != Direction::Left){
                        road.push( SE( dir, cor ) );
                        dir = Direction::Left;
                    }
                break;
                case 1:
                    if(dir != Direction::LeftDown){
                        road.push( SE( dir, cor ) );
                        dir = Direction::LeftDown;
                    }
                break;
                case -1:
                    if(dir != Direction::LeftUp){
                        road.push( SE( dir, cor ) );
                        dir = Direction::LeftUp;
                    }
                break;
                default:
                break;
            }
            break;
            default:
            break;
        }
        cor = newTarget;
    }
   
    road.push( SE( dir, cor) );

    for( unsigned int i = 0; i< m_mapSize.x; i++ ){
        delete [] previous[i];
    }
    delete [] previous;

    return road;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void S_Movement::SetTargetPosition(const EntityId& l_entity, float l_x, float l_y){
  //  C_Position* position = m_systemManager->GetEntityManager()->GetComponent<C_Position>(l_entity,Component::Position);
   // auto pos = position->GetPosition();

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

void S_Movement::HandleEvent(const EntityId& l_entity,const EntityEvent& l_event)
{

    #ifdef DEBUGG_RUN
        std::cout << "S_MOVEMENT :: HANDLEEVENT :: START" << std::endl;
    #endif

    switch(l_event){
    case EntityEvent::Colliding_X:
        StopEntity(l_entity,Axis::x); 
        break;

    case EntityEvent::Colliding_Y:
        StopEntity(l_entity, Axis::y); 
        break;

    case EntityEvent::Moving_Left:
        SetDirection(l_entity, Direction::Left); 
        break;

    case EntityEvent::Moving_Right:
        SetDirection(l_entity, Direction::Right); 
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

#ifdef DEBUGG_RUN
    std::cout << "S_MOVEMENT :: HANDLEEVENT :: DONE" << std::endl;
#endif
}


/*
void S_Movement::MovementStep(float l_dT, C_Movable* l_movable,C_Position* l_position)
    {


    #ifdef DEBUGG_RUN
        std::cout << "S_MOVEMENT :: MOVEMENTSTEP :: START" << std::endl;
    #endif

        sf::Vector2f f_coefficient =GetTileFriction(
            l_position->GetElevation(),
            floor(l_position->GetPosition().x / Sheet::Tile_Size),
            floor(l_position->GetPosition().y / Sheet::Tile_Size)
        );

        sf::Vector2f friction(l_movable->GetSpeed().x * f_coefficient.x,l_movable->GetSpeed().y * f_coefficient.y);
        
        l_movable->AddVelocity(l_movable->GetAcceleration() * l_dT);
        l_movable->SetAcceleration(sf::Vector2f(0.0f, 0.0f));
        l_movable->ApplyFriction(friction * l_dT);

        float magnitude = sqrt(
        (l_movable->GetVelocity().x * l_movable->GetVelocity().x) +
        (l_movable->GetVelocity().y * l_movable->GetVelocity().y));

    if (magnitude <= l_movable->GetMaxVelocity()){ return; }
    float max_V = l_movable->GetMaxVelocity();
    
    l_movable->SetVelocity(sf::Vector2f(
    (l_movable->GetVelocity().x / magnitude) * max_V,
    (l_movable->GetVelocity().y / magnitude) * max_V));

    #ifdef DEBUGG_RUN
        std::cout << "S_MOVEMENT :: MOVEMENTSTEP :: DONE" << std::endl;
    #endif
}
*/
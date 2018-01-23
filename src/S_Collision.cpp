
#include "S_Collision.h"

S_Collision::S_Collision(SystemManager* l_systemMgr)
:S_Base(System::Collision,l_systemMgr)
{
    Bitmask req;
    req.TurnOnBit((unsigned int)Component::Position);
    req.TurnOnBit((unsigned int)Component::State);
    req.TurnOnBit((unsigned int)Component::Collidable);
    m_requiredComponents.push_back(req);
    req.Clear();
    m_gameMap = nullptr;

}

S_Collision::~S_Collision(){
    m_gameMap = nullptr;

    for( unsigned int i = 0; i < m_setsSizes.y; i++){
      delete[] m_squares[i];
    }

    delete[] m_squares;
      
}

void S_Collision::SetMap(Map* l_map){ 

    m_gameMap = l_map; 

    mapSize = m_gameMap->GetMapSize();
    m_setsSizes = { static_cast<unsigned int>(((mapSize.x*32)/SQUARE_SIZE)+2) , static_cast<unsigned int>(((mapSize.y*32)/SQUARE_SIZE)+2) };

    m_squares = new std::set<unsigned int>*[m_setsSizes.y];
    for( unsigned int i = 0 ; i < m_setsSizes.y; i++)
        m_squares[i] = new std::set<unsigned int>[m_setsSizes.x];
}

void S_Collision::HandleEvent(const EntityId& l_entity
    __attribute((unused)),
     const EntityEvent& l_event __attribute((unused))){

}


void S_Collision::Update(float l_dT __attribute((unused))){
    if (!m_gameMap){ return; }
    EntityManager* entities = m_systemManager->GetEntityManager();

    for(auto &entity : m_entities){
        C_Position* position = entities->GetComponent<C_Position>(entity, Component::Position);
        C_Collidable* collidable = entities->GetComponent<C_Collidable>(entity, Component::Collidable);
    

        if( !position->isMoving() or position->isMoving() ){
            collidable->SetPosition(position->GetFPosition());
            continue;
        }


        
        collidable->SetPosition(position->GetFPosition());
        collidable->ResetCollisionFlags();

        ReplaceUnit(position->GetFPosition().x, position->GetFPosition().y, entity);
   }

        //    CheckOutOfBounds(position, collidable);
            // TO powyzej może się nie przydać

        //    MapCollisions(entity, position, collidable);
            // A TO nas chyba nie bardzo obowiązuje

    EntityCollisions();
}



void S_Collision::ReplaceUnit(float l_x, float l_y, unsigned int& l_entity){

    const float offsetX = 32.0f;
    const float offsetY = 32.0f;
   
    for( unsigned int i = 0 ; i< m_setsSizes.x; i++){
        for( unsigned int j = 0; j < m_setsSizes.y; j++){

            auto it = m_squares[i][j].find(l_entity);

            if( it != m_squares[i][j].end() ){

                if(!(
                    (( l_x > (static_cast<float>(i*SQUARE_SIZE) -offsetX)  and l_x < (i+1)*SQUARE_SIZE )
                    and
                    ( l_y > (static_cast<float>(j*SQUARE_SIZE) -offsetY)  and l_y < (j+1)*SQUARE_SIZE ))
                )
                ){
                    DeleteFromSquare( sf::Vector2u(i,j),l_entity);
                    SwitchSquare(sf::Vector2f(l_x, l_y), sf::Vector2u(i,j), l_entity);
                    return;
                }
            }
        }
    }
}

bool S_Collision::RemoveEntity(const EntityId& l_entity){

    ReleaseUnit(l_entity);

    for( auto it = m_entities.begin(); it != m_entities.end(); it++){
        if( l_entity == *it ){
            m_entities.erase(it);
            return true;
        }
    }
    return false;
}

void S_Collision::ReleaseUnit(const unsigned int & l_entity){
    EntityManager* entities = m_systemManager->GetEntityManager();

    C_Position* position  = entities->GetComponent<C_Position>(l_entity , Component::Position);


    sf::Vector2u v = {
        static_cast<unsigned int>(position->GetFPosition().x/SQUARE_SIZE) , 
        static_cast<unsigned int>(position->GetFPosition().y/SQUARE_SIZE) 
    };

    if( v.x == 0 ){ v.x = 1;}
    if( v.y == 0 ){ v.y = 1;}

    if( v.x == m_setsSizes.x ){ v.x = m_setsSizes.x -1;}
    if( v.y == m_setsSizes.y ){ v.y = m_setsSizes.y -1;}



    for( unsigned int i = v.x - 1 ; i<= v.x +1; i++){
        for( unsigned int j = v.y - 1; j <= v.y+1; j++){

            auto it = m_squares[i][j].find(l_entity);
            if( it != m_squares[i][j].end() ){
                m_squares[i][j].erase(it);
            }
        }
    }
}

void S_Collision::DeleteFromSquare( sf::Vector2u l_set, unsigned int & l_entity){
    if( l_set.x == 0 ){ l_set.x = 1;}
    if( l_set.y == 0 ){ l_set.y = 1;}

    if( l_set.x == m_setsSizes.x ){ l_set.x = m_setsSizes.x -1;}
    if( l_set.y == m_setsSizes.y ){ l_set.y = m_setsSizes.y -1;}

    for( unsigned int i = l_set.x - 1 ; i<= l_set.x +1; i++){
        for( unsigned int j = l_set.y - 1; j <= l_set.y+1; j++){

            auto it = m_squares[i][j].find(l_entity);
            if( it != m_squares[i][j].end() ){
                m_squares[i][j].erase(it);
            }
        }
    }
}


void S_Collision::SwitchSquare(sf::Vector2f l_pos, sf::Vector2u l_set, unsigned int & l_entity){

    const float offsetX = 32.0f;
    const float offsetY = 32.0f;

    if( l_set.x == 0 ){ l_set.x = 1;}
    if( l_set.y == 0 ){ l_set.y = 1;}

    if( l_set.x == m_setsSizes.x ){ l_set.x = m_setsSizes.x -1;}
    if( l_set.y == m_setsSizes.y ){ l_set.y = m_setsSizes.y -1;}

    for( unsigned int i = l_set.x - 1 ; i<= l_set.x +1; i++){
        for( unsigned int j = l_set.y - 1; j <= l_set.y+1; j++){
            if(
                (( l_pos.x > (static_cast<float>(i*SQUARE_SIZE) -offsetX)  and l_pos.x < (i+1)*SQUARE_SIZE )
                and
                ( l_pos.y > (static_cast<float>(j*SQUARE_SIZE) -offsetY)  and l_pos.y < (j+1)*SQUARE_SIZE ))
            ){
                m_squares[i][j].insert(l_entity);
            }
        }
    }
}

void S_Collision::EntityCollisions(){

    EntityManager* entities = m_systemManager->GetEntityManager();

    for( unsigned int i = 0; i < m_setsSizes.y; i++){
        for( unsigned int j = 0; j < m_setsSizes.x; j++){
     
            if( !m_squares[i][j].size() ){continue;}

            for(auto itr = m_squares[i][j].begin() ;itr != m_squares[i][j].end(); ++itr){
                for(auto itr2 = std::next(itr);itr2 != m_squares[i][j].end(); ++itr2){

                    C_Position* position  = entities->GetComponent<C_Position>(*itr , Component::Position);
                    C_Position* position2 = entities->GetComponent<C_Position>(*itr2, Component::Position);

                    if( i == 0 ){

                        if( position->GetFPosition().x < 32  ){
                            
                            //std::cout << i << " " << j << std::endl;

                            m_systemManager->AddEvent(
                                *itr,
                                (EventID)EntityEvent::Colliding_Left);
                             }
                            
                        
                            
                        if( position2->GetFPosition().x < 32 ){
                            
                            //std::cout << i << " " << j << std::endl;

                            m_systemManager->AddEvent(
                                *itr2,
                                (EventID)EntityEvent::Colliding_Left);
                             }
                            
                        


                    }

                    if( j == 0){

                        if( position->GetFPosition().y < 32 ){
                            //std::cout << i << " " << j << std::endl;
                            m_systemManager->AddEvent(
                                *itr,
                                (EventID)EntityEvent::Colliding_Up);
                             }
                            
                        
                            
                        if( position2->GetFPosition().y < 32 ){
                           // std::cout << i << " " << j << std::endl;
                            m_systemManager->AddEvent(
                                *itr2,
                                (EventID)EntityEvent::Colliding_Up);
                             }
                            
                        


                    }

                    if( i == m_setsSizes.x-1){
                        if( position->GetFPosition().x > mapSize.x-32 ){
                            //std::cout << i << " " << j << std::endl;
                            m_systemManager->AddEvent(
                                *itr,
                                (EventID)EntityEvent::Colliding_Right);
                             }
                            
                        
                            
                        if( position2->GetFPosition().x > mapSize.x-32 ){
                           // std::cout << i << " " << j << std::endl;
                            m_systemManager->AddEvent(
                                *itr2,
                                (EventID)EntityEvent::Colliding_Right);
                             }
                            
                        
                    }

                    if( j == m_setsSizes.y-1){

                        if( position->GetFPosition().y > mapSize.y-32 ){
                            //std::cout << i << " " << j << std::endl;
                            m_systemManager->AddEvent(
                                *itr,
                                (EventID)EntityEvent::Colliding_Down);
                             }
                            
                        
                            
                        if( position2->GetFPosition().y > mapSize.y-32 ){
                            //std::cout << i << " " << j << std::endl;
                            m_systemManager->AddEvent(
                                *itr2,
                                (EventID)EntityEvent::Colliding_Down);
                             }
                            }
                            

                    if( !position->isMoving()  and  !position2->isMoving() )
                    {


                        continue;
                    }




                    int x = position2->GetFPosition().x - position->GetFPosition().x;
                    x = x*x; 
                    int y = position2->GetFPosition().y - position->GetFPosition().y;
                    y = y*y;

                    const int DISTANCE = 84*84;

                    if( DISTANCE > x+y ){

                        C_Collidable* collidable1 = entities->GetComponent<C_Collidable>(*itr, Component::Collidable);
                        C_Collidable* collidable2 = entities->GetComponent<C_Collidable>(*itr2, Component::Collidable);

                        if(collidable1->GetCollidable().intersects(collidable2->GetCollidable())){
                        //     std::cout << "We get Collision" << std::endl;

                            /*

                            std::cout << " Speed 1 " <<  position->GetVelocity().x << " " <<  position->GetVelocity().y << std::endl;
                            std::cout << " Position 1 " << position->GetFPosition().x + position->GetVelocity().x <<
                            " " << position->GetFPosition().x + position->GetVelocity().x << std::endl;
                            
                            std::cout << " Speed 2 " << position2->GetVelocity().x << " " << position2->GetVelocity().y << std::endl;
                            std::cout << " Position 2 " << position2->GetFPosition().x + position2->GetVelocity().x <<
                            " " << position2->GetFPosition().x + position2->GetVelocity().x << std::endl;
                            

                            if( ! position->isChanging() or  position->IsPaused() ){
                                    position2->Pause();
                                    collidable2->SetPosition(position2->GetFPosition());
                            }else if( ! position2->isChanging() or position2->IsPaused() ){
                                    position->Pause();
                                    collidable1->SetPosition(position->GetFPosition());
                            }else{
                            

                                    float x = position2->GetFPosition().x - position->GetFPosition().x;
                                        x = x*x; 
                                    float y = position2->GetFPosition().y - position->GetFPosition().x;
                                        y = y*y;

                                    float sum = x + y;

                                    if( sum <= 48 ){

                                        position2->Pause();
                                        collidable2->SetPosition(position->GetFPosition());

                                    }

                            */
                                //if( position->GetFPosition)

                                /*

                                    if(  MovingTheSameDirection(position->GetVelocity(), position2->GetVelocity() )){


                                    int x = position2->GetFPosition().x - 32;
                                        x = x*x; 
                                    int y = position2->GetFPosition().y - 32;
                                        y = y*y;

                                    int sum = x + y;

                                        x = position->GetFPosition().x - 32;
                                        y = position->GetFPosition().y - 32;

                                        x*=x;
                                        y*=y;

                                    if( sum > x+y ){
                                        position2->Pause();
                                        collidable2->SetPosition(position2->GetFPosition());
                                    }else{
                                        position->Pause();
                                        collidable1->SetPosition(position->GetFPosition());
                                    }

                            //        position2->Pause();
                            ///        collidable2->SetPosition(position2->GetFPosition());

                                    }else{


                                    }


                                    */
                         //   }



                            /*

                            if( position->isChanging() and position2->isChanging() ){
 
                                

                                if( position->IsPaused() ){
                                    position2->Pause();
                                    collidable2->SetPosition(position2->GetFPosition());
                                }else if( position2->IsPaused() ){
                                    position->Pause();
                                    collidable1->SetPosition(position->GetFPosition());
                                }else{
                                    position->Pause();
                                    collidable1->SetPosition(position->GetFPosition());
                                    
                                }
                                
                                */

                                //else{
                                //    position->Pause();
                                //    collidable1->SetPosition(position->GetFPosition());
                                //}
 
                               // if( ! MovingTheSameDirection(position->GetVelocity(), position2->GetVelocity() )){
                            //        position2->Pause();
                             //   }


                           // }else if( position->isChanging() ){
                             //   position->Pause();
                            //    collidable1->SetPosition(position->GetFPosition());
        
                          //  }else if( position2->isChanging() ){
                                //position2->Pause();
                                //collidable2->SetPosition(position2->GetFPosition());
        
                              //  }*/
                            }
                        }
                    }
                }
            }
        
    }    
}

bool S_Collision::MovingTheSameDirection( sf::Vector2f& p1, sf::Vector2f& p2 ){
    if( p1.x > 0 ){
        if( p2.x == 0 ){
            return false;
        }else if( p2.x < 0){
            return false;
        }
    } else if( p1.x == 0 ){
        if( p2.x < 0 ){
            return false;
        }else if( p2.x > 0){
            return false;
        }
    } else if( p1.x < 0 ){
        if( p2.x == 0 ){
            return false;
        }else if( p2.x > 0){
            return false;
        }
    } 

    if( p1.y > 0 ){
        if( p2.y == 0 ){
            return false;
        }else if( p2.y < 0){
            return false;
        }
    } else if( p1.y == 0 ){
        if( p2.y < 0 ){
            return false;
        }else if( p2.y > 0){
            return false;
        }
    } else if( p1.y < 0 ){
        if( p2.y == 0 ){
            return false;
        }else if( p2.y > 0){
            return false;
        }
    } 

    return true;
}


void S_Collision::PlaceUnit( float l_x, float l_y, unsigned int l_entity){
    EntityManager* entities = m_systemManager->GetEntityManager();
   
    const float offsetX = 32.0f;
    const float offsetY = 32.0f;
   
    for( unsigned int i = 0 ; i< m_setsSizes.x; i++){
        for( unsigned int j = 0; j < m_setsSizes.y; j++){

            if( m_squares[i][j].size() ) m_squares[i][j].erase(l_entity);

            if(
                ( l_x > (static_cast<float>(i*SQUARE_SIZE) -offsetX)  and l_x < (i+1)*SQUARE_SIZE )
                and
                ( l_y > (static_cast<float>(j*SQUARE_SIZE) -offsetY)  and l_y < (j+1)*SQUARE_SIZE )
            ){

            C_Position* position = entities->GetComponent<C_Position>(l_entity, Component::Position);
            C_Collidable* collidable = entities->GetComponent<C_Collidable>(l_entity, Component::Collidable);
    
            collidable->SetPosition(position->GetFPosition());
            collidable->ResetCollisionFlags();
            m_squares[i][j].insert(l_entity);

            }
        }
    }
}


void S_Collision::Notify(const Message& l_message __attribute((unused)) ){

}

void S_Collision::HandleEvent(
    const std::set<unsigned int>& participians __attribute((unused)),
    const EntityEvent& l_events __attribute((unused))
){

}

//////////////////////////////////////////////////////////////////////////////////////


void S_Collision::MapCollisions(
    const EntityId& l_entity __attribute((unused))
    ,C_Position* l_pos __attribute((unused)),
     C_Collidable* l_col __attribute((unused))
)
    {
        /*
        unsigned int TileSize = m_gameMap->getTileSize();
        Collisions c;
        sf::FloatRect EntityAABB = l_col->GetCollidable();
        int FromX = floor(EntityAABB.left / TileSize);
        int ToX =   floor((EntityAABB.left + EntityAABB.width)/TileSize);
        int FromY = floor(EntityAABB.top / TileSize);
        int ToY =   floor((EntityAABB.top + EntityAABB.height)/TileSize);
        for (int x = FromX; x <= ToX; ++x){
            for (int y = FromY; y <= ToY; ++y){
              //  for (int l = 0; l < Sheet::Num_Layers; ++l){
                    Tile* t = m_gameMap->GetTile(x, y);
                    if (!t){ continue; }
                    //if (!t->m_solid){ continue; }
                    sf::FloatRect TileAABB(x*TileSize, y*TileSize,TileSize, TileSize);
                    sf::FloatRect Intersection;
                    EntityAABB.intersects(TileAABB, Intersection);
                    float S = Intersection.width * Intersection.height;
                    c.emplace_back(S, t->m_properties, TileAABB);
                break;
              //  }
            }
        }
        if (c.empty()){ return; }
        std::sort(c.begin(), c.end(),[](CollisionElement& l_1, CollisionElement& l_2){
                return l_1.m_area > l_2.m_area;
                });

        for (auto &col : c){
            EntityAABB = l_col->GetCollidable();
            if (!EntityAABB.intersects(col.m_tileBounds)){ continue; }
            float xDiff = (EntityAABB.left + (EntityAABB.width / 2))-(col.m_tileBounds.left + (col.m_tileBounds.width / 2));
            float yDiff = (EntityAABB.top + (EntityAABB.height / 2))-(col.m_tileBounds.top + (col.m_tileBounds.height / 2));
            float resolve = 0;
            if (std::abs(xDiff) > std::abs(yDiff)){
                if (xDiff > 0){
                    resolve = (col.m_tileBounds.left + TileSize) - EntityAABB.left;
                } else {
                    resolve = -((EntityAABB.left + EntityAABB.width) -col.m_tileBounds.left);
                }
            l_pos->MoveBy(resolve, 0);
            l_col->SetPosition(l_pos->GetFPosition());
            m_systemManager->AddEvent(l_entity,
                (EventID)EntityEvent::Colliding_X);
                l_col->CollideOnX();
            } else {
                if (yDiff > 0){
                    resolve = (col.m_tileBounds.top + TileSize) -EntityAABB.top;
                } else {
                    resolve = -((EntityAABB.top + EntityAABB.height) - col.m_tileBounds.top);
                }
            l_pos->MoveBy(0, resolve);
            l_col->SetPosition(l_pos->GetFPosition());
            m_systemManager->AddEvent(l_entity,
                (EventID)EntityEvent::Colliding_Y);
                l_col->CollideOnY();
            }

        }
        */
    }

void S_Collision::CheckOutOfBounds(C_Position* l_pos,C_Collidable* l_col)
    {
        unsigned int TileSize = m_gameMap->getTileSize();
        if (l_pos->GetFPosition().x < 0){
            l_pos->SetPosition(0.0f, l_pos->GetFPosition().y);
            l_col->SetPosition(l_pos->GetFPosition());
        } else if (l_pos->GetFPosition().x >m_gameMap->GetMapSize().x * TileSize){
        
            l_pos->SetPosition(m_gameMap->GetMapSize().x * TileSize,l_pos->GetFPosition().y);
            l_col->SetPosition(l_pos->GetFPosition());
        }

        if (l_pos->GetFPosition().y < 0){
            
            l_pos->SetPosition(l_pos->GetFPosition().x, 0.0f);
            l_col->SetPosition(l_pos->GetFPosition());
        } else if (l_pos->GetFPosition().y > m_gameMap->GetMapSize().y * TileSize){
            
            l_pos->SetPosition(l_pos->GetFPosition().x,m_gameMap->GetMapSize().y * TileSize);
            l_col->SetPosition(l_pos->GetFPosition());
        }
    }


/*

    Solution 1
    Divide whole space into overlapping squares and check for collision only 
    with circles that are in the same square. Squares need to overlap so 
    there won't be a problem when a circle moves from one square to another.

    Solution 2
    At the beginning distances between every pair of circles need to be 
    calculated.
    If the distance is small then these pair is stored in some list, and 
    we need to check for collision in every update.
    If the distance is big then we store after which update there can 
    be a collision (it can be calculated because we know the distance 
    and velocitites). It needs to be stored in some kind of 
    priority queue. After previously calculated number of updates 
    distance needs to be checked again and then we do the same procedure - 
    put it on the list or again in the priority queue.

    
    Solution 1
    Divide your space into rectangular cells that don't overlap. 
    So when you check one circle for collision you look for all 
    circles inside a cell that your first circle is, and look X cells 
    in each direction around. I've tried many values of X and found that 
    X=1 is the fastest solution. So you have to divide space into cells size 
    in each direction equal to:
    
    Divisor = SimulationBoxSize / MaximumCircleDiameter;
    CellSize = SimulationBoxSize / Divisor;
    
    Divisor should be bigger than 3, otherwise it will cause errors 
    (if it is too small, you should enlarge your simulation box).
    Then your algorithm will look like this:
    
        Put all circles inside the box
        Create cell structure and store indexes or pointers to circles 
        inside a cell (on array or on a list)
        Make a step in time (move everything) and update circles positions 
        inside on cells
        Look around every circle for collision. You should check one cell 
        around in every direction
        If there is a collision - do something
        Go to 3.
    
    If you will write it correctly then you would have something about O(N) 
    complexity, because maximum number of circles inside 9 cells (in 2D) or 
    27 cells (in 3D) is constant for any total number of circles.
    
    Solution 2
    Ususaly this is done like this:
    
        For each circle create a list of circles that are in distance R < 
        R_max, calculate time after which we should update lists (something 
        about T_update = R_max / V_max; where V_max is maximum current 
        velocity)
        Make a step in time
        Check distance of each circle with circles on its list
        If there is a collision - do something
        If current time is bigger then T_update, go to 1.
        Else go to 2.
    
    This solution with lists is very often improved by adding another 
    list with R_max_2 > R_max and with its own T_2 expiration time. 
    In this solution this second list is used to update the first list. 
    Of course after T_2 you have to update all lists which is O(N^2). 
    Also be carefull with this T and T_2 times, because if collision 
    can change velocity then those times would change. Also if you 
    introduce some foreces to your system, then it will also cause 
    velocity change.
    
    Solution 1+2 You can use lists for collision detection and cells 
    for updating lists. In one book it was written that this is the 
    best solution, but I think that if you create small cells 
    (like in my example) then solution 1 is better. But it is my opinion.
    
    Other stuff
    You can also do other things to improve speed of simulation:
    
        When you calculate distance 
        r = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + ...) 
        you don't have to do square root operation. 
        You can compare r^2 to some value - it's ok. 
        Also you don't have to do all (x1-x2)*(x1-x2) operations 
        (I mean, for all dimentions), because if x*x is bigger than some 
        r_collision^2 then all other y*y and so on, summed up, would be bigger.
        Molecular dynamics method is very easy to parallelise. 
        You can do it with threads or even on GPU. 
        You can calculate each distance in different thread. 
        On GPU you can easly create thousends of threads almost costless.
    
    For hard-spheres there is also effective algorithm that 
    doesn't do steps in time, but instead it looks for 
    nearest collision in time and jumps to this time and updates all positions. It can be good for not dense systems where collisions are not very probable.
*/   
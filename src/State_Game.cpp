#include "State_Game.h"
#include <iostream>
#include <stdio.h>


State_Game::State_Game(StateManager * l_stateManager) : BaseState(l_stateManager){

    #ifdef DEBUGG_RUN
        std::cout << "StateGame :: START" << std:: endl;
    #endif

}

State_Game::~State_Game(){
    #ifdef DEBUGG_RUN
        std::cout << "StateGame :: END" << std:: endl;
    #endif
    m_context = nullptr;
    m_stateMgr = nullptr;
}

void State_Game::OnCreate(){

    

    //m_increment = sf::Vector2f(0.5f,0.5f);

    EventManager * evMgr = m_stateMgr->GetContext()->m_eventManager;
    m_context = m_stateMgr->GetContext();
    sf::Vector2u size = m_stateMgr->GetContext()->m_wind->GetWindowSize();

    m_view.setSize(size.x,size.y);
    m_view.setCenter(size.x/2,size.y/2);
    m_viewSize = { (float) size.x, (float) size.y};
  //  m_view.zoom(0.6f);
    m_cameraMove = { m_viewSize.x/2 ,m_viewSize.y/2 };
    m_fullscreen = m_context->m_wind->IsFullscreen();
    MOVE_SPEED = m_viewSize.x / 20;

    m_context->m_wind->GetRenderWindow()->setView(m_view);
    m_gameMap = new Map(m_stateMgr->GetContext(), this);

    m_gameMap->LoadBitMap("res/maps/map1.bmp");
 //   m_gameMap->LoadMap("data/maps/map1.map");

    m_mapSize = m_gameMap->GetMapSize();
  //  evMgr-> AddCallback(StateType::Game, "Key_Escape", &State_Game::MainMenu,this);


    evMgr-> AddCallback(StateType::Game, "Key_Q", &State_Game::MainMenu, this);

    evMgr-> AddCallback(StateType::Game, "Key_P", &State_Game::Pause,this);
    evMgr-> AddCallback(StateType::Game, "Arrow_left", &State_Game::MoveCamera ,this);
    evMgr-> AddCallback(StateType::Game, "Arrow_right", &State_Game::MoveCamera ,this);
    evMgr-> AddCallback(StateType::Game, "Arrow_up", &State_Game::MoveCamera ,this);
    evMgr-> AddCallback(StateType::Game, "Arrow_down", &State_Game::MoveCamera ,this);

    evMgr-> AddCallback(StateType::Game, "EntityStop", &State_Game::StopEntity ,this);
    evMgr-> AddCallback(StateType::Game, "EntityMove", &State_Game::MoveEntity ,this);
    evMgr-> AddCallback(StateType::Game, "Key_S", &State_Game::SpawnEntity ,this);
    evMgr-> AddCallback(StateType::Game, "Key_R", &State_Game::Reinforcment, this);
    evMgr-> AddCallback(StateType::Game, "Key_T", &State_Game::Test, this);
    evMgr-> AddCallback(StateType::Game, "Key_A", &State_Game::ChoseAll, this);
    evMgr-> AddCallback(StateType::Game, "Key_D", &State_Game::KillEntity, this);
    evMgr-> AddCallback(StateType::Game, "Key_B", &State_Game::Print, this);
    evMgr-> AddCallback(StateType::Game, "Key_Z", &State_Game::SpawnHorse ,this);


    m_stateMgr->GetContext()->m_systemManager->GetSystem<S_Movement>(System::Movement)->SetMap(m_gameMap);
    m_stateMgr->GetContext()->m_systemManager->GetSystem<S_Collision>(System::Collision)->SetMap(m_gameMap);
    
    m_leftClickDown = false;

// CHOSE BOX PART

    m_posDown = sf::Vector2f( 0, 0 );
    m_posRelease = sf::Vector2f( 0, 0 );

    box.setOutlineColor(sf::Color::Red);
    box.setOutlineThickness(1);
    box.setFillColor(sf::Color::Transparent);

}


void State_Game::Print( EventDetails* l_details __attribute__((unused))){

    for( unsigned int i =0; i< m_mapSize.x; i++){
    for( unsigned int j =0; j< m_mapSize.y; j++){
        std::cout << m_stateMgr->GetContext()->m_gameMap->IsWalkable(j,i);
    }
        std::cout << std::endl;
    }
    

}

void State_Game::Test( EventDetails* l_details __attribute__((unused))){


    m_stateMgr->GetContext()->m_systemManager->GetSystem<S_Movement>(System::Movement)->TestTargetPosition();

}

void State_Game::KillEntity(EventDetails* l_details __attribute__((unused))){

    //m_stateMgr->GetContext()->m_systemManager->CheckEntityLists();


    std::vector<unsigned int> todelete = std::vector<unsigned int>();

     for( unsigned int i = 0; i < m_entities.size(); i++){

    //    std::cout << " CHECKING :: " << *itr << std::endl;

     //   auto itr = m_entities.begin();
        if( m_stateMgr->GetContext()->m_systemManager->GetSystem<S_Control>(System::Control)->IsActive(m_entities[i])){
        
     //       std::cout << m_entities[i] << " :  is Active " << std::endl; 

    //        m_stateMgr->GetContext()->m_systemManager->GetSystem<S_Collision>(System::Collision)->ReleaseUnit(*itr);
        //    m_stateMgr->GetContext()->m_systemManager->RemoveEntity(*itr);
     
            todelete.push_back(m_entities[i]);
        
        }
    }

    for( auto d : todelete){
    //      std::cout << "D: " <<  d << std::endl;
          m_stateMgr->GetContext()->m_systemManager->RemoveEntity(d);
    }

    m_entities = m_stateMgr->GetContext()->m_systemManager->GetList();



 //   }

    

 /*

    if( todelete.size() ){

    for( auto d : todelete){
          std::cout << "D: " <<  d << std::endl;
          m_stateMgr->GetContext()->m_systemManager->RemoveEntity(d);
    }



    std::vector<unsigned int> verryNotFine = std::vector<unsigned int>();

    bool flag = true;

    for( unsigned int i = 0; i < m_entities.size(); i++){
        flag = true;
 \

        for( unsigned int j = 0; j < todelete.size(); j++){
            if(  m_entities[i] == todelete[j]) {  flag = false; }
        }

        if( flag ) {verryNotFine.push_back( m_entities[i]);}

    }

    for( unsigned int i = 0; i < m_entities.size(); i++){
        std::cout << " E : "<< m_entities[i] << " ";
    }
    std::cout << std::endl;

    for( unsigned int i = 0; i < verryNotFine.size(); i++){
        std::cout << " V : "<< verryNotFine[i] << " ";
    }
    
    std::cout << std::endl;

    m_entities = verryNotFine;

    }else{

    for( unsigned int i = 0; i < m_entities.size(); i++){
        std::cout << " E : "<< m_entities[i] << " ";
    }   std::cout << std::endl;

}
*/
}

void State_Game::StopEntity(EventDetails* l_details __attribute__((unused))){

    m_leftClickDown  = true;

    m_posDown = { 
        m_context->m_eventManager->GetMousePos(m_context->m_wind->GetRenderWindow()).x 
         + m_cameraMove.x 
         - m_context->m_wind->GetWindowSize().x/2, 
        m_context->m_eventManager->GetMousePos(m_context->m_wind->GetRenderWindow()).y
         + m_cameraMove.y 
         - m_context->m_wind->GetWindowSize().y/2
 };

}

void State_Game::MoveEntity(EventDetails* l_details __attribute__((unused))){


    if( m_activeUnits.empty() ){
        return;
    }

    auto pos = m_context->m_eventManager->GetMousePos(

        m_context->m_wind->GetRenderWindow()

    );

    srand( time( NULL ) );

  //  for( unsigned int i = 0; i < m_entities.size(); i++){

     //   unsigned int t1 = std::rand()%( m_mapSize.x );
     //   unsigned int t2 = std::rand()%( m_mapSize.y );

   //     m_stateMgr->GetContext()->m_systemManager->GetSystem<S_Movement>(System::Movement)->SetTargetPosition(m_entities[i], (t1) *32, (t2) *32);
        
   // }
    
    float cor_x = (float)pos.x + m_cameraMove.x - m_context->m_wind->GetWindowSize().x/2;
    float cor_y = (float)pos.y + m_cameraMove.y - m_context->m_wind->GetWindowSize().y/2;

    cor_x = (int)(cor_x/Tile_Size) * Tile_Size + Tile_Size/2 ;
    cor_y = (int)(cor_y/Tile_Size) * Tile_Size + Tile_Size/2 ;

    //m_stateMgr->GetContext()->m_systemManager->GetSystem<S_Movement>(System::Movement)->SetTargetPosition(cor_x, cor_y);
    
     m_stateMgr->GetContext()->m_systemManager->GetSystem<S_Movement>(System::Movement)->MoveAsFormation(m_activeUnits, 0, sf::Vector2f(cor_x, cor_y));

}


void State_Game::MoveCamera(EventDetails* l_details){
    if( l_details->m_name == "Arrow_left"){

    /*
        m_cameraMove.x = 
        m_cameraMove.x <= m_viewSize.x/2 ?
        m_cameraMove.x : m_cameraMove.x - MOVE_SPEED;

        if(! (m_cameraMove.x >= m_viewSize.x - MOVE_SPEED)){
            m_cameraMove.x = m_viewSize.x/2;
        }

    */
    
        m_cameraMove.x = 
        m_cameraMove.x <= (float)m_viewSize.x/2 ?
        (float)m_viewSize.x/2 : m_cameraMove.x - MOVE_SPEED;
    
    }else if(l_details->m_name == "Arrow_right"){

        m_cameraMove.x =
        m_cameraMove.x >= (float)m_mapSize.x * 32 - m_viewSize.x/2 ?
        (float)m_mapSize.x * 32 - m_viewSize.x/2 : m_cameraMove.x + MOVE_SPEED;
    }else if(l_details->m_name == "Arrow_up"){
        m_cameraMove.y =
        m_cameraMove.y >= (float)m_mapSize.y * 32 - m_viewSize.y/2 ?
        (float)m_mapSize.y * 32 - m_viewSize.y/2 : m_cameraMove.y + MOVE_SPEED;
    }else if(l_details->m_name == "Arrow_down"){
        m_cameraMove.y = 
        m_cameraMove.y <= m_viewSize.y/2 ?
        m_viewSize.y/2 : m_cameraMove.y - MOVE_SPEED;
    }

}


void State_Game::OnDestroy(){

      EventManager * evMgr = m_stateMgr->GetContext()->m_eventManager;
      evMgr->RemoveCallback(StateType::Game, "Key_Escape");
      evMgr->RemoveCallback(StateType::Game, "Key_P");
      evMgr->RemoveCallback(StateType::Game, "Arrow_left");
      evMgr->RemoveCallback(StateType::Game, "Arrow_right");
      evMgr->RemoveCallback(StateType::Game, "Arrow_up");
      evMgr->RemoveCallback(StateType::Game, "Arrow_down");
      evMgr->RemoveCallback(StateType::Game, "EntityStop");
      evMgr->RemoveCallback(StateType::Game, "EntityMove");
      evMgr->RemoveCallback(StateType::Game, "Key_S");
      evMgr->RemoveCallback(StateType::Game, "Key_A");
      evMgr->RemoveCallback(StateType::Game, "Key_R");
      evMgr->RemoveCallback(StateType::Game, "Key_Q");
      evMgr->RemoveCallback(StateType::Game, "Key_D");
      evMgr->RemoveCallback(StateType::Game, "Key_B");
      evMgr->RemoveCallback(StateType::Game, "Key_Z");


      delete m_gameMap;
      m_gameMap = nullptr;

}

void State_Game::ChoseAll(EventDetails* l_details __attribute__((unused))){

    sf::Vector2f t = {
        static_cast<float>(m_mapSize.x*32),
        static_cast<float>(m_mapSize.y*32)
    };
    sf::Vector2f p = {0,0};

    m_activeUnits = m_stateMgr->GetContext()->m_systemManager->GetSystem<S_Control>(System::Control)->FindUnit( p, t );

}


void State_Game::Reinforcment(EventDetails* l_details __attribute__((unused))){

    for( int i = 0; i < 1000; i++){
    auto newEntity = m_context->m_entityManager->AddEntity("example.ent");

    
        m_entities = m_stateMgr->GetContext()->m_systemManager->GetList();

        auto cor_x = std::rand()%( m_mapSize.x -1 )*Tile_Size + Tile_Size/2;
        auto cor_y = std::rand()%( m_mapSize.y -1 )*Tile_Size + Tile_Size/2;

        m_stateMgr->GetContext()->m_systemManager->GetSystem<S_Movement>(System::Movement)->SetPosition(
            cor_x, 
            cor_y, 
            newEntity);

            m_stateMgr->GetContext()->m_systemManager->GetSystem<S_Collision>(System::Collision)->PlaceUnit(
        cor_x, 
        cor_y, 
        newEntity);
    };


}

void State_Game::SpawnEntity(EventDetails* l_details __attribute__((unused))){
    auto pos = m_context->m_eventManager->GetMousePos(
        
                m_context->m_wind->GetRenderWindow()
        
            );
            
    float cor_x = (float)pos.x + m_cameraMove.x - m_context->m_wind->GetWindowSize().x/2;
    float cor_y = (float)pos.y + m_cameraMove.y - m_context->m_wind->GetWindowSize().y/2;
    
    cor_x = (int)(cor_x/Tile_Size) * Tile_Size + Tile_Size/2 ;
    cor_y = (int)(cor_y/Tile_Size) * Tile_Size + Tile_Size/2 ;

    auto newEntity = m_context->m_entityManager->AddEntity("example.ent");

    m_entities = m_stateMgr->GetContext()->m_systemManager->GetList();

    m_stateMgr->GetContext()->m_systemManager->GetSystem<S_Movement>(System::Movement)->SetPosition(
        cor_x, 
        cor_y, 
        newEntity);

    m_stateMgr->GetContext()->m_gameMap->BlockTile((int)(cor_x/Tile_Size),(int)(cor_y/Tile_Size));


    m_stateMgr->GetContext()->m_systemManager->GetSystem<S_Collision>(System::Collision)->PlaceUnit(
        cor_x, 
        cor_y, 
        newEntity);
            
}

void State_Game::SpawnHorse(EventDetails* l_details __attribute__((unused))){
    auto pos = m_context->m_eventManager->GetMousePos(
        
                m_context->m_wind->GetRenderWindow()
        
            );
            
    float cor_x = (float)pos.x + m_cameraMove.x - m_context->m_wind->GetWindowSize().x/2;
    float cor_y = (float)pos.y + m_cameraMove.y - m_context->m_wind->GetWindowSize().y/2;
    
    cor_x = (int)(cor_x/Tile_Size) * Tile_Size + Tile_Size/2 ;
    cor_y = (int)(cor_y/Tile_Size) * Tile_Size + Tile_Size/2 ;

    auto newEntity = m_context->m_entityManager->AddEntity("H_i.ent");

    m_entities = m_stateMgr->GetContext()->m_systemManager->GetList();

    m_stateMgr->GetContext()->m_systemManager->GetSystem<S_Movement>(System::Movement)->SetPosition(
        cor_x, 
        cor_y, 
        newEntity);

    m_stateMgr->GetContext()->m_gameMap->BlockTile((int)(cor_x/Tile_Size),(int)(cor_y/Tile_Size));


    m_stateMgr->GetContext()->m_systemManager->GetSystem<S_Collision>(System::Collision)->PlaceUnit(
        cor_x, 
        cor_y, 
        newEntity);
            
}

void State_Game::Update(const sf::Time& l_time){
//m_context = m_stateMgr->GetContext();
UpdateCamera();

//Print(nullptr);

//std::cout << "ENTITies  :: " << m_entities.size() << std::endl;

m_gameMap->Update(l_time.asSeconds());
m_stateMgr->GetContext()->m_systemManager->Update(l_time.asSeconds());

// TRZEBA TO BEDZIE ZREFAKTORYZOWAc WPIERW

if( m_leftClickDown ){
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
        m_posRelease = { 
            m_context->m_eventManager->GetMousePos(m_context->m_wind->GetRenderWindow()).x 
             + m_cameraMove.x 
             - m_context->m_wind->GetWindowSize().x/2, 
            m_context->m_eventManager->GetMousePos(m_context->m_wind->GetRenderWindow()).y
             + m_cameraMove.y 
             - m_context->m_wind->GetWindowSize().y/2
     };
        
        box.setSize( 
                sf::Vector2f(
                    float(std::abs( m_posRelease.x - m_posDown.x )),
                    float(std::abs( m_posRelease.y - m_posDown.y ))
                )
        );

        sf::Vector2f boxPosition = sf::Vector2f(
            m_posRelease.x <= m_posDown.x ? m_posRelease.x : m_posDown.x,
            m_posRelease.y <= m_posDown.y ? m_posRelease.y : m_posDown.y
        );

        box.setPosition( boxPosition );


    }else{
          m_leftClickDown = false;
          m_posRelease = { 
            m_context->m_eventManager->GetMousePos(m_context->m_wind->GetRenderWindow()).x 
             + m_cameraMove.x 
             - m_context->m_wind->GetWindowSize().x/2, 
            m_context->m_eventManager->GetMousePos(m_context->m_wind->GetRenderWindow()).y
             + m_cameraMove.y 
             - m_context->m_wind->GetWindowSize().y/2
     };

        m_stateMgr->GetContext()->m_systemManager->GetSystem<S_Control>(System::Control)->ReleaseUnits( );
        m_activeUnits = m_stateMgr->GetContext()->m_systemManager->GetSystem<S_Control>(System::Control)->FindUnit(m_posDown, m_posRelease);

    }
}

if( m_fullscreen ^ m_context->m_wind->IsFullscreen()){
    sf::Vector2u size = m_stateMgr->GetContext()->m_wind->GetWindowSize();
    m_view.setSize(size.x,size.y);
 //   m_view.setCenter(size.x/2,size.y/2);
    m_viewSize = { (float) size.x, (float) size.y};
  //  m_view.zoom(0.6f);

    if( m_cameraMove.x < size.x/2 ){
        m_cameraMove.x = size.x/2;
    }else if ( m_cameraMove.x > m_mapSize.x*32 - size.x/2 ){
        m_cameraMove.x = m_mapSize.x*32 - size.x/2;
    }

    if( m_cameraMove.y < size.y/2 ){
        m_cameraMove.y = size.y/2;
    }else if ( m_cameraMove.y > m_mapSize.y*32 - size.y/2 ){
        m_cameraMove.y = m_mapSize.y*32 - size.y/2;
    }


    m_fullscreen = m_context->m_wind->IsFullscreen();
    MOVE_SPEED = m_viewSize.x / 20;
    #ifdef DEBUGG_RUN
    std::cout << "StateGame :: FULLSCREEN_TOGGLED :: CAMERA_MOVE_SPEED :: " << MOVE_SPEED << std:: endl;
    #endif
    }



}


void State_Game::UpdateCamera(){
    m_view.setCenter(m_cameraMove);
    m_context->m_wind->GetRenderWindow()->setView(m_view);
}
/*
void loadEntites( std::string l_path ){

    vector<unsigned int> m_playerEntities;


    std::string name;
    keystream >> name;
    int entityId = m_context->m_entityManager->AddEntity(name);
    if (entityId < 0){ continue; }
    if(name == "Champion"){ m_ChampionId = entityId; }
    C_Base* position = m_context->m_entityManager->GetComponent<C_Position>(entityId,Component::Position);
    if(position){ keystream >> *position; }


}


*/

void State_Game::Draw(){
 
    m_gameMap->Draw();
    m_stateMgr->GetContext()->m_systemManager->Draw(m_stateMgr->GetContext()->m_wind, 1);
    if(  m_leftClickDown  )m_stateMgr->GetContext()->m_wind->GetRenderWindow()->draw(box);

}

void State_Game::MainMenu(EventDetails* l_details __attribute__((unused))){
    m_stateMgr->SwitchTo(StateType::MainMenu);
}

void State_Game::Pause(EventDetails* l_details __attribute__((unused))){
   
     m_stateMgr->SwitchTo(StateType::Pause);

}

void State_Game::Activate(){
    #ifdef DEBUGG_RUN
    std::cout << "StateGame :: ACTIVATED" << std:: endl;
#endif


}

void State_Game::Deactivate(){
    #ifdef DEBUGG_RUN
    std::cout << "StateGame :: DEACTIVATED" << std:: endl;
#endif
}

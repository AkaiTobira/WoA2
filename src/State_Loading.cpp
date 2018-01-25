#include "State_Loading.h"

State_Loading::State_Loading(StateManager * l_stateManager) : BaseState(l_stateManager){
    #ifdef DEBUGG_RUN
    std::cout << "StateLoading :: START" << std:: endl;
#endif

}

State_Loading::~State_Loading(){
    m_stateMgr = nullptr;
    #ifdef DEBUGG_RUN
    std::cout << "StateLoading :: END" << std:: endl;
#endif

}

void State_Loading::OnCreate(){

    sf::Vector2u windowSize = m_stateMgr->GetContext()->m_wind->GetRenderWindow()->getSize();

        if(!m_stateMgr->GetContext()->m_fontManager->RequireResource("Titania")){
            #ifdef DEBUGG_RUN
                 std::cout << "StateMainMenu:: FONT_LOAD_FAIL" << std::endl; 
            #endif
        }

        m_font  = *m_stateMgr->GetContext()->m_fontManager->GetResource("Titania");
    m_complete = false;

    //tu cos trzeba poprawic



    m_text.setFont(m_font);
    m_text.setString({"Loading ..."});
    m_text.setCharacterSize(15);
    sf::FloatRect textRect = m_text.getLocalBounds();
    m_text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f - windowSize.y/2.0f + windowSize.y/15.0f );
    
   // std::cout << "S: " << windowSize.x / (float)3 << " " << (windowSize.y*(float)6)/(float)10 << std::endl; 
    
  //  float posx = windowSize.x / (float)3;
  //  float posy = (windowSize.y*(float)6)/(float)10;

    auto pos = m_stateMgr->GetContext()->m_wind->GetRenderWindow()->getView().getSize();

    std::cout << " LOADING :: " << pos.x/2 << " " << pos.y/2 << std::endl;

    m_text.setPosition( pos.x/2 - 10 ,  pos.y/2 -10  );

//    EventManager* evMgr = m_stateMgr->GetContext()->m_eventManager;
//   evMgr->AddCallback(StateType::Loading, "Loading_Continue", &State_Loading::Continue,this);
}

void State_Loading::OnDestroy(){    
      //  EventManager* evMgr = m_stateMgr->GetContext()->m_eventManager;
      //  evMgr->RemoveCallback(StateType::Loading, "Loading_Continue");
}

void State_Loading::Update(const sf::Time& l_time __attribute__((unused))){
    m_complete = true;
}    

void State_Loading::Draw(){
    sf::RenderWindow* window = m_stateMgr->GetContext()->m_wind->GetRenderWindow();
    window->draw(m_text);

    if(m_complete)Continue(nullptr);
}


void State_Loading::Continue(EventDetails* l_details __attribute__((unused))){
    m_stateMgr->SwitchTo(StateType::Game);
    Deactivate();
    
}

void State_Loading::Activate(){
    
        #ifdef DEBUGG_RUN
        std::cout << "StateLoading :: ACTIVATED" << std:: endl;
    #endif
}

void State_Loading::Deactivate(){
        #ifdef DEBUGG_RUN
        std::cout << "StateLoading :: DEACTIVATED" << std:: endl;
    #endif
}
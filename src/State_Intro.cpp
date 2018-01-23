#include "State_Intro.h"

State_Intro::State_Intro(StateManager * l_stateManager) : BaseState(l_stateManager){
    #ifdef DEBUGG_RUN
    std::cout << "StateIntro :: START" << std:: endl;
#endif

}

State_Intro::~State_Intro(){

    
    m_stateMgr = nullptr;
    #ifdef DEBUGG_RUN
    std::cout << "StateIntro :: END" << std:: endl;
#endif

}


void State_Intro::SetStart(){
      m_color = 0.0f;
      m_complete = false;
}

void State_Intro::ResetIntro(){
  //    sf::Vector2u windowSize = m_stateMgr->GetContext()->m_wind->GetRenderWindow()->getSize();
      m_timePassed = 0;
      m_animationID = 1;
      SetStart();
}

void State_Intro::OnCreate(){
    SetStart();

    sf::Vector2u windowSize = m_stateMgr->GetContext()->m_wind->GetRenderWindow()->getSize();

    std::string path = "res/fonts/Titania.ttf";

    #ifdef DEBUGG_RUN

    if ( ! m_stateMgr->GetContext()->m_textureManager->RequireResource("Intro")){
        
        std::cout << "StateIntro :: NO_INTRO_TEXTURE" << std::endl;
    
    }

    #endif

    if ( ! m_stateMgr->GetContext()->m_textureManager->RequireResource("Intro")){
        
      //  std::cout << "StateIntro :: NO_INTRO_TEXTURE" << std::endl;
    
    }

    m_introTexture = *m_stateMgr->GetContext()->m_textureManager->GetResource("Intro");
    m_introSprite.setTexture(m_introTexture);
    m_introSprite.setOrigin(m_introTexture.getSize().x/2.0f,m_introTexture.getSize().y/2.0f );

    m_introSprite.setPosition(windowSize.x/2.0f,windowSize.y/2.0f);

    //tu cos trzeba poprawic

    if(!m_font.loadFromFile(Utils::GetWorkingDirectory() + path)){
        #ifdef DEBUGG_RUN
        std::cout << "StateIntro :: FONT_LOAD_FAIL" << std::endl; 
    #endif
    }

    #ifdef DEBUGG_RUN

        std::cout << "StateIntro :: FONT :: " << path << std::endl;

    #endif


    m_text.setFont(m_font);
    m_text.setString({"Press SPACE to continue"});
    m_text.setCharacterSize(15);
    sf::FloatRect textRect = m_text.getLocalBounds();
    m_text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f - windowSize.y/2.0f + windowSize.y/15.0f );
    m_text.setPosition(windowSize.x/2.0f, windowSize.y/2.0f);
    m_complete = false;
    m_animationID = 1;

    EventManager* evMgr = m_stateMgr->GetContext()->m_eventManager;
    evMgr->AddCallback(StateType::Intro, "Intro_Continue", &State_Intro::Continue,this);
}

void State_Intro::OnDestroy(){

    EventManager* evMgr = m_stateMgr->GetContext()->m_eventManager;
    evMgr->RemoveCallback(StateType::Intro, "Intro_Continue");
  //  m_stateMgr->GetContext()->m_textureManager->ReleaseResource("Intro");
}

void State_Intro::AnimateLogo( sf::Sprite& l_sprite ){

if(  m_color > 255 ) m_color = 255;
    if(  m_color == 255 ) m_complete = true;

    if( m_color != 255 and !m_complete){
        m_color = m_color + 0.5f;
  //      m_timePassed += l_time.asSeconds();
        l_sprite.setColor(sf::Color(m_color,m_color,m_color));
    }else{
        if( m_color > 0){
        m_color = m_color - 1.0f;
  //      m_timePassed += l_time.asSeconds();
        l_sprite.setColor(sf::Color(m_color,m_color,m_color));

        }else{
            SetStart();
            m_animationID++;
        }

    }


}

void State_Intro::Update(const sf::Time& l_time __attribute__((unused))){

    switch(m_animationID){
        case 1:
            AnimateLogo( m_introSprite );
        break;

        // CAN BE ADDED HERE AS A NEXT CASES

        case 2:
            Continue(nullptr);
        break;
        default:
        break;
    }

}

void State_Intro::Draw(){
    sf::RenderWindow* window = m_stateMgr->GetContext()->m_wind->GetRenderWindow();
    window->draw(m_introSprite);
    window->draw(m_text);

}

void State_Intro::Continue(EventDetails* l_details __attribute__((unused))){

        ResetIntro();
        m_stateMgr->SwitchTo(StateType::MainMenu);
      //  m_stateMgr->Remove(StateType::Intro);
      Deactivate();

}

void State_Intro::Activate(){

    #ifdef DEBUGG_RUN
    std::cout << "StateIntro :: ACTIVATED" << std:: endl;
#endif
}
void State_Intro::Deactivate(){
    #ifdef DEBUGG_RUN
    std::cout << "StateIntro :: DEACTIVATED" << std:: endl;
#endif
}

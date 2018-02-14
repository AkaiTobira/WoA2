#include "State_Options.h"

State_Options::State_Options(StateManager * l_stateManager) : BaseState(l_stateManager){std::cout << "[OPTIONS: ININTIATE]" << std::endl;}

State_Options::~State_Options(){
    std::cout << "[OPTIONS: DESTROYED] " << std::endl;
    m_stateMgr = nullptr;
}



void State_Options::OnCreate(){
        if(!m_stateMgr->GetContext()->m_fontManager->RequireResource("Titania")){
            #ifdef DEBUGG_RUN
                 std::cout << "StateMainMenu:: FONT_LOAD_FAIL" << std::endl; 
            #endif
        }
     EventManager * evMgr = m_stateMgr->GetContext()->m_eventManager;
        m_font  = *m_stateMgr->GetContext()->m_fontManager->GetResource("Titania");
       m_text.setFont(m_font);
       m_text.setString(sf::String("OPTIONS:"));
       m_text.setCharacterSize(18);


       m_middle = m_stateMgr->GetContext()->m_wind->GetRenderWindow()->getSize().x;

       sf::FloatRect textRect = m_text.getLocalBounds();
       m_text.setOrigin(textRect.left +textRect.width/2.0f, textRect.top + textRect.height/2.0f);
       m_text.setPosition(m_middle/2.0f - 150.0f,70);

       m_buttonSize = sf::Vector2f(150.0f,32.0f);
       m_buttonPos = sf::Vector2f(m_middle/2.0f,200);
       m_buttonPadding = 4;

       std::string str[3];
       str[0] = "FullScreen";
       str[1] = "ON";
       str[2] = "OFF";

       for( int i = 0; i <=NUMBER_OF_OPTIONS; ++i){


           sf::Vector2f ButtonPosition(m_buttonPos.x -50.0f , m_buttonPos.y + (i*(m_buttonSize.y +m_buttonPadding)));
           m_rects[i].setSize(m_buttonSize);
           m_rects[i].setFillColor(sf::Color(64,64,64));

           m_rects[i].setOrigin(m_buttonSize.x/2.0f, m_buttonSize.y/2.0f);
           m_rects[i].setPosition(ButtonPosition);

           m_labels[i].setFont(m_font);
           m_labels[i].setString(sf::String(str[1]));
           m_labels[i].setCharacterSize(12);

           sf::FloatRect rect = m_labels[i].getLocalBounds();
           m_labels[i].setOrigin(rect.left + rect.width/2.0f, rect.top + rect.height/2.0f);
           m_labels[i].setPosition(ButtonPosition);
  
  
       }

        evMgr-> AddCallback(StateType::Options, "Key_Q", &State_Options::MainMenu, this);
    //   m_stateMgr->SwitchTo(StateType::Options);
}

void State_Options::MainMenu(EventDetails* l_details __attribute__((unused))){
    m_stateMgr->SwitchTo(StateType::MainMenu);
}

void State_Options::OnDestroy(){
    EventManager * evMgr = m_stateMgr->GetContext()->m_eventManager;
    evMgr->RemoveCallback(StateType::Options, "Key_Q");
}

void State_Options::Activate(){
    std::cout << "[OPTION: ACTIVE]" <<std::endl;
/*
    if(m_stateMgr->HasState(StateType::Game) && m_labels[0].getString() == "PLAY"){
        m_labels[0].setString(sf::String("RESUME"));
        sf::FloatRect rect = m_labels[0].getLocalBounds();
        m_labels[0].setOrigin(rect.left + rect.width/2.0f, rect.top + rect.height/2.0f);
    }
    */
}

void State_Options::Deactivate(){
    std::cout << "[OPTION: DEACTIVE]" <<std::endl;
}

void State_Options::MouseClick(EventDetails* l_details){
    sf::Vector2i mousePos = l_details->m_mouse;

    float halfX = m_buttonSize.x/2.0f;
    float halfY = m_buttonSize.y/2.0f;

    for( int i = 0; i < 3; ++i){

        if( (mousePos.x >= m_rects[i].getPosition().x - halfX) && (mousePos.x <= m_rects[i].getPosition().x + halfX) && (mousePos.y >= m_rects[i].getPosition().y - halfY) && (mousePos.y <= m_rects[i].getPosition().y + halfY)){
           switch(i){
               case 0:
                     m_stateMgr->SwitchTo(StateType::Game);
               break;
               case 1:
                     m_stateMgr->SwitchTo(StateType::Intro);
               break;
               case 2:
                     m_stateMgr->GetContext()->m_wind->Close(nullptr);
               break;
               default:
               break;
           }
        }
    }
}

void State_Options::Draw(){
    sf::RenderWindow* window = m_stateMgr->GetContext()->m_wind->GetRenderWindow();
 
    window->draw(m_text);
   
   
    for( int i = 0; i <NUMBER_OF_OPTIONS; ++i){
        window->draw(m_rects[i]);
        window->draw(m_labels[i]);
    }
    

    window = nullptr;
}

void State_Options::Update(const sf::Time& l_time __attribute((unused))){
}
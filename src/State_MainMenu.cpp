#include "State_MainMenu.h"

State_MainMenu::State_MainMenu(StateManager * l_stateManager) 
:BaseState(l_stateManager){

    #ifdef DEBUGG_RUN
        std::cout << "StateMainMenu :: START" << std:: endl;
    #endif
}

State_MainMenu::~State_MainMenu(){
    
    #ifdef DEBUGG_RUN
        std::cout << "StateMainMenu :: END" << std:: endl;
    #endif
}

void State_MainMenu::Quit(EventDetails* l_details __attribute__((unused))){
    m_stateMgr->GetContext()->m_wind->Close(nullptr);
}

void State_MainMenu::OnCreate(){


        #ifdef DEBUGG_RUN
            std::cout << "StateMainMenu :: FONT :: " << path <<  std:: endl;
        #endif

        if(!m_stateMgr->GetContext()->m_fontManager->RequireResource("Titania")){
            #ifdef DEBUGG_RUN
                 std::cout << "StateMainMenu:: FONT_LOAD_FAIL" << std::endl; 
            #endif
        }

        m_font  = *m_stateMgr->GetContext()->m_fontManager->GetResource("Titania");

       m_text.setFont(m_font);
       m_text.setString(sf::String("MAIN MENU: "));
       m_text.setCharacterSize(18);

       m_windowSize = m_stateMgr->GetContext()->m_wind->GetWindowSize();
       m_fullscreen = false;

       sf::FloatRect textRect = m_text.getLocalBounds();
       m_text.setOrigin(textRect.left +textRect.width/2.0f, textRect.top + textRect.height/2.0f);
       m_text.setPosition(m_windowSize.x/2 , m_windowSize.y/10);

       m_buttonSize = sf::Vector2f(300.0f,32.0f);
       m_buttonPos = sf::Vector2f(m_windowSize.x/2,m_windowSize.y/10 + 100);
       m_buttonPadding = 4;

       std::string str[NUMBER_OF_LABELS];
       str[0] = "PLAY";
       str[1] = "CREDITS";
       str[2] = "OPTIONS";
       str[3] = "EXIT";

       for( int i = 0; i <NUMBER_OF_LABELS; ++i){
           sf::Vector2f ButtonPosition(m_buttonPos.x, m_buttonPos.y + (i*(m_buttonSize.y +m_buttonPadding)));
           m_rects[i].setSize(m_buttonSize);
           m_rects[i].setFillColor(sf::Color::Red);

           m_rects[i].setOrigin(m_buttonSize.x/2.0f, m_buttonSize.y/2.0f);
           m_rects[i].setPosition(ButtonPosition);

            m_labels[i].setFont(m_font);
            m_labels[i].setString(sf::String(str[i]));
            m_labels[i].setCharacterSize(12);

            sf::FloatRect rect = m_labels[i].getLocalBounds();
            m_labels[i].setOrigin(rect.left + rect.width/2.0f, rect.top + rect.height/2.0f);
            m_labels[i].setPosition(ButtonPosition);
        }

        m_stateMgr->SwitchTo(StateType::MainMenu);
        EventManager* evMgr = m_stateMgr->GetContext()->m_eventManager;
   //     evMgr->AddCallback(StateType::MainMenu, "Mouse_Left", &State_MainMenu::MouseClick,this);
    //    evMgr->AddCallback(StateType::MainMenu, "Key_Quit", &State_MainMenu::Quit,this);


        GUI_Manager* gui = m_stateMgr->GetContext()->m_guiManager;
        gui->LoadInterface(StateType::MainMenu,"MainMenu.interface", "MainMenu");
        gui->GetInterface(StateType::MainMenu,"MainMenu")->SetPosition(sf::Vector2f(m_windowSize.x/2 -gui->GetInterface(StateType::MainMenu,"MainMenu")->GetSize().x/2 , m_windowSize.y/10));

        EventManager* eMgr = m_stateMgr->GetContext()->m_eventManager;
        eMgr->AddCallback(StateType::MainMenu,"MainMenu_Play", &State_MainMenu::Play, this);
        eMgr->AddCallback(StateType::MainMenu,"MainMenu_Quit", &State_MainMenu::Quit, this);
        eMgr->AddCallback(StateType::MainMenu,"MainMenu_Credits", &State_MainMenu::Credits, this);
        eMgr->AddCallback(StateType::MainMenu,"MainMenu_Options", &State_MainMenu::Options, this);
}

void State_MainMenu::Play(EventDetails* l_details){
    m_stateMgr->SwitchTo(StateType::Loading);
   // m_stateMgr->SwitchTo(StateType::Intro);
}


void State_MainMenu::Credits(EventDetails* l_details){
    //m_stateMgr->SwitchTo(StateType::Loading);
    m_stateMgr->SwitchTo(StateType::Intro);
}

void State_MainMenu::Options(EventDetails* l_details){
    //m_stateMgr->SwitchTo(StateType::Loading);
    m_stateMgr->SwitchTo(StateType::Options);
}

void State_MainMenu::OnDestroy(){
    EventManager* evMgr = m_stateMgr->GetContext()->m_eventManager;
 //   evMgr->RemoveCallback(StateType::MainMenu, "Mouse_Left");
  //  evMgr->RemoveCallback(StateType::MainMenu, "Key_Quit"  );

  //  m_stateMgr->GetContext()->m_guiManager->RemoveInterface(StateType::MainMenu, "MainMenu");

    evMgr->RemoveCallback(StateType::MainMenu, "MainMenu_Play");
    evMgr->RemoveCallback(StateType::MainMenu, "MainMenu_Quit"  );
    evMgr->RemoveCallback(StateType::MainMenu, "MainMenu_Credits"  );
    evMgr->RemoveCallback(StateType::MainMenu, "MainMenu_Options"  );
}

void State_MainMenu::Activate(){
    #ifdef DEBUGG_RUN
        std::cout << "StateMainMenu :: ACTIVATED" <<  std:: endl;
    #endif


   // auto& play = *m_stateMgr->GetContext()->m_guiManager->
 //   GetInterface(StateType::MainMenu, "MainMenu")->GetElement("Play");
/*    if (m_stateMgr->HasState(StateType::Game)){
    // Resume
        play.SetText("Resume");
    } else {
// Play
        play.SetText("Play");
    }
*/
    if(m_stateMgr->HasState(StateType::Game) && m_labels[0].getString() == "PLAY"){
        m_labels[0].setString(sf::String("RESUME"));
        sf::FloatRect rect = m_labels[0].getLocalBounds();
        m_labels[0].setOrigin(rect.left + rect.width/2.0f, rect.top + rect.height/2.0f);
    }
}

void State_MainMenu::Deactivate(){
    #ifdef DEBUGG_RUN
        std::cout << "StateMainMenu :: DEACTIVATED" <<  std:: endl;
    #endif
}

void State_MainMenu::MouseClick(EventDetails* l_details){
    sf::Vector2i mousePos = l_details->m_mouse;

    float halfX = m_buttonSize.x/2.0f;
    float halfY = m_buttonSize.y/2.0f;

    for( int i = 0; i < NUMBER_OF_LABELS; ++i){

        if( (mousePos.x >= m_rects[i].getPosition().x - halfX) && (mousePos.x <= m_rects[i].getPosition().x + halfX) && (mousePos.y >= m_rects[i].getPosition().y - halfY) && (mousePos.y <= m_rects[i].getPosition().y + halfY)){
           switch(i){
               case 0:
                     m_stateMgr->SwitchTo(StateType::Loading);
               break;
               case 1:
        //             m_stateMgr->SwitchTo(StateType::Intro);
               break;
               case 2:
        //            m_stateMgr->SwitchTo(StateType::Options);
               break;
               case 3:
        //             Quit(nullptr);
               break;
               default:
               break;
           }
        }
    }
}

void State_MainMenu::Draw(){

}

void State_MainMenu::Update(const sf::Time& l_time __attribute__((unused))){

    if( m_stateMgr->GetContext()->m_wind->IsFullscreen() ^ m_fullscreen ){

        m_fullscreen = m_stateMgr->GetContext()->m_wind->IsFullscreen();
        m_windowSize = m_stateMgr->GetContext()->m_wind->GetWindowSize();

        m_stateMgr->GetContext()->m_guiManager->GetInterface(StateType::MainMenu,"MainMenu")->SetPosition(sf::Vector2f(m_windowSize.x/2 -m_stateMgr->GetContext()->m_guiManager->GetInterface(StateType::MainMenu,"MainMenu")->GetSize().x/2 , m_windowSize.y/10));
    }
}

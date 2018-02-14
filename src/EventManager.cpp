#include "EventManager.h"

void Binding::BindEvent( EventType l_type, EventInfo l_info)
{
    m_events.emplace_back(l_type,l_info);
}

EventManager::EventManager(std::string l_path ): m_path(l_path), m_hasFocus(true)
{

    #ifdef DEBUGG_RUN
    std::cout << "EventManager :: START :: " << m_path << std::endl;
    #endif

    LoadBindings();
}

EventManager::~EventManager()
{

    for ( auto & itr : m_bindings )
    {
        delete itr.second;
        itr.second = nullptr;
    }

    #ifdef DEBUGG_RUN
    std::cout << "EventManager :: END" << std::endl;
    #endif
}

void EventManager::LoadBindings()
{
    std::string delimeter = ":";

    std::ifstream bindings;



    bindings.open(Utils::GetWorkingDirectory() + m_path);
    if( !bindings.is_open())
    {
        std::cout << "! Failed loading : " << m_path << std::endl;
        return;
    }

    std::string line;
    while( std::getline(bindings, line))
    {
        std::stringstream keystream(line);
        std::string callbackName = "";
        keystream >> callbackName;

        Binding* bind = new Binding(callbackName);
        while( !keystream.eof() )
        {
            std::string keyval = "";
            keystream >> keyval;

         //   std::cout << keyval << std::endl;

            int start = 0;
            auto end = keyval.find(delimeter);
            if( end == std::string::npos)
            {
                delete bind;
                bind = nullptr;
                break;
            }


            EventType type = EventType(stoi(keyval.substr(start, end-start)));

        //    std::cout << (unsigned int)type << std::endl;

            EventInfo eventInfo;
            if (
                type == EventType::GUI_Click   ||
                type == EventType::GUI_Release ||
                type == EventType::GUI_Hover   ||
                type == EventType::GUI_Leave
                ){


                    std::cout << " Loaded GUI EVENT "<< std::endl;

                    start = end + delimeter.length();
                    end = keyval.find(delimeter, start);
                    std::string window = keyval.substr(start, end - start);
                    std::string element;
                    if (end != std::string::npos){
                        start = end + delimeter.length();
                        end = keyval.length();
                        element = keyval.substr(start, end);
                    }
                char* w = new char[window.length() + 1]; // +1 for \0
                char* e = new char[element.length() + 1];
                // Size in bytes is the same as character length.1 char = 1B.
                
                //strcpy(w, window.length() + 1, window.c_str());
                
                strcpy(w,window.c_str());
                strcpy(e,element.c_str());

                std::cout <<"EVENTMANAGR :: " <<w << " " << e << std::endl;

                //strcpy_s(w, window.length() + 1, window.c_str());
                //strcpy_s(e, element.length() + 1, element.c_str());
                
                eventInfo.m_gui.m_interface = w;
                eventInfo.m_gui.m_element = e;
            } else {
                int code = stoi(keyval.substr(end + delimeter.length(),
                keyval.find(delimeter,end + delimeter.length())));
                eventInfo.m_code = code;
            }
            bind->BindEvent(type, eventInfo);

        }

        if( !AddBinding(bind) )
        {
            delete bind;
        }
        bind = nullptr;
    }


    #ifdef DEBUGG_RUN
    std::cout << "EventManager :: BINDINGS_LOADED :: " << m_bindings.size() << std::endl;
    #endif

    bindings.close();
}

bool EventManager::AddBinding( Binding *l_binding)
{
    if( m_bindings.find(l_binding->m_name) != m_bindings.end())
        return false;

    return m_bindings.emplace(l_binding->m_name, l_binding).second;
}

bool EventManager::RemoveBinding(std::string l_name)
{
    auto itr = m_bindings.find(l_name);
    if( itr == m_bindings.end())
    {
        return false;
    }
    delete itr->second;
    m_bindings.erase(itr);
    return true;

}

void EventManager::SetFocus( bool const& l_focus)
{
    m_hasFocus = l_focus;
}


void EventManager::setCurrentState(StateType l_type)
{
    m_currentState = l_type;
}

void EventManager::HandleEvent(GUI_Event& l_event){
    for (auto &b_itr : m_bindings){
    Binding* bind = b_itr.second;
    for (auto &e_itr : bind->m_events)
    {
        if (e_itr.first != EventType::GUI_Click &&
            e_itr.first != EventType::GUI_Release &&
            e_itr.first != EventType::GUI_Hover &&
            e_itr.first != EventType::GUI_Leave)
        { continue; }
        if ((e_itr.first == EventType::GUI_Click &&
            l_event.m_type != GUI_EventType::Click) ||
            (e_itr.first == EventType::GUI_Release &&
            l_event.m_type != GUI_EventType::Release) ||
            (e_itr.first == EventType::GUI_Hover &&
            l_event.m_type != GUI_EventType::Hover) ||
            (e_itr.first == EventType::GUI_Leave &&
            l_event.m_type != GUI_EventType::Leave))
        { continue; }
        if (strcmp(e_itr.second.m_gui.m_interface,
                    l_event.m_interface) ||
             strcmp(e_itr.second.m_gui.m_element, l_event.m_element))
        { continue; }
        bind->m_details.m_guiInterface = l_event.m_interface;
        bind->m_details.m_guiElement = l_event.m_element;
        ++(bind->c);
        }
    }
}
void EventManager::HandleEvent( sf::Event& l_event)
{
    for( auto & b_itr : m_bindings)
    {
        Binding*bind = b_itr.second;
        for( auto &e_itr : bind->m_events)
        {
            EventType sfmlEvent = (EventType)l_event.type;
            if (e_itr.first == EventType::GUI_Click ||
                e_itr.first == EventType::GUI_Release ||
                e_itr.first == EventType::GUI_Hover ||
                e_itr.first == EventType::GUI_Leave)
            {
                continue;
            }   


            if( e_itr.first != sfmlEvent )
            {
                continue;
            }
            if( sfmlEvent == EventType::KeyDown || sfmlEvent == EventType::KeyUp)
            {
                if(e_itr.second.m_code == l_event.key.code)
                {
                    if( bind->m_details.m_keyCode != -1)
                    {
                        bind->m_details.m_keyCode = e_itr.second.m_code;
                    }
                    ++(bind->c);
                    break;
                }

            }
            else if( sfmlEvent == EventType::MButtonDown || sfmlEvent == EventType::MButtonUp )
            {
                if(e_itr.second.m_code == l_event.mouseButton.button)
                {
                    bind->m_details.m_mouse.x = l_event.mouseButton.x;
                    bind->m_details.m_mouse.y = l_event.mouseButton.y;

                    if ( bind->m_details.m_keyCode != 1)
                    {
                        bind->m_details.m_keyCode = e_itr.second.m_code;
                    }
                    ++(bind->c);
                    break;
                }
                
            }
            else
            {
                if( sfmlEvent == EventType::MouseWheel)
                {
                    bind->m_details.m_mouseWheelDelta = l_event.mouseWheel.delta;
                }
                else if( sfmlEvent == EventType::WindowResized)
                {
                    bind->m_details.m_size.x = l_event.size.width;
                    bind->m_details.m_size.y = l_event.size.height;
                }
                else if ( sfmlEvent == EventType::TextEntered)
                {
                    bind->m_details.m_textEntered = l_event.text.unicode;
                }
                ++(bind->c);

            }
        }
    }
}

void EventManager::Update()
{
    if(!m_hasFocus)
    {
        return;
    }

    for( auto &b_itr : m_bindings )
    {
        Binding* bind = b_itr.second;

        for ( auto &e_itr : bind ->m_events)
        {
            switch(e_itr.first)
            {
            case (EventType::Keyboard):
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(e_itr.second.m_code)))
                {
                    if( bind->m_details.m_keyCode != -1)
                    {
                        bind->m_details.m_keyCode = e_itr.second.m_code;
                    }
                    ++(bind->c);
                }
                break;
            case (EventType::Mouse):
                if( sf::Mouse::isButtonPressed( sf::Mouse::Button(e_itr.second.m_code)))
                {
                    if( bind->m_details.m_keyCode != -1)
                    {
                        bind->m_details.m_keyCode = e_itr.second.m_code;
                    }
                    ++(bind->c);
                }
                break;
            case (EventType::Joystick):
                break;
            default:
                break;

            }
        }

        if( bind->m_events.size() == bind->c)
        {
            auto stateCallbacks = m_callbacks.find(m_currentState);
            auto otherCallbacks = m_callbacks.find(StateType(0));
            if( stateCallbacks != m_callbacks.end() )
            {
                auto callItr = stateCallbacks->second.find(bind->m_name);
                if( callItr != stateCallbacks->second.end())
                {
                    callItr->second(&bind->m_details);
                }
            }

            if( otherCallbacks != m_callbacks.end() )
            {
                auto callItr = otherCallbacks->second.find(bind->m_name);
                if( callItr != otherCallbacks->second.end())
                {
                    callItr->second(&bind->m_details);
                }
            }
            bind->c = 0;
            bind->m_details.Clear();

        }

    }

}


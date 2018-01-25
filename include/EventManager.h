#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H



#include "Utilities.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>
#include <utility>
#include <unordered_map>
#include <functional>
#include <sstream>
#include <fstream>
#include "GUI_Events.h"
//#include "GUI.h"

//#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <stdlib.h>
//#include <string>
//#include <string.h>
//#include <stdlib.h>
//#include <stdio.h>
//#include <errno.h>
#include <cstring>

enum class EventType
{
    KeyDown = sf::Event::KeyPressed,
    KeyUp = sf::Event::KeyReleased,
    MButtonDown = sf::Event::MouseButtonPressed,
    MButtonUp = sf::Event::MouseButtonReleased,
    MouseWheel = sf::Event::MouseWheelMoved,
    WindowResized = sf::Event::Resized,
    GainedFocus = sf::Event::GainedFocus,
    LostFocus = sf::Event::LostFocus,
    MouseEntered = sf::Event::MouseEntered,
    MouseLeft = sf::Event::MouseLeft,
    Closed = sf::Event::Closed,
    TextEntered = sf::Event::TextEntered,
    Keyboard = sf::Event::Count +1, 
    Mouse, 
    Joystick,
    GUI_Click, 
    GUI_Release, 
    GUI_Hover, 
    GUI_Leave
};


struct EventInfo
{
    EventInfo()
    {
        m_code = 0;
    }
    EventInfo(int l_event)
    {
        m_code = l_event;
    }
    EventInfo(GUI_Event l_guiEvent){
        m_gui = l_guiEvent;
    }
    union
    {
        int m_code;
        GUI_Event m_gui;
    };
};

using Events = std::vector<std::pair<EventType,EventInfo>>;

struct EventDetails
{
    EventDetails(const std::string& l_bindName) : m_name(l_bindName)
    {
        Clear();
    }
    std::string m_name;

    sf::Vector2i m_size;
    sf::Uint32 m_textEntered;
    sf::Vector2i m_mouse;
    int m_mouseWheelDelta;
    int m_keyCode;
    std::string m_guiInterface; // GUI interface name.
    std::string m_guiElement; // GUI element name.
    GUI_EventType m_guiEvent; // GUI event type.

    void Clear()
    {

        m_size = sf::Vector2i(0,0);
        m_textEntered =0;
        m_mouse = sf::Vector2i(0,0);
        m_mouseWheelDelta = 0;
        m_keyCode = -1;
        m_guiInterface = "";
        m_guiElement = "";
        m_guiEvent = GUI_EventType::None;

    }



};


class Binding
{
public:
    std::string m_name;
    EventDetails m_details;
    unsigned int c;
    Events m_events;

    Binding(const std::string& l_name): 
    m_name(l_name), 
    m_details(l_name), 
    c(0) {}

    ~Binding(){
    // GUI portion.
    for (auto itr = m_events.begin();
        itr != m_events.end(); ++itr)
            {
            if (
                itr->first == EventType::GUI_Click ||
                itr->first == EventType::GUI_Release ||
                itr->first == EventType::GUI_Hover ||
                itr->first == EventType::GUI_Leave
                ){
                    delete [] itr->second.m_gui.m_interface;
                    delete [] itr->second.m_gui.m_element;
            }
        }
    }


    void BindEvent( EventType l_type, EventInfo l_info = EventInfo());
};


using Bindings = std::unordered_map<std::string, Binding*>;

using CallbackContainer = std::unordered_map<std::string, std::function<void(EventDetails*)>>;

enum class StateType
{
    Intro = 1, MainMenu, Game, Pause, GameOver, Credits, Loading ,Options
};


struct EnumClassHash
{
    template <typename T>
    std::size_t operator()(T t) const
    {
        return static_cast<std::size_t>(t);
    }
};

using Callbacks = std::unordered_map<StateType, CallbackContainer, EnumClassHash>;



class EventManager
{
public:
    EventManager(std::string l_path);
    ~EventManager();

    bool AddBinding(Binding *l_binding);
    bool RemoveBinding(std::string l_name);
    void SetFocus(const bool& l_focus);

    template<class T>
    bool AddCallback(StateType l_state, const std::string & l_name, void (T::*l_func)(EventDetails*), T* l_instance)
    {

        auto itr = m_callbacks.emplace(l_state, CallbackContainer()).first;

        auto temp = std::bind( l_func, l_instance, std::placeholders::_1);
        return itr->second.emplace(l_name, temp).second;
    }
    void setCurrentState(StateType l_type);
    bool RemoveCallback(StateType l_state, const std::string & l_name)
    {
        auto itr = m_callbacks.find(l_state);
        if( itr == m_callbacks.end() )
        {
            return false;
        }
        auto itr2 = itr->second.find(l_name);
        if( itr2 == itr->second.end() )
        {
            return false;
        }
        itr->second.erase(l_name);
        return true;
    }

    void HandleEvent(sf::Event& l_event);
    void HandleEvent(GUI_Event& l_event);
    void Update();

    sf::Vector2i GetMousePos( sf:: RenderWindow* l_wind = nullptr)
    {
        return (l_wind ? sf::Mouse::getPosition(*l_wind) : sf::Mouse::getPosition());
    }

protected:

private:
    void LoadBindings();
    std::string m_path;
    Bindings m_bindings;
    Callbacks m_callbacks;
    bool m_hasFocus;
    StateType m_currentState;

};



#endif // EVENTMANAGER_H

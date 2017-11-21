#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>

#if not defined EVENTMANAGER_H
#include "EventManager.h"
#endif

class Window
{
public:
    Window(const std::string & l_title = "WINDOW", const sf::Vector2u & l_size = sf::Vector2u(640,480));
    ~Window();
    Window(const Window & l_wind ) = default;

    void BeginDraw(); // Clear window;
    void EndDraw(); // Display changes;

    void Update();

    bool IsDone();
    bool IsFullscreen();
    bool isFocused();
    EventManager* GetEventManager();
    sf::FloatRect GetViewSpace();
    void ToggleFullscreen(EventDetails* l_details);
    void Close(EventDetails* l_details = nullptr);
    sf::Vector2u GetWindowSize();

    void ToggleFullscreen(); // Przelacz fullscren;
    void Draw(sf::Drawable & l_drawable);
    void Close();
    sf::RenderWindow* GetRenderWindow();
    void SetViewMove(float x,float y);
    sf::Vector2f GetViewSize();
    sf::Vector2f GetMoveWiew();
    sf::Vector2f GetViewPos();

private:

    void Setup(const std::string & l_title, const sf::Vector2u & l_size);
    void Destroy();
    void Create();

    sf::RenderWindow m_window;
    EventManager m_eventManager;
    sf::Vector2u m_windowSize;
    sf::Vector2u m_windowDefaultSize;
    sf::Vector2f m_moveView;
    std::string m_windowTitle;
    std::vector<sf::VideoMode> m_mods;

    bool m_isDone;
    bool m_isFullscreen;
    bool m_isFocused;
};

#endif

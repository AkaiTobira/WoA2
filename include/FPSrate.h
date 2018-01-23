#ifndef EVENTMANAGER_H
#include "SharedElements.h"
#endif

#ifndef FPSRATE_H
#define FPSRATE_H

#include <string>
#include <iostream>
#include <sstream>
#include <numeric>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include "Window.h"
#include "Utilities.h"
//#include <stdio.h>

class FPS
{
public:

    FPS( std::string l_path );
    FPS( int l_charSize, int l_width, sf::Vector2f  l_screePos);
    ~FPS();

    void Setup( int l_charSize, int l_width, sf::Vector2f  l_screePos);
    void SetVisible(const bool & l_visible);
    void Update(const sf::Time & l_elapsed);
    void SetWindow(Window* l_window);
    void Draw();
    int Evaluate();

private:
    const unsigned int NUMBER_OF_COUTS = 100;

    sf::RectangleShape m_backdrop;
    sf::Font m_font;
    sf::Text m_content;
    sf::Vector2f m_viewSize;
    bool m_visible;
    std::vector<int> m_values;

    sf::Clock m_clock;
    sf::Time m_current;
    std::string m_fontPath;

    Window* m_window;
};



#endif

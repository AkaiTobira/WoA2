#ifndef GAME_H
#define GAME_H



#include "Window.h"
//#include "Textbox.h"
#include "EventManager.h"
#include "StateManager.h"
#include "ResourceManager.h"
#include "FPSrate.h"
#include "time.h"
#include "iomanip"
#include "EntityManager.h"

class Game
{
public :
    //  Game(const std::string & l_title = "Game", const sf::Vector2u & l_size = { 800 , 600 });
    Game();
    ~Game();

    void Update();
    void Render();
    void MoveSprite(EventDetails* l_details);
    Window * GetWindow();
    void RestartClock();
    sf::Time m_elapsed;
    void LateUpdate();

private:

    void MoveTextureForm();

    Window m_window;

    StateManager m_stateManager;
    TextureManager m_textureManager;
    SystemManager  m_systemManager;
    EntityManager  m_entityManager;
    FPS m_fps;

    sf::Texture m_texture;
    sf::Sprite m_spite;
    sf::Vector2i m_increment;
    sf::Clock m_clock;

    sf::Clock m_clockFPS;
    sf::Time m_currentFPS;

    SharedContext m_context;

    //Textbox m_textbox;

    float frametime = 1.0f / 60.0f;

    sf::Time GetElapsed();
};

#endif

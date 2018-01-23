#include "Game.h"

Game::Game(): m_window("Project : WOA", sf::Vector2u(800,640)),
m_stateManager(&m_context),
m_textureManager(static_cast<std::string>("config/textures.cfg")),
m_systemManager(),
m_entityManager(&m_systemManager,&m_textureManager),
m_fps(static_cast<std::string>("res/fonts/Titania.ttf"))
{

    #ifdef DEBUGG_RUN
        std::cout << "Game :: START :: " << Utils::GetWorkingDirectory() << std:: endl;
    #endif


    m_context.m_wind = &m_window;
    m_context.m_textureManager = &m_textureManager;
    m_systemManager.SetEntityManager(&m_entityManager);
    m_context.m_systemManager = &m_systemManager;
    m_context.m_entityManager = &m_entityManager;
    m_fps.SetWindow( &m_window );
    m_fps.SetVisible(true);
    m_clockFPS.restart();
    m_context.m_eventManager = m_window.GetEventManager();
    m_stateManager.SwitchTo(StateType::Intro);
}

Game::~Game() {
    #ifdef DEBUGG_RUN
        std::cout << "Game :: END" << std:: endl;
    #endif
}

void Game::Update()
{

    m_currentFPS = m_clockFPS.getElapsedTime();
    m_window.Update();
    //  float timestep = 1.0f; // m_snake.GetSpeed();
    m_stateManager.Update(m_currentFPS);
    m_fps.Update(m_currentFPS);
    m_clockFPS.restart();

}

/*
src/S_Movement.cpp:631:0: warning: ignoring #pragma omp parallel [-Wunknown-pragmas]
     #pragma omp parallel for
 
src/S_Movement.cpp:659:0: warning: ignoring #pragma omp critical [-Wunknown-pragmas]
     #pragma omp critical
*/

Window * Game::GetWindow()
{
    return &m_window;
}

void Game::Render()
{

  //  clock_t t = clock();

    m_window.BeginDraw();
    m_stateManager.Draw();
    m_fps.Draw();
    m_window.EndDraw();

 //   clock_t t1 = clock();

  //  std::cout << std::fixed << std::setprecision(5) << 1.0f / static_cast<float>( static_cast<float>(t1-t) /CLOCKS_PER_SEC ) << std::endl;

    //std::cout << std::fixed << std::setprecision(5) << static_cast<float>(1.0f/ (static_cast<float>((t1-t))/CLOCKS_PER_SEC)) << std::endl;

}

void Game::LateUpdate()
{
    m_stateManager.ProcessRequests();
    RestartClock();
}

sf::Time Game::GetElapsed()
{
    return m_elapsed;
}
void Game::RestartClock()
{
    m_elapsed += m_clock.restart();
}

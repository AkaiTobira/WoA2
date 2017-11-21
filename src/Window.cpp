#include "Window.h"

Window::Window( const std::string & l_title, const sf::Vector2u & l_size )
:m_eventManager("config/keys.cfg")
{
    #ifdef DEBUGG_RUN
        std::cout << "MainWindow :: START :: config/keys.cfg" << std:: endl;
    #endif

    Setup(l_title, l_size);
}

Window::~Window()
{   
     #ifdef DEBUGG_RUN
        std::cout << "MainWindow :: END" << std:: endl;
    #endif

    Destroy();
}

void Window::ToggleFullscreen( EventDetails* l_details __attribute__((unused)))
{
    m_isFullscreen = !m_isFullscreen;
    Destroy();
    Create();
}

void Window::Close( EventDetails* l_details __attribute__((unused)) )
{
    m_isDone = true;
}

void Window::SetViewMove(float x,float y)
{
    m_moveView.x = m_moveView.x + x;
    m_moveView.y = m_moveView.y + y;
}


sf::Vector2f Window::GetViewPos()
{
    sf::Vector2f viewCenter = m_window.getView().getCenter();
    sf::Vector2f viewSize = m_window.getView().getSize();
    sf::Vector2f viewSizeHalf(viewSize.x / 2, viewSize.y / 2);
    return viewCenter - viewSizeHalf;
}


sf::Vector2f Window::GetViewSize()
{
    return m_window.getView().getSize();
}

sf::FloatRect Window::GetViewSpace()
{
    sf::Vector2f viewCenter = m_window.getView().getCenter();
    sf::Vector2f viewSize = m_window.getView().getSize();
    sf::Vector2f viewSizeHalf(viewSize.x / 2, viewSize.y / 2);
    sf::FloatRect viewSpace(viewCenter - viewSizeHalf + m_moveView, viewSize);
    return viewSpace;
}

EventManager* Window::GetEventManager()
{
    return &m_eventManager;
}

void Window::Setup( const std::string & l_title, const sf::Vector2u & l_size )
{
    m_windowTitle = l_title;
    m_windowSize = l_size;
    m_windowDefaultSize = l_size;
    m_isFullscreen = false;
    m_isDone = false;
    m_moveView = sf::Vector2f(0,0);
    m_isFocused = true;
    m_eventManager.AddCallback(StateType(0),"Fullscreen_toggle", &Window::ToggleFullscreen, this);
    m_eventManager.AddCallback(StateType(0),"Window_close", &Window::Close, this);
    m_mods = sf::VideoMode::getFullscreenModes();
    m_window.setFramerateLimit(30);

    Create();
}

void Window::Create()
{
    if( m_isFullscreen ){
        for( unsigned int i = 0; i < m_mods.size(); i++){
                if( m_mods[i].isValid() ){
                m_windowSize = { m_mods[i].width, m_mods[i].height };
                m_window.create( 
                    {  
                        m_mods[i].width, 
                        m_mods[i].height, 
                        m_mods[i].bitsPerPixel 
                    }, 
                    m_windowTitle, 
                    sf::Style::Fullscreen );
                    
                }
                break;
            }
        }else{
            m_windowSize = m_windowDefaultSize;
            m_window.create( 
                { 
                    m_windowSize.x, 
                    m_windowSize.y, 
                    32 
                }, 
                m_windowTitle, 
                sf::Style::Default );            
        }
    
    m_window.setVerticalSyncEnabled(true);
}

sf::Vector2f Window::GetMoveWiew()
{
    return m_moveView;
}

void Window::Destroy()
{
    m_window.close();
}

void Window::Update()
{
    sf::Event event;
    while( m_window.pollEvent(event) )
    {
        switch(event.type)
        {
        case sf::Event::LostFocus:
            m_isFocused = false;
            m_eventManager.SetFocus(false);
            break;
        case sf::Event::GainedFocus:
            m_isFocused = true;
            m_eventManager.SetFocus(true);
            break;
        default:
            m_eventManager.HandleEvent(event);
            break;
        }
        m_eventManager.Update();
    }
}

void Window::ToggleFullscreen()
{
    #ifdef DEBUGG_RUN
        std::cout << "MainWindow :: FULLSCREEN_CHANGED" << std:: endl;
    #endif

    m_isFullscreen = !m_isFullscreen;
    Destroy();
    Create();
}

void Window::BeginDraw()
{
    m_window.clear(sf::Color::Black);
}
void Window::EndDraw()
{
    m_window.display();
}

bool Window::IsDone()
{
    return m_isDone;
}
bool Window::IsFullscreen()
{
    return m_isFullscreen;
}
sf::Vector2u Window::GetWindowSize()
{
    return m_windowSize;
}

void Window::Draw(sf::Drawable & l_drawable)
{
    m_window.draw(l_drawable);
}

sf::RenderWindow* Window::GetRenderWindow()
{
    return &m_window;
}

void Window::Close()
{
    m_isDone = true;
}

#include "FPSrate.h"

FPS::FPS(std::string l_path ): m_fontPath( l_path )
{
    Setup(10,50,sf::Vector2f(0,10));
    
    #ifdef DEBUGG_RUN
        std::cout << "FPS :: START :: " << m_fontPath << std::endl;
    #endif 

}

FPS::~FPS() {

    #ifdef DEBUGG_RUN
        std::cout << "FPS :: END" << std::endl;
    #endif 
}

void FPS::SetWindow(Window *l_window)
{
    m_window = l_window;
    sf::Vector2u m_v = m_window->GetWindowSize();
    m_viewSize = m_window->GetViewSize();
    Setup(10,50,sf::Vector2f((m_v.x - 60.f),10.0f));
}

void FPS::SetVisible( const bool & l_visible)
{
    m_visible = l_visible;
}

void FPS::Setup(  int l_charSize, int l_width, sf::Vector2f l_screenPos)
{
    sf::Vector2f l_offset( 0.2f, 0.2f);
    m_visible = false;
    m_font.loadFromFile( Utils::GetWorkingDirectory() + m_fontPath );
    m_content.setFont(m_font);
    m_content.setString("");
    m_content.setCharacterSize(l_charSize);
    m_content.setFillColor(sf::Color::White);

    m_content.setPosition((l_screenPos + l_offset));

    m_backdrop.setSize(sf::Vector2f(l_width, (l_charSize * 1.2f)) );
    m_backdrop.setFillColor(sf::Color(90,90,90,90));

    m_backdrop.setPosition(l_screenPos);

    m_current =  m_clock.getElapsedTime();

}

void FPS::Draw()
{
    m_window->Draw(m_backdrop);
    m_window->Draw(m_content);
}

int FPS::Evaluate()
{
    int m_sum = std::accumulate(m_values.begin(), m_values.end(), 0);

    if( m_values.size() > NUMBER_OF_COUTS )
        m_values.assign( m_values.begin()+ m_values.size() - m_values.size()/3, m_values.end());

    return (int)(m_sum/m_values.size());
}

void FPS::Update( const sf::Time & l_elapsed)
{
    if( !m_visible )
    {
        m_content.setString("");
        return;
    }

    sf::Vector2f l_offset( 0.2f, 0.2f);
    
   // std::cout << l_elapsed.asSeconds() << std::endl;

    m_current =  m_clock.getElapsedTime();
    m_viewSize = m_window->GetViewPos();
    // sf::Vector2u m_v = m_window->GetWindowSize();

    //  m_v = {WINDOW_SCALE * m_v.x, WINDOW_SCALE * m_v.y };

    //  sf::Vector2f cor = { 60 , (float)m_v.y - 10 };

    //  sf::Vector2f win = { (float)m_v.x, (float)m_v.y};

    //  sf::Vector2f sum = m_viewSize + win - cor ;

    // sf::Vector2f i = {30,30};


    m_content.setPosition(( m_viewSize + l_offset));
    m_backdrop.setPosition(  m_viewSize );

    //   std::cout << m_viewSize.x << " " << m_viewSize.y << " " << m_v.x << " " << m_v.y << " " << (m_viewSize + win - cor + l_offset).x<< " " << (m_viewSize + win - cor + l_offset).y   << std::endl;


    //  std::cout << m_viewSize.x << " " << m_viewSize.y << " " << m_v.x << " " << m_v.y << std::endl;

    //if( m_current.asSeconds() > 0.08f ){
    //  std::string l_content = std::to_string(  (int)(1.0f / l_elapsed.asSeconds()) ) + " :FPS";
    //  m_content.setString(l_content);
    //  m_clock.restart();
// }

    std::stringstream factory;

    if( m_current.asSeconds() > 0.5f )
    {
        m_values.push_back(static_cast<unsigned int>(1.0f / l_elapsed.asSeconds()) );
        factory <<  Evaluate()  << " :FPS";
        std::string l_content = factory.str();
        m_content.setString(l_content);
        m_clock.restart();
    }


}

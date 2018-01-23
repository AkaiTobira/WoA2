#ifndef STATEPAUSED_H
#define STATEPAUSED_H

#include "StateManager.h"

class State_Paused : public BaseState{
    public:
        ~State_Paused();
        State_Paused(StateManager * l_stateManager) ;
        void Continue(EventDetails* l_details);

        void OnCreate() ;
        void OnDestroy() ;

        void Activate() ;
        void Deactivate() ;

        void Update(const sf::Time& l_time) ;
        void Draw() ;
        void Unpause(EventDetails* l_details);
    private:
        sf::Texture m_introTexture;
        sf::Sprite m_introSprite;
        sf::Text m_text;
        sf::Font m_font;
        sf::RectangleShape m_rect;
        float m_timePassed;
};

#endif
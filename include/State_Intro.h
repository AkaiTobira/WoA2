#ifndef STATE_INTRO_H
#define STATE_INTRO_H


#include "StateManager.h"

class State_Intro : public BaseState{
    public:
        State_Intro(StateManager * l_stateManager);
        ~State_Intro();
        void Continue(EventDetails* l_details);

        void OnCreate() ;
        void OnDestroy() ;

        void Activate() ;
        void Deactivate() ;

        void Update(const sf::Time& l_time) ;
        void AnimateLogo( sf::Sprite& l_sprite);
        void SetStart();
        void Draw() ;

        void ResetIntro();
    private:

        sf::Texture m_introTexture;
        sf::Sprite m_introSprite;
        sf::Text m_text;
        sf::Font m_font;
        float m_timePassed;
        float m_color;
        bool m_complete;
        int  m_animationID;

};

#endif // STATE_INTRO_H

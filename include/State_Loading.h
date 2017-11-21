#ifndef STATE_LOAD_H
#define STATE_LOAD_H


#include "StateManager.h"

class State_Loading : public BaseState{
    public:
        State_Loading(StateManager * l_stateManager);
        ~State_Loading();
        void Continue(EventDetails* l_details);

        void OnCreate() ;
        void OnDestroy() ;

        void Activate() ;
        void Deactivate() ;

        void Update(const sf::Time& l_time);
        void Draw() ;

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

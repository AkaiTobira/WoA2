#ifndef STATEMENU_H
#define STATEMENU_H

#include "StateManager.h"

const int NUMBER_OF_LABELS = 4;


class State_MainMenu : public BaseState{
    public:
        //void Continue(EventDetails* l_details);
        State_MainMenu(StateManager * l_stateManager);
        ~State_MainMenu();

        void OnCreate() ;
        void OnDestroy() ;

        void Activate() ;
        void Deactivate() ;

        void Update(const sf::Time& l_time) ;
        void Draw();

     //   void Quit(EventDetails* l_details);
        void MouseClick(EventDetails* l_details);

        void Play(EventDetails* l_details);
        void Credits(EventDetails* l_details);
        void Quit(EventDetails* l_details);
        void Options(EventDetails* l_details);

    private:


       // sf::Texture m_introTexture;
       // sf::Sprite m_introSprite;

        sf::Vector2f m_buttonSize;
        sf::Vector2f m_buttonPos;
        sf::Vector2u m_windowSize;

        unsigned int m_buttonPadding;

        sf::Text m_text;
        sf::RectangleShape m_rects[NUMBER_OF_LABELS];

        sf::Text m_labels[NUMBER_OF_LABELS];

        sf::Font m_font;


};

#endif

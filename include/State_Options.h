#ifndef STATEOPTIONS_H
#define STATEOPTIONS_H

#include "StateManager.h"

const int NUMBER_OF_OPTIONS = 1;

class State_Options : public BaseState{
    public:
        State_Options( StateManager * l_stateManager);
        ~State_Options();

        void OnCreate();
        void OnDestroy();

        void Activate() ;
        void Deactivate() ;

        void Update(const sf::Time& l_time);
        void Draw();

        void MouseClick(EventDetails* l_details);
    private:

        float m_middle;
        sf::Vector2f m_buttonSize;
        sf::Vector2f m_buttonPos;
        unsigned int m_buttonPadding;

        sf::Text m_text;
        sf::RectangleShape m_rects[NUMBER_OF_OPTIONS*2]; 
        sf::Text m_labels[NUMBER_OF_OPTIONS*3];

        sf::Font m_font;

};



#endif
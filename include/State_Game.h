#ifndef STATEGAME_H
#define STATEGAME_H

#include "StateManager.h"
#include "Map.h"
#include <SFML/Graphics.hpp>
#include <algorithm>

class State_Game : public BaseState{
    public:
        State_Game(StateManager * l_stateManager);// : BaseState(l_stateManager){};
        //void Continue(EventDetails* l_details);
        ~State_Game();
        
        void OnCreate() ;
        void OnDestroy() ;

        void Activate() ;
        void Deactivate() ;

        void Update(const sf::Time& l_time) ;
        void Draw() ;

        void MainMenu( EventDetails* l_details );
        void Pause( EventDetails * l_details );
        void UpdateCamera();
        //void MouseClick(EventDetails* l_details);

    private:
       // sf::Texture m_introTexture;
       // sf::Sprite m_introSprite;
       float MOVE_SPEED;
        void Print( EventDetails* l_details);
        void Reinforcment(EventDetails* l_details);
        void MoveCamera  (EventDetails* l_details);
        void StopEntity  (EventDetails* l_details);
        void MoveEntity  (EventDetails* l_details);
        void SpawnEntity (EventDetails* l_details);
        void SpawnHorse  (EventDetails* l_details);
        void ChoseAll    (EventDetails* l_details);
        void Test        (EventDetails* l_details);
        void KillEntity  (EventDetails* l_details);

        sf::Texture m_texture;
        sf::Sprite m_spite;
        sf::Vector2f m_increment;
       
        sf::Vector2f m_buttonSize;
        sf::Vector2f m_buttonPos;
        unsigned int m_buttonPadding;
        SharedContext * m_context;
        sf::Text m_text;
        sf::RectangleShape m_rects[3];
        sf::Vector2f m_cameraMove;
        sf::Text m_labels[3];
        sf::Vector2u m_mapSize;
        sf::Vector2f m_viewSize;

        sf::Font m_font;
        Map* m_gameMap;

        std::vector<unsigned int> m_entities;
        int m_entityid;

        sf::RectangleShape box;

        bool m_leftClickDown;

        sf::Vector2f m_posDown;
        sf::Vector2f m_posRelease;
};

#endif
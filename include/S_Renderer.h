
#ifndef S_RENDERER_H
#define S_RENDERER_H


#include "EntityManager.h"


class S_Renderer : public S_Base{
public:
    S_Renderer(SystemManager* l_systemMgr);
    ~S_Renderer();

    void Update(float l_dT);
    void HandleEvent(const EntityId& l_entity,const EntityEvent& l_event);
    void Notify(const Message& l_message);
    void Render(Window* l_wind, unsigned int l_layer);
    void HandleEvent(
        const std::set<unsigned int>& participians,
        const EntityEvent& l_events
    );

    bool RemoveEntity(const EntityId& l_entity);

private:
    void SetSheetDirection(const EntityId& l_entity, const Direction& l_dir);
    void SortDrawables();

    sf::Font m_font;
    sf::Texture m_targetTexture;
    sf::Sprite m_targetSprite;
    bool m_once;
};


#endif

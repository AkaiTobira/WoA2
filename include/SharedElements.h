#pragma once
#ifndef SHAREDELEMENTS_H
#define SHEREDELEMENTS_H

#include "Window.h"
#include "GUI.h"
#include "EventManager.h"


class TextureManager;
class Map;
class EntityManager;
class SystemManager;
class FontManager;
class GUI_Manager;


struct SharedContext
{
    SharedContext():m_wind(nullptr),m_eventManager(nullptr) {}
    Window*         m_wind;
    EventManager*   m_eventManager;
    TextureManager* m_textureManager;
    FontManager*    m_fontManager;
    EntityManager*  m_entityManager;
    SystemManager*  m_systemManager;
    GUI_Manager*    m_guiManager;
    Map*            m_gameMap;
};

#endif

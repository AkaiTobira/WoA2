#pragma once
#ifndef SHAREDELEMENTS_H
#define SHEREDELEMENTS_H

#include "Window.h"
#include "EventManager.h"


class TextureManager;
class Map;
class EntityManager;
class SystemManager;

struct SharedContext
{
    SharedContext():m_wind(nullptr),m_eventManager(nullptr) {}
    Window*         m_wind;
    EventManager*   m_eventManager;
    TextureManager* m_textureManager;
    EntityManager*  m_entityManager;
    SystemManager*  m_systemManager;
    Map*            m_gameMap;
};

#endif

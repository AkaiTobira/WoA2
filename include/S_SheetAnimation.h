#ifndef S_SHETANIMAT_H
#define S_SHETANIMAT_H

#include "EntityManager.h"

class S_SheetAnimation : public S_Base{
public:
    S_SheetAnimation(SystemManager* l_systemMgr);
    ~S_SheetAnimation();

    void Update(float l_dT);
    void HandleEvent(const EntityId& l_entity,const EntityEvent& l_event);
    void Notify(const Message& l_mssage);
    void ChangeAnimation(
        const EntityId& l_entity,
        const std::string& l_anim,
        bool l_play,
        bool l_loop
    );
    
    bool RemoveEntity(const EntityId& l_entity);

    void ChangeAnimation(
        const std::set<unsigned int>& l_entities,
        const std::string& l_anim,
        bool l_play,
        bool l_loop
    );
    void HandleEvent(
        const std::set<unsigned int>& participians,
        const EntityEvent& l_events
    );
};

#endif
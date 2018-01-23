#ifndef S_STATE_H
#define S_STATE_H

#include "EntityManager.h"

class S_State : public S_Base{
public:
    S_State(SystemManager* l_systemMgr);
    ~S_State();

    void Update(float l_dT);
    void HandleEvent(const EntityId& l_entity,const EntityEvent& l_event);
    void Notify(const Message& l_mssage);
    void ChangeState(const EntityId& l_entity,const EntityState& l_state, const bool& l_force);
    void ChangeState(const std::vector<int>& reciver,const EntityState& l_state);
    void ChangeState(const std::set<unsigned int>& participians,const EntityState& l_state, const bool& l_force);
    void HandleEvent(const std::set<unsigned int>& participians,const EntityEvent& l_events);
    void KeepState(const EntityId& l_entity,const EntityState& l_state);

    bool RemoveEntity(const EntityId& l_entity);
};

#endif
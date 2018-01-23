#ifndef ENTITYMESSAGE_H
#define ENTITYMESSAGE_H


enum class EntityMessage {
        Move,
        Is_Moving, 
        State_Changed, 
        Direction_Changed,  
        Switch_State, 
        Spawned,
        Attack_Action, 
        Dead,
        Is_Idling
 };

#endif // ENTITYMESSAGE_H

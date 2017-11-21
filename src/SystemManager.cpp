#include "SystemManager.h"

S_Base::S_Base(const System& l_id, SystemManager* l_systemMgr)
    : m_id(l_id), m_systemManager(l_systemMgr) {}
S_Base::~S_Base()
{

    Purge();

}


bool S_Base::AddEntity(const EntityId& l_entity)
{

    if (HasEntity(l_entity))
    {
        return false;
    }
    m_entities.emplace_back(l_entity);

    return true;
}

bool S_Base::HasEntity(const EntityId& l_entity)
{
    return std::find(m_entities.begin(),m_entities.end(), l_entity) != m_entities.end();
}
bool S_Base::RemoveEntity(const EntityId& l_entity)
{
    auto entity = std::find_if(m_entities.begin(), m_entities.end(),
                               [&l_entity](EntityId& id)
    {
        return id = l_entity;
    });
    if (entity == m_entities.end())
    {
        return false;
    }
    m_entities.erase(entity);
    return true;
}

bool S_Base::FitsRequirements(const Bitmask& l_bits)
{
    return std::find_if(m_requiredComponents.begin(),m_requiredComponents.end(), [&l_bits](Bitmask& b)
    {
        return b.Matches(l_bits, b.GetMask());
    }) != m_requiredComponents.end();
}

void S_Base::Purge()
{
    m_entities.clear();
}


SystemManager::SystemManager(): m_entityManager(nullptr)
{

    #ifdef DEBUGG_RUN
        std::cout << "SystemManager :: START" << std::endl;
    #endif

    
    m_systems[System::State] = new S_State(this);
    m_systems[System::Control] = new S_Control(this);
    m_systems[System::Movement] = new S_Movement(this);
    m_systems[System::Collision] = new S_Collision(this);
    m_systems[System::SheetAnimation] = new S_SheetAnimation(this);
    m_systems[System::Renderer] = new S_Renderer(this);

    m_partOfHandling = 0;

    #ifdef DEBUGG_RUN
        std::cout << "SystemManager :: REGISTERED_SYSTEMS :: "<< m_systems.size() << std::endl;
    #endif
}
SystemManager::~SystemManager()
{
    PurgeSystems();
}
void SystemManager::SetEntityManager(EntityManager* l_entityMgr)
{
    if(!m_entityManager)
    {
        m_entityManager = l_entityMgr;
    }
  //  std::cout<<"S_Base SETENTITYMANAGR " << m_entityManager->m_entities.size() << " BEGIN " << std::endl;
}

EntityManager* SystemManager::GetEntityManager()
    {
        return m_entityManager;
    }
MessageHandler* SystemManager::GetMessageHandler()
    {
        return &m_messages;
    }
void SystemManager::AddEvent(const EntityId& l_entity,    const EventID& l_event)
    {
        m_events[l_entity].AddEvent(l_event);
    }

void SystemManager::Update(float l_dT)
    {
     //   std::cout<<"SYSTSEM UPDATE " << m_entityManager->m_entities.size() << " BEGIN " << std::endl;
     
  //   int i = 0;
    // std::cout << "UPDATE :: " << std::endl;
        for(auto &itr : m_systems)
        {
   //         std::cout << i++ << std::endl; 
            itr.second->Update(l_dT);
        }
    //    std::cout << "HANDLE EVENTS :: " << m_events.size() <<  std::endl;
        HandleEvents();
    }
    


void SystemManager::Draw(Window* l_wind,  unsigned int l_elevation)
    {
     //   std::cout<<"SYSTSEM DRAW" << m_entityManager->m_entities.size() << " BEGIN " << std::endl;
        auto itr = m_systems.find(System::Renderer);
        if (itr == m_systems.end())
        {
            return;
        }
        S_Renderer* system = (S_Renderer*)itr->second;
        system->Render(l_wind, l_elevation);
    }

void SystemManager::EntityModified(const EntityId& l_entity,  const Bitmask& l_bits)
    {
      //  std::cout<<"SYSTSEM ENTITYMODIFIED " << m_entityManager->m_entities.size() << " BEGIN " << std::endl;
        for(auto &s_itr : m_systems)
        {
            S_Base* system = s_itr.second;
            if(system->FitsRequirements(l_bits))
            {
                if(!system->HasEntity(l_entity))
                {
                    system->AddEntity(l_entity);
                }
            }
            else
            {
                if(system->HasEntity(l_entity))
                {
                    system->RemoveEntity(l_entity);
                }
            }
        }
    }

void SystemManager::RemoveEntity(const EntityId& l_entity)
    {
        for(auto &system : m_systems)
        {
            system.second->RemoveEntity(l_entity);
        }
    }

void SystemManager::PurgeEntities()
    {
        for(auto &system : m_systems)
        {
            system.second->Purge();
        }
    }

void SystemManager::PurgeSystems()
    {
        for (auto &system : m_systems)
        {
            delete system.second;
        }
        m_systems.clear();
    }


void SystemManager::HandleEvents()
    {    
        



        clock_t t; 
        clock_t t1;
        t = clock();

        for( auto &event : m_events){
            EventID id = 0;
            while(event.second.ProcessEvents(id)){

                for( auto &system : m_systems){
                    if(system.second->HasEntity(event.first))
                    {
 //                       std::cout << id << std::endl;
                        events[id].insert(event.first);
                    }
                }
            }
        }
/*
        unsigned int eventID = 0;
        for( auto &system : m_systems){
            for( auto &event : events)
                if(! event.empty() ){
                system.second->HandleEvent(event, (EntityEvent)eventID++);
            }
        }
*/
       // std::cout << (int)EntityEvent::Became_Idle << std::endl;

        int eventID;
        switch(m_partOfHandling){
            case 0:
                
                for( auto &system : m_systems){
                
                    for( eventID = 0; eventID < 3; eventID++){
                    //for( auto &event : events)
                        if( events[eventID].size() ){
                        system.second->HandleEvent(events[eventID], (EntityEvent)eventID);
                  //      set.clear();
                    //}
                    }
                }
                }

                for( eventID = 0 ; eventID < 3; eventID++) 
                    if( events[eventID].size() )  events[eventID].clear();

            break;
            case 1:

                
                for( auto &system : m_systems){
            
                    for( eventID = 3; eventID < 6; eventID++)
     
                        if( events[eventID].size() ){
                        system.second->HandleEvent(events[eventID], (EntityEvent)eventID);
                //}
                }
            }

            for( eventID = 3 ; eventID < 6; eventID++) 
                if( events[eventID].size() ) events[eventID].clear();
            break;
            case 2:
            
                
                for( auto &system : m_systems){
        
                    for( eventID = 6; eventID < 9; eventID++){
            //for( auto &event : events)

                        if( events[eventID].size() ){

                        
                        system.second->HandleEvent(events[eventID], (EntityEvent)eventID);
            //}
                     }
                }
            }
                for( eventID = 6 ; eventID < 9; eventID++) 
                    if( events[eventID].size() ) events[eventID].clear();

            break;
            case 3:
                ;
                for( auto &system : m_systems){
     
                    for( eventID = 9 ; eventID < 11; eventID++){
    
                        if( events[eventID].size() ){
                    //        std::cout << events[eventID].size() << "  " << eventID << " " << m_systems.size() << std::endl;

                            system.second->HandleEvent(events[eventID], (EntityEvent)eventID);
                //}
                        }
                    }
                }
                for( eventID = 9 ; eventID < 11; eventID++)
                    if( events[eventID].size() ) events[eventID].clear();
            break;
            case 4:
            /*
                unsigned int eventID = 3;
                for( auto &system : m_systems){
        
                for( eventID ; eventID < 6; eventID++)
            //for( auto &event : events)
                    if(! event.empty() ){
                    system.second->HandleEvent(event[eventID], (EntityEvent)eventID++);
            //}
                }
                */
            break;
            case 5:
                m_partOfHandling = -1;
            break; 
            default:
            break;
        }

        m_partOfHandling++;


        int sum = 0;
        for( auto event : events){
            sum += event.size();
        }

   //     std::cout << "NUBER OF Handled EVENTS  ::: " << sum << std::endl; 


     //   for( auto set : events){
    //        set.clear();
    //    }

      //  system.second->HandleEvent(event.first,(EntityEvent)id);

      //  std::cout<<"SYSTSEM HANDLEEVENTS " << m_entityManager->m_entities.size() << " BEGIN " << std::endl;
     /*
      for(auto &event : m_events)
        {
            EventID id = 0;
            std::cout << "EVENTSIZE" << event.second.size() << std::endl;
            while(event.second.ProcessEvents(id))
            {
                for(auto &system : m_systems)
                {
                    if(system.second->HasEntity(event.first))
                    {
                        system.second->HandleEvent(event.first,(EntityEvent)id);
                    }
                }
            }
        }

        

        */

     

        t1 = clock();
   //     std::cout << "HANDLE EVENTS :: " << m_events.size() <<  std::endl;
     //   std::cout << "HANDLE EVENT :" <<  std::fixed << std::setprecision(7) <<  static_cast<float>( static_cast<float>(t1-t) /CLOCKS_PER_SEC ) << std::endl;
        
}
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
  //  SortEntities();
    return true;
}

bool S_Base::HasEntity(const EntityId& l_entity)
{
    return std::find(m_entities.begin(),m_entities.end(), l_entity) != m_entities.end();
}

/*

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

*/

EntityList S_Base::GiveList(){
    return m_entities;
}

void S_Base::SortEntities(){

    for( unsigned int i = 0; i < m_entities.size(); i++){
        for( unsigned int j =0 ; j < m_entities.size()-1; j++){
            if( m_entities[i] < m_entities[j] ){
            auto temp = m_entities[i];
            m_entities[i]= m_entities[j];
            m_entities[j] = temp;
            }
        }
    }

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


EntityList SystemManager::GetList(){
    for( auto &system : m_systems){
        return system.second->GiveList();
    }
    return EntityList();
}


bool SystemManager::CheckEntityLists(){

    EntityList l;
    
    bool flag = true;

    int nm = 0;

    for( auto &system : m_systems){
        system.second->SortEntities();
        nm++;
        if( flag ){
        l = system.second->GiveList();
        flag = false;
        }else{
             EntityList l2 = system.second->GiveList();
            if( l2.size() != l.size() ){ 
                std::cout << "HELL AS FUCK" << std::endl;
                return false;
            }

             for( unsigned int i = 0; i < l2.size(); i++ ){
                if( l2[i] != l[i] ) { 
                    std::cout << "HELL AS FUCK2 " << nm <<" "<< nm+1 << std::endl;
             
                    std::cout << "DIfrence : " << l2[i] << " " << l[i] << std::endl;
             
                    //  return false;
                }
             }

             l = l2;

        }
    }

    std::cout << " ALL FINE " << std::endl;
    return true;
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
     
  
    // std::cout << "UPDATE :: " << std::endl;
        for(auto &itr : m_systems)
        {
        //    std::cout << i++ << std::endl; 
            itr.second->Update(l_dT);
        }
    //    std::cout << "HANDLE EVENTS :: " << m_events.size() <<  std::endl;
        HandleEvents();
    //    std::cout << "UPDATE :: Done" << std::endl;
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
        



    //    clock_t t; 
     //   clock_t t1;
     //   t = clock();
        for( auto &event : m_events){
            EventID id = 0;
            while(event.second.ProcessEvents(id)){

                for( auto &system : m_systems){

                    if(system.second->HasEntity(event.first))
                    {

                        if( id < (int)EntityEvent::END ){
                        events[id].insert(event.first);

                        }else{
                            std::cout <<" Get :: "<<  id << " :  " << event.first << std::endl;
                        
                    }

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

        const unsigned int PER_PART = (unsigned int)EntityEvent::END/ 4;

        unsigned int eventID;
        switch(m_partOfHandling){
            case 0:
        //       std::cout << "HANDle ::  1" << std::endl;
                for( auto &system : m_systems){
                

                    for( eventID = m_partOfHandling *  PER_PART; eventID < (m_partOfHandling+1)* PER_PART ; eventID++){
     
                    //for( auto &event : events)
                        if( events[eventID].size() ){
                        system.second->HandleEvent(events[eventID], (EntityEvent)eventID);
                    //      set.clear();
                        //}
                        }
                    }
                }


                for( eventID = m_partOfHandling *  PER_PART; eventID < (m_partOfHandling+1)* PER_PART ; eventID++)
                    if( events[eventID].size() )  events[eventID].clear();

            break;
            case 1:

            //    std::cout << "HANDle ::  2" << std::endl;
                for( auto &system : m_systems){
            
                    for( eventID = m_partOfHandling *  PER_PART; eventID < (m_partOfHandling+1)* PER_PART ; eventID++){
     
                        if( events[eventID].size() ){
                        system.second->HandleEvent(events[eventID], (EntityEvent)eventID);
                //}
                }
            }
                }

            for( eventID = m_partOfHandling *  PER_PART; eventID < (m_partOfHandling+1)* PER_PART ; eventID++)
                if( events[eventID].size() ) events[eventID].clear();
            break;
            case 2:
            
            //    std::cout << "HANDle ::  3" << std::endl;
                for( auto &system : m_systems){
        
                    
                    for( eventID = m_partOfHandling *  PER_PART; eventID < (m_partOfHandling+1)* PER_PART ; eventID++)
     {
            //for( auto &event : events)

                        if( events[eventID].size() ){

                        
                        system.second->HandleEvent(events[eventID], (EntityEvent)eventID);
            //}
                     }
                }
            }
                
                    for( eventID = m_partOfHandling *  PER_PART; eventID < (m_partOfHandling+1)* PER_PART ; eventID++)
      
                    if( events[eventID].size() ) events[eventID].clear();

            break;
            case 3:
            //    std::cout << "HANDle ::  4" << std::endl;
                for( auto &system : m_systems){
     
                    
                    for( eventID = m_partOfHandling *  PER_PART; eventID < (m_partOfHandling+1)* PER_PART ; eventID++)
     {
    
                        if( events[eventID].size() ){
                    //        std::cout << events[eventID].size() << "  " << eventID << " " << m_systems.size() << std::endl;

                            system.second->HandleEvent(events[eventID], (EntityEvent)eventID);
                //}
                        }
                    }
                }
                
                    for( eventID = m_partOfHandling *  PER_PART; eventID < (m_partOfHandling+1)* PER_PART ; eventID++)
     
                    if( events[eventID].size() ) events[eventID].clear();
            break;
            case 4:
                for( auto &system : m_systems){
     
                    
                    for( eventID = m_partOfHandling *  PER_PART; eventID < (int)EntityEvent::END; eventID++)
     {
    
                        if( events[eventID].size() ){
                    //        std::cout << events[eventID].size() << "  " << eventID << " " << m_systems.size() << std::endl;

                            system.second->HandleEvent(events[eventID], (EntityEvent)eventID);
                //}
                        }
                    }
                }
                
                    for( eventID = m_partOfHandling *  PER_PART; eventID < (int)EntityEvent::END ; eventID++)
     
                    if( events[eventID].size() ) events[eventID].clear();
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

     

    //    t1 = clock();
   //     std::cout << "HANDLE EVENTS :: " << m_events.size() <<  std::endl;
     //   std::cout << "HANDLE EVENT :" <<  std::fixed << std::setprecision(7) <<  static_cast<float>( static_cast<float>(t1-t) /CLOCKS_PER_SEC ) << std::endl;
        
}
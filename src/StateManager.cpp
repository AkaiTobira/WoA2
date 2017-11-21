#include "StateManager.h"
#include "State_Intro.h"
#include "State_Game.h"
#include "State_Loading.h"
#include "State_MainMenu.h"


StateManager::StateManager(SharedContext* l_shared)
    :m_shared(l_shared)
{
     RegisterState<State_Intro>(StateType::Intro);
     RegisterState<State_MainMenu>(StateType::MainMenu);
     RegisterState<State_Game>(StateType::Game);
     RegisterState<State_Loading>(StateType::Loading);
    //  RegisterState<State_Paused>(StateType::Pause);
    //  RegisterState<State_Options>(StateType::Options);
    #ifdef DEBUGG_RUN
    std::cout << "StateManager :: REGISTERED_STATES :: " << m_stateFactory.size() <<  std:: endl;
    #endif

}

StateManager::~StateManager()
{
    #ifdef DEBUGG_RUN
    std::cout << "StateManager :: END" <<  std:: endl;
#endif
    for( auto &itr: m_states)
    {
        itr.second->OnDestroy();
        delete itr.second;
    }
}

void StateManager::CreateState( const StateType& l_type)
{

    auto newState = m_stateFactory.find(l_type);
    if( newState == m_stateFactory.end())
    {
        return;
    }
    BaseState* state = newState->second();
    state->m_view = m_shared->m_wind->GetRenderWindow()->getDefaultView();
    m_states.emplace_back(l_type, state);
    state->OnCreate();

}

void StateManager::RemoveState(const StateType & l_type)
{

    for( auto itr = m_states.begin(); itr != m_states.end(); ++itr)
    {
        if( itr->first == l_type)
        {
            itr->second->OnDestroy();
            delete itr->second;
            m_states.erase(itr);
            return;
        }
    }
}

void StateManager::Update( const sf::Time & l_time)
{

    if( m_states.empty())
    {
        return;
    }

    if( m_states.back().second->IsTranscendent() && m_states.size() > 1 )
    {

        auto itr = m_states.end();

        while (itr != m_states.begin() )
        {
            if( itr != m_states.end() )
            {
                if( !itr->second->IsTranscendent() )
                {
                    break;
                }
            }
            --itr;
        }
        for(; itr!= m_states.end(); ++itr)
        {
            itr->second->Update(l_time);
        }

    }
    else
    {
        m_states.back().second->Update(l_time);
    }
}


void StateManager::Draw()
{

//    std::cout << m_states.size() << std::endl;

    if( m_states.empty())
    {
        return;
    }
    if( m_states.back().second->IsTransparent() && m_states.size() >1 )
    {

        auto itr = m_states.end();
        while (itr != m_states.begin() )
        {
            if( itr != m_states.end() )
            {
                if( !itr->second->IsTransparent() )
                {
                    break;
                }
            }
            --itr;
        }
        for(; itr!= m_states.end(); ++itr)
        {
            m_shared->m_wind->GetRenderWindow()->setView(itr->second->GetView());
            itr->second->Draw();
        }
    }
    else
    {
        m_states.back().second->Draw();
    }

    //       std::cout << m_states.size() << std::endl;

}


void StateManager::SwitchTo(const StateType& l_type){

    m_shared->m_eventManager->setCurrentState(l_type);

    for( auto itr = m_states.begin(); itr != m_states.end(); ++itr){
        if( itr-> first == l_type ){
            m_states.back().second->Deactivate();
            StateType tmp_type = itr->first;
            BaseState* tmp_state = itr->second;
            m_states.erase(itr);
            m_states.emplace_back(tmp_type,tmp_state);
            tmp_state->Activate();
            m_shared->m_wind->GetRenderWindow()->setView(tmp_state->GetView());
            return;
        }

    }
    if( !m_states.empty() ){
        m_states.back().second->Deactivate();
    }


    CreateState(l_type);
    m_states.back().second->Activate();
    m_shared->m_wind->GetRenderWindow()->setView(m_states.back().second->GetView());

}

SharedContext* StateManager::GetContext(){ return m_shared ;}

bool StateManager::HasState(const StateType& l_type){

    for( auto itr = m_states.begin(); itr != m_states.end(); ++itr){
        if( itr->first == l_type){
            auto removed = std::find(m_toRemove.begin(), m_toRemove.end(), l_type);
            if( removed != m_toRemove.end()) {return true;}
            return false;
        }
    }
    return false;
}

void StateManager::Remove(const StateType& l_type){
    m_toRemove.push_back(l_type);
}

void StateManager::ProcessRequests(){

    while( m_toRemove.begin() != m_toRemove.end() ){
        RemoveState(*m_toRemove.begin());
        m_toRemove.erase(m_toRemove.begin());
    }
}

#include "S_Renderer.h"
#define RENDER_DEBUG


S_Renderer::S_Renderer(SystemManager* l_systemMgr)
:S_Base(System::Renderer, l_systemMgr)
{
    Bitmask req;
    req.TurnOnBit((unsigned int)Component::Position);
    req.TurnOnBit((unsigned int)Component::SpriteSheet);

     #ifdef RENDER_DEBUG
        req.TurnOnBit((unsigned int)Component::State);
        req.TurnOnBit((unsigned int)Component::Collidable);
        req.TurnOnBit((unsigned int)Component::Movable);
    #endif


    m_requiredComponents.push_back(req);
    req.Clear();
    m_systemManager->GetMessageHandler()-> Subscribe(EntityMessage::Direction_Changed,this);


    #ifdef RENDER_DEBUG
        std::string path = "res/fonts/Titania.ttf";
        m_font.loadFromFile( Utils::GetWorkingDirectory() + path);
    #endif
}


S_Renderer::~S_Renderer(){}

void S_Renderer::Update(float l_dT __attribute__((unused))){
    EntityManager* entities = m_systemManager->GetEntityManager();

    for(auto &entity : m_entities){

        C_Position* position = entities->GetComponent<C_Position>(entity, Component::Position);
        C_Drawable* drawable = nullptr;
        if (entities->HasComponent(entity, Component::SpriteSheet)){
            drawable = entities->GetComponent<C_Drawable>(entity, Component::SpriteSheet);
        } else { continue; }

        drawable->UpdatePosition(position->GetFPosition());
    }
}

bool S_Renderer::RemoveEntity(const EntityId& l_entity)
{

    for( auto it = m_entities.begin(); it != m_entities.end(); it++){
        if( l_entity == *it ){
            m_entities.erase(it);
            return true;
        }
    }
    return false;
}

void S_Renderer::Notify(const Message& l_message){

    for( auto reciver : l_message.m_receivers){

    if(HasEntity(reciver)){
        EntityMessage m = (EntityMessage)l_message.m_type;
        switch(m){
            case EntityMessage::Direction_Changed:
            SetSheetDirection(reciver,(Direction)l_message.m_int);
            break;
            default:
            break;
        }
    }
}
}
#include <string>

void S_Renderer::Render(Window* l_wind, unsigned int l_layer){
    EntityManager* entities = m_systemManager->GetEntityManager();
    for(auto &entity : m_entities){
        C_Position* position = entities->GetComponent<C_Position>(entity, Component::Position);
        if(position->GetElevation() < l_layer){ continue; }
        if(position->GetElevation() > l_layer){ break; }

        C_Drawable* drawable = nullptr;
        if (!entities->HasComponent(entity,Component::SpriteSheet)){
            continue; 
        }
        drawable = entities->GetComponent<C_Drawable>(entity, Component::SpriteSheet);
        sf::FloatRect drawableBounds;
        drawableBounds.left   = position->GetFPosition().x - ( drawable->GetSize().x / 2 ) ;
        drawableBounds.top    = position->GetFPosition().y -   drawable->GetSize().y;
        drawableBounds.width  = drawable->GetSize().x;
        drawableBounds.height = drawable->GetSize().y;

        #ifdef RENDER_DEBUG
            sf::Text nmbr;
            nmbr.setFont(m_font);
            nmbr.setString(std::to_string(entity));
            nmbr.setFillColor(sf::Color::White);
            nmbr.setCharacterSize(12);
            nmbr.setPosition( position->GetFPosition().x , position->GetFPosition().y  );
       
       
            C_Collidable* collidable = entities->GetComponent<C_Collidable>(entity, Component::Collidable);
    
            sf::RectangleShape collisionBox;

            collisionBox.setSize(collidable->GetSize());
            collisionBox.setPosition(collidable->GetPosition());

            collisionBox.setOutlineThickness(1);
            if( !position->IsPaused() ){
            collisionBox.setOutlineColor(sf::Color::Blue);
            }else{
            collisionBox.setOutlineColor(sf::Color::Yellow);    
            }
            collisionBox.setFillColor(sf::Color::Transparent);

            C_Movable* movable = entities->GetComponent<C_Movable>(entity, Component::Movable);

            sf::CircleShape targetDot;
            targetDot.setRadius(6);
            targetDot.setPosition(movable->GetFTargetPosition());
            targetDot.setFillColor(sf::Color::Magenta);
            targetDot.setOutlineColor(sf::Color::Magenta);  


            sf::RectangleShape LB1;
            sf::RectangleShape LB2;

            LB1.setSize(collidable->GetSize());
            LB1.setPosition(movable->GetCurrentReservation().x*32,movable->GetCurrentReservation().y*32 );
            LB1.setOutlineColor(sf::Color::Magenta);  
            LB1.setOutlineThickness(1);
            LB1.setFillColor(sf::Color::Transparent);

            LB2.setSize(sf::Vector2f(collidable->GetSize().x-2,collidable->GetSize().y-2) );
            LB2.setPosition(movable->GetPrevReservation().x*32+1,movable->GetPrevReservation().y*32+1 );
            LB2.setOutlineColor(sf::Color::Cyan);  
            LB2.setOutlineThickness(1);
            LB2.setFillColor(sf::Color::Transparent);

        #endif

        if (!l_wind->GetViewSpace().intersects(drawableBounds))
        {
            continue;
        }
          #ifdef RENDER_DEBUG
            l_wind->GetRenderWindow()->draw(LB1);
            l_wind->GetRenderWindow()->draw(LB2);
            #endif
        drawable->Draw(l_wind->GetRenderWindow());

        #ifdef RENDER_DEBUG

            l_wind->GetRenderWindow()->draw(nmbr);
            l_wind->GetRenderWindow()->draw(collisionBox);
            l_wind->GetRenderWindow()->draw(targetDot);
        #endif
    }
}

void S_Renderer::SetSheetDirection(const EntityId& l_entity,const Direction& l_dir)
    {
    EntityManager* entities = m_systemManager->GetEntityManager();
    if (!entities->HasComponent(l_entity,Component::SpriteSheet))
    {
    return;
    }
    C_SpriteSheet* sheet = entities->GetComponent<C_SpriteSheet>(l_entity,Component::SpriteSheet);
    sheet->GetSpriteSheet()->SetDirection(l_dir);
}

void S_Renderer::SortDrawables(){
        EntityManager* e_mgr = m_systemManager->GetEntityManager();
        std::sort(m_entities.begin(), m_entities.end(), [e_mgr](unsigned int l_1, unsigned int l_2){
            auto pos1 = e_mgr->GetComponent<C_Position>(l_1, Component::Position);
            auto pos2 = e_mgr->GetComponent<C_Position>(l_2, Component::Position);
            if (pos1->GetElevation() == pos2->GetElevation()){
                return pos1->GetFPosition().y < pos2->GetFPosition().y;
            }
            return pos1->GetElevation() < pos2->GetElevation();
            }
        );
}





void S_Renderer::HandleEvent(const std::set<unsigned int>& participians __attribute((unused)), const EntityEvent& l_event){
    if (l_event == EntityEvent::Moving_Left      ||
        l_event == EntityEvent::Moving_Right     ||
        l_event == EntityEvent::Moving_Up        ||
        l_event == EntityEvent::Moving_Down      ||
        l_event == EntityEvent::Elevation_Change ||
        l_event == EntityEvent::Moving_LeftDown  ||
        l_event == EntityEvent::Moving_LeftUp    ||
        l_event == EntityEvent::Moving_RightDown ||
        l_event == EntityEvent::Moving_RightUp   ||
        l_event == EntityEvent::Spawned 
    ){
        SortDrawables();
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

void S_Renderer::HandleEvent(const EntityId& l_entity __attribute((unused)),const EntityEvent& l_event){
    if (l_event == EntityEvent::Moving_Left ||
        l_event == EntityEvent::Moving_Right ||
        l_event == EntityEvent::Moving_Up ||
        l_event == EntityEvent::Moving_Down ||
        l_event == EntityEvent::Elevation_Change ||
        l_event == EntityEvent::Spawned)
    {
        SortDrawables();
    }
}
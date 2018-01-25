
#ifndef GUI_H
#define GUI_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <unordered_map>
#include <iostream>
#ifndef EVENTMANAGER_H
//#include <EventManager.h>
#endif
#include "GUI_Elements.h"
#include "GUI_Events.h"
#include "ResourceManager.h"


enum class GUI_ElementType{ 
    None,
    Window, 
    Label, 
    Button, 
    Scrollbar,
    Textfield 
};

using ElementStyles = std::unordered_map<GUI_ElementState, GUI_Style>;

class GUI_Interface;

class GUI_Element{
    friend class GUI_Interface;
        public:
        GUI_Element(const std::string& l_name, const GUI_ElementType& l_type, GUI_Interface* l_owner);
        virtual ~GUI_Element();
        // Event methods.
        virtual void ReadIn(std::stringstream& l_stream) = 0;
        virtual void OnClick(const sf::Vector2f& l_mousePos) = 0;
        virtual void OnRelease() = 0;
        virtual void OnHover(const sf::Vector2f& l_mousePos) = 0;
        virtual void OnLeave() = 0;
        virtual void Update(float l_dT) = 0;
        virtual void Draw(sf::RenderTarget* l_target) = 0;
        virtual void UpdateStyle(const GUI_ElementState& l_state,
        const GUI_Style& l_style);
        virtual void ApplyStyle();

        void SetRedraw( bool l_red);
        bool IsControl() const;
        void SetPosition(const sf::Vector2f& l_pos);
        bool IsInside(const sf::Vector2f& l_point) const;
        sf::Vector2f GetGlobalPosition() const;
        const sf::Vector2f& GetSize() const;
        const sf::Vector2f& GetPosition() const;
        void SetActive(const bool& l_active);
        const bool IsActive() const;
        std::string GetText() const;
        void SetText(const std::string& l_text);
        void SetState(const GUI_ElementState& l_state);
        const sf::Vector2f& GetPadding() const;
        void SetName(const std::string& l_name);
        void SetOwner(GUI_Interface* l_owner);
        const GUI_ElementType& GetType() const ;
        const bool NeedsRedraw() const;
        GUI_ElementState GetState();

        friend std::stringstream& operator >>(std::stringstream& l_stream, GUI_Element& b){
            b.ReadIn(l_stream);
            return l_stream;
        }
    protected:
        void ApplyTextStyle();
        void ApplyBgStyle();
        void ApplyGlyphStyle();
        
        void RequireTexture(const std::string& l_name);
        void RequireFont(const std::string& l_name);

        void ReleaseTexture(const std::string& l_name);
        void ReleaseFont(const std::string& l_name);
        void ReleaseResources();

        std::string m_name;
        GUI_ElementType m_type;
        GUI_Interface* m_owner;
        GUI_ElementState m_state;
        bool m_needsRedraw;
        bool m_active;
        bool m_isControl;

        sf::Vector2f m_position;
        ElementStyles m_style; // Style of drawables.
        GUI_Visual m_visual; // Drawable bits.


    };

using Elements = std::unordered_map<std::string,GUI_Element*>;

class GUI_Manager;

class GUI_Interface : public GUI_Element{
friend class GUI_Element;
friend class GUI_Manager;
public:
    GUI_Interface(const std::string& l_name,GUI_Manager* l_guiManager);
    ~GUI_Interface();


    bool AddElement(const GUI_ElementType& l_type, const std::string& l_name);
    

    void SetPosition(const sf::Vector2f& l_pos);
    const sf::Vector2f& GetPosition() const;
    const sf::Vector2f& GetContentSize();
    
    GUI_Element* GetElement(const std::string& l_name) const;
    bool RemoveElement(const std::string& l_name);
    
    
    void ReadIn(std::stringstream& l_stream);
    
    void OnClick(const sf::Vector2f& l_mousePos);
    void OnRelease();
    void OnTextEntered(const char& l_char);
    void OnHover(const sf::Vector2f& l_mousePos);
    void OnLeave();
    void Update(float l_dT);
    void Draw(sf::RenderTarget* l_target);
    void BeginMoving();
    void StopMoving();
    sf::Vector2f GetGlobalPosition() const;
    void ApplyStyle();

    void Redraw();
    void RedrawContent();
    void RedrawControls();

    void UpdateScrollHorizontal( unsigned int l_percent);
    void UpdateScrollVertical(unsigned int l_percent);
    
    GUI_Manager* GetManager();
    const sf::Vector2f& GetPadding() const;
    void SetName(const std::string& l_name);

    void Focus();

    bool NeedsRedraw() const;
    bool NeedsContentRedraw() const;
    bool NeedsControlRedraw() const ; 

    bool IsFocused() const ;
    bool IsBeingMoved() const;
    bool IsInside(const sf::Vector2f& l_point) const;

private:
    void DefocusTextfields();
    Elements m_elements;
    sf::Vector2f m_elementPadding;
    GUI_Interface* m_parent;
    GUI_Manager* m_guiManager;

    sf::RenderTexture* m_backdropTexture;
    sf::Sprite m_backdrop;
    // Movement.
    sf::RectangleShape m_titleBar;
    sf::Vector2f m_moveMouseLast;
    bool m_movable;
    bool m_beingMoved;
    bool m_showTitleBar;
    bool m_focused;
    // Variable size.
    void AdjustContentSize(const GUI_Element* l_reference= nullptr);
    void SetContentSize(const sf::Vector2f& l_vec);
    sf::RenderTexture* m_contentTexture;
    sf::Sprite m_content;
    sf::Vector2f m_contentSize;
    int m_scrollHorizontal;
    int m_scrollVertical;
    bool m_contentRedraw;
// Control layer.
    sf::RenderTexture* m_controlTexture;
    sf::Sprite m_control;
    bool m_controlRedraw;
};

using GUI_Interfaces = std::unordered_map<std::string,GUI_Interface*>;
using GUI_Container = std::unordered_map<StateType,GUI_Interfaces>;
using GUI_Events = std::unordered_map<StateType,std::vector<GUI_Event>>;
using GUI_Factory = std::unordered_map<GUI_ElementType,std::function<GUI_Element*(GUI_Interface*)>>;
using GUI_ElemTypes = std::unordered_map<std::string,GUI_ElementType>;

struct SharedContext; // Forward declaration.
class GUI_Manager{
    friend class GUI_Interface;
    public:
    GUI_Manager(EventManager* l_evMgr, SharedContext* l_context);
    ~GUI_Manager();

    SharedContext* GetContext();
    void AddEvent(GUI_Event l_event);

    void HandleClick (EventDetails* l_details);
    void HandleRelease (EventDetails* l_details);
    void HandleTextEntered (EventDetails* l_details);
    bool AddInterface(const StateType& l_state, const std::string& l_name);
    GUI_Interface* GetInterface(const StateType& l_state, const std::string& l_name);
    bool RemoveInterface(const StateType& l_state,const std::string& l_name);
    void SetCurrentState(const StateType& l_state);
    bool PollEvent(GUI_Event& l_event);
    void Update(float l_dT);
    void Render(sf::RenderWindow* l_wind);
    bool LoadInterface(const StateType& l_state, const std::string& l_interface, const std::string& l_name);
   
    template<class T>
    void RegisterElement(const GUI_ElementType& l_id){
        m_factory[l_id] = [](GUI_Interface* l_owner) -> GUI_Element*{
            return new T("",l_owner);
            };
    }
    private:
    GUI_Element* CreateElement(const GUI_ElementType& l_id, GUI_Interface* l_owner);
    GUI_ElementType StringToType(const std::string& l_string);
    bool LoadStyle(const std::string& l_file,GUI_Element* l_element);
    

    EventManager* m_eventMgr;
    GUI_Container m_interfaces;
    GUI_Events m_events;
    SharedContext* m_context;
    StateType m_currentState;
    GUI_Factory m_factory;
    GUI_ElemTypes m_elemTypes;
};

#include "GUI_Structures.h"
#include "SharedElements.h"

#endif
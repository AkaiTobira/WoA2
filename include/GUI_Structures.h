
#ifndef GUISTRUCTS_H
#define GUISTRUCTS_H

#ifndef GUI_H
#include "GUI.h"
#endif

class GUI_Label : public GUI_Element
{   
    public:
    GUI_Label(const std::string& l_name,GUI_Interface* l_owner);


    void ReadIn(std::stringstream& l_stream);
    void OnClick(const sf::Vector2f& l_mousePos);
    void OnRelease();
    void OnHover(const sf::Vector2f& l_mousePos);
    void OnLeave();
    void Draw(sf::RenderTarget* l_target);
    void Update(float l_dT);
    
    ~GUI_Label(){}
};


class GUI_Textfield : public GUI_Element
{
    public:

    GUI_Textfield(const std::string& l_name, GUI_Interface* l_owner);
    void ReadIn(std::stringstream& l_stream);
    void OnRelease();
    void Draw(sf::RenderTarget* l_target);
    void OnClick( const sf::Vector2f& l_mousePos);
    void OnHover( const sf::Vector2f& l_mousePos);
    void OnLeave();
    void Update(float l_dT);

    ~GUI_Textfield(){}
};


enum class SliderType{ Horizontal, Vertical };
class GUI_Scrollbar : public GUI_Element{
public:
    GUI_Scrollbar(const std::string& l_name,GUI_Interface* l_owner);
    void ReadIn(std::stringstream& l_stream);
    void SetPosition(const sf::Vector2f& l_pos);
    void ApplyStyle();
    void UpdateStyle(const GUI_ElementState& l_state,const GUI_Style& l_style);
    void OnClick(const sf::Vector2f& l_mousePos);
    void OnRelease();
    void OnHover(const sf::Vector2f& l_mousePos);
    void OnLeave();
    void Update(float l_dT);
    void Draw(sf::RenderTarget* l_target);
private:
SliderType m_sliderType;
sf::RectangleShape m_slider;
sf::Vector2f m_moveMouseLast;
int m_percentage;
};

#endif
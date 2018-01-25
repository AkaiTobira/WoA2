#include "GUI.h"

GUI_Element::GUI_Element(const std::string& l_name,const GUI_ElementType& l_type, GUI_Interface* l_owner):
 m_name(l_name),
 m_type(l_type), 
 m_owner(l_owner),
 m_state(GUI_ElementState::Neutral), 
 m_needsRedraw(false),
 m_active(true), 
 m_isControl(false){}

 GUI_Element::~GUI_Element(){ ReleaseResources(); }

void GUI_Element::SetRedraw( bool l_red){
    m_needsRedraw = l_red;
}

const sf::Vector2f& GUI_Element::GetPosition() const{
    return m_position;
}

void GUI_Element::ReleaseResources(){
    for (auto &itr : m_style){
        ReleaseTexture(itr.second.m_backgroundImage);
        ReleaseTexture(itr.second.m_glyph);
        ReleaseFont(itr.second.m_textFont);
    }
}

void GUI_Element::ReleaseTexture(const std::string& l_name){
    if (l_name == ""){ return; }
    m_owner->GetManager()->GetContext()->m_textureManager->ReleaseResource(l_name);
}

void GUI_Element::ReleaseFont(const std::string& l_name){
    if (l_name == ""){ return; }
    m_owner->GetManager()->GetContext()->m_fontManager->ReleaseResource(l_name);
}

void GUI_Element::UpdateStyle(const GUI_ElementState& l_state, const GUI_Style& l_style)
{
    // Resource management.
    if (l_style.m_backgroundImage !=m_style[l_state].m_backgroundImage)
    {
        ReleaseTexture(m_style[l_state].m_backgroundImage);
        RequireTexture(l_style.m_backgroundImage);
    }
    if (l_style.m_glyph != m_style[l_state].m_glyph){
        ReleaseTexture(m_style[l_state].m_glyph);
        RequireTexture(l_style.m_glyph);
    }
    if (l_style.m_textFont != m_style[l_state].m_textFont){
        ReleaseFont(m_style[l_state].m_textFont);
        RequireFont(l_style.m_textFont);
    }
    // Style application.
    m_style[l_state] = l_style;
    if (l_state == m_state){ 
        SetRedraw(true); 
        ApplyStyle(); 
        }
}

void GUI_Element::RequireTexture(const std::string& l_name){
    if (l_name == ""){ return; }
    m_owner->GetManager()->GetContext()->m_textureManager->RequireResource(l_name);
}

void GUI_Element::RequireFont(const std::string& l_name){
    if (l_name == ""){ return; }
    m_owner->GetManager()->GetContext()->m_fontManager->RequireResource(l_name);
}

bool GUI_Element::IsControl() const{
    return m_isControl;
}

void GUI_Element::ApplyStyle(){
    ApplyTextStyle();
    ApplyBgStyle();
    ApplyGlyphStyle();
    if (m_owner != this && !IsControl()){
        m_owner->AdjustContentSize(this);
    }
}

void GUI_Element::ApplyTextStyle(){
    FontManager* fonts = m_owner->GetManager()->GetContext()->m_fontManager;
    const GUI_Style& CurrentStyle = m_style[m_state];
    if (CurrentStyle.m_textFont != ""){
        m_visual.m_text.setFont(*fonts->GetResource(CurrentStyle.m_textFont));
        m_visual.m_text.setFillColor(CurrentStyle.m_textColor);
        m_visual.m_text.setCharacterSize(CurrentStyle.m_textSize);
    if (CurrentStyle.m_textCenterOrigin){
        sf::FloatRect rect = m_visual.m_text.getLocalBounds();
        m_visual.m_text.setOrigin(rect.left + rect.width / 2.0f,rect.top + rect.height / 2.0f);
    } else {
        m_visual.m_text.setOrigin(0.f, 0.f);
    }
    }
    m_visual.m_text.setPosition(m_position + CurrentStyle.m_textPadding);
}

void GUI_Element::ApplyBgStyle(){
    TextureManager* textures = m_owner->GetManager()->GetContext()->m_textureManager;
    const GUI_Style& CurrentStyle = m_style[m_state];
    if (CurrentStyle.m_backgroundImage != ""){ m_visual.m_backgroundImage.setTexture(*textures->GetResource(CurrentStyle.m_backgroundImage));
    m_visual.m_backgroundImage.setColor(CurrentStyle.m_backgroundImageColor); }
    m_visual.m_backgroundImage.setPosition(m_position);
    m_visual.m_backgroundSolid.setSize(sf::Vector2f(CurrentStyle.m_size));
    m_visual.m_backgroundSolid.setFillColor(CurrentStyle.m_backgroundColor);
    m_visual.m_backgroundSolid.setPosition(m_position);
}

void GUI_Element::ApplyGlyphStyle(){
    const GUI_Style& CurrentStyle = m_style[m_state];
    TextureManager* textures = m_owner->GetManager()->GetContext()->m_textureManager;
    if (CurrentStyle.m_glyph != ""){
        m_visual.m_glyph.setTexture(*textures->GetResource(CurrentStyle.m_glyph));
    }
    m_visual.m_glyph.setPosition(m_position +CurrentStyle.m_glyphPadding);
}

void GUI_Element::SetState(const GUI_ElementState& l_state){
    if (m_state == l_state){ return; }
    m_state = l_state;
    SetRedraw(true);
}

void GUI_Element::SetPosition(const sf::Vector2f& l_pos){
    m_position = l_pos;
    if (m_owner == nullptr || m_owner == this){ return; }
    const auto& padding = m_owner->GetPadding();
    
    if (m_position.x < padding.x){ m_position.x = padding.x; }
    if (m_position.y < padding.y){ m_position.y = padding.y; }
}

bool GUI_Element::IsInside(const sf::Vector2f& l_point) const{
    sf::Vector2f position = GetGlobalPosition();
    return(l_point.x >= position.x &&  l_point.y >= position.y &&  l_point.x <= position.x + m_style.at(m_state).m_size.x &&   l_point.y <= position.y + m_style.at(m_state).m_size.y);
}

sf::Vector2f GUI_Element::GetGlobalPosition() const{
    sf::Vector2f position = GetPosition();
    if (m_owner == nullptr || m_owner == this){ return position; }
    position += m_owner->GetGlobalPosition();
    if (IsControl()){ return position; }
    position.x -= m_owner->m_scrollHorizontal;
    position.y -= m_owner->m_scrollVertical;
    return position;
}

const sf::Vector2f& GUI_Element::GetSize() const{
    return m_style.at(m_state).m_size;
}

void GUI_Element::SetActive(const bool& l_active){
    if (l_active != m_active){
        m_active = l_active;
        SetRedraw(true);
    }
}

std::string GUI_Element::GetText() const{
    return m_visual.m_text.getString();
}

void GUI_Element::SetText(const std::string& l_text){
    m_visual.m_text.setString(l_text);
    SetRedraw(true);
}

GUI_Interface::GUI_Interface(const std::string& l_name,GUI_Manager* l_guiManager)
: GUI_Element(l_name, GUI_ElementType::Window, this), 
m_parent(nullptr), 
m_guiManager(l_guiManager), 
m_movable(false), 
m_beingMoved(false), 
m_showTitleBar(false), 
m_focused(false), 
m_scrollHorizontal(0),
m_scrollVertical(0),
m_contentRedraw(true), 
m_controlRedraw(true)
{
    m_backdropTexture = new sf::RenderTexture();
    m_contentTexture  = new sf::RenderTexture();
    m_controlTexture  = new sf::RenderTexture();
}

GUI_Interface::~GUI_Interface(){
    delete m_backdropTexture;
    delete m_contentTexture;
    delete m_controlTexture;

    for (auto &itr : m_elements){
        delete itr.second;
    }
}

void GUI_Interface::SetPosition(const sf::Vector2f& l_pos){
    GUI_Element::SetPosition(l_pos);
    m_backdrop.setPosition(l_pos);
    m_content.setPosition(l_pos);
    m_control.setPosition(l_pos);
    m_titleBar.setPosition(m_position.x, m_position.y - m_titleBar.getSize().y);
    m_visual.m_text.setPosition(m_titleBar.getPosition()+ m_style[m_state].m_textPadding);
}

bool GUI_Interface::AddElement(const GUI_ElementType& l_type, const std::string& l_name)
{
    if (m_elements.find(l_name) != m_elements.end()){return false;}
    GUI_Element* element = nullptr;
    element = m_guiManager->CreateElement(l_type, this);
    if (!element){ return false; }
    element->SetName(l_name);
    element->SetOwner(this);
    m_elements.emplace(l_name, element);

    m_contentRedraw = true;
    m_controlRedraw = true;
    return true;
}

GUI_Element* GUI_Interface::GetElement(const std::string& l_name) const{
auto itr = m_elements.find(l_name);
return(itr != m_elements.end() ? itr->second : nullptr);
}

bool GUI_Interface::RemoveElement(const std::string& l_name){
    auto itr = m_elements.find(l_name);
    if (itr == m_elements.end()){ return false; }
    delete itr->second;
    m_elements.erase(itr);
    m_contentRedraw = true;
    m_controlRedraw = true;
    AdjustContentSize();
    return true;
}

bool GUI_Interface::IsInside(const sf::Vector2f& l_point) const{
    if (GUI_Element::IsInside(l_point)){ 
        return true; 
        }
    return m_titleBar.getGlobalBounds().contains(l_point);
}

void GUI_Interface::ReadIn(std::stringstream& l_stream){
    std::string movableState;
    std::string titleShow;
    std::string title;
    l_stream >> m_elementPadding.x >> m_elementPadding.y >> movableState >> titleShow;
    Utils::ReadQuotedString(l_stream, title);
    m_visual.m_text.setString(title);
    if (movableState == "Movable"){ m_movable = true; }
    if (titleShow == "Title"){ m_showTitleBar = true; }
}

void GUI_Interface::OnClick(const sf::Vector2f& l_mousePos){
    DefocusTextfields();
    if (m_titleBar.getGlobalBounds().contains(l_mousePos) && m_movable && m_showTitleBar){
        m_beingMoved = true;
    } else {
        GUI_Event event;
        event.m_type = GUI_EventType::Click;
        event.m_interface = m_name.c_str();
        event.m_element = "";
        event.m_clickCoords.x = l_mousePos.x;
        event.m_clickCoords.y = l_mousePos.y;
        m_guiManager->AddEvent(event);
    for (auto &itr : m_elements){
        if (!itr.second->IsInside(l_mousePos)){ continue; }
        itr.second->OnClick(l_mousePos);
        event.m_element = itr.second->m_name.c_str();
        m_guiManager->AddEvent(event);
    }
    SetState(GUI_ElementState::Clicked);
    }
}

void GUI_Interface::OnRelease(){
    GUI_Event event;
    event.m_type = GUI_EventType::Release;
    event.m_interface = m_name.c_str();
    event.m_element = "";
    m_guiManager->AddEvent(event);
    for (auto &itr : m_elements){
        if (itr.second->GetState() != GUI_ElementState::Clicked){
            continue;
        }
    itr.second->OnRelease();
    event.m_element = itr.second->m_name.c_str();
    m_guiManager->AddEvent(event);
    }
    SetState(GUI_ElementState::Neutral);
}

void GUI_Interface::OnTextEntered(const char& l_char){
    for (auto &itr : m_elements){
        if (itr.second->GetType() != GUI_ElementType::Textfield){
            continue;
        }
        if (itr.second->GetState() != GUI_ElementState::Clicked){
            continue;
        }
        if (l_char == 8){
        // Backspace.
            const auto& text = itr.second->GetText();
            itr.second->SetText(text.substr(0, text.length() -1));
            return;
        }
        if (l_char < 32 || l_char > 126){ return; }
        std::string text = itr.second->GetText();
        text.push_back(l_char);
        itr.second->SetText(text);
        return;
    }
}

void GUI_Interface::DefocusTextfields(){
    GUI_Event event;
    event.m_type = GUI_EventType::Release;
    event.m_interface = m_name.c_str();
    event.m_element = "";
    for (auto &itr : m_elements){
        if (itr.second->GetType() != GUI_ElementType::Textfield){
            continue;
        }
        itr.second->SetState(GUI_ElementState::Neutral);
        event.m_element = itr.second->m_name.c_str();
        m_guiManager->AddEvent(event);
    }
}

const sf::Vector2f& GUI_Interface::GetPosition() const{
    return m_position;
}

const sf::Vector2f& GUI_Interface::GetContentSize(){
    return m_contentSize;
}

GUI_Manager* GUI_Interface::GetManager(){
    return m_guiManager;
}

bool GUI_Interface::NeedsRedraw() const{
    return m_needsRedraw;

}
bool GUI_Interface::NeedsContentRedraw() const{
    return m_contentRedraw;
}
bool GUI_Interface::NeedsControlRedraw() const {
    return m_controlRedraw;
} 

bool GUI_Interface::IsFocused() const {
    return m_focused;
}
bool GUI_Interface::IsBeingMoved() const{
    return m_beingMoved;
}

GUI_ElementState GUI_Element::GetState(){
    return m_state;   
};

void GUI_Element::SetName(const std::string& l_name){
    m_name = l_name;
}

void GUI_Element::SetOwner(GUI_Interface* l_owner){
    m_owner = l_owner;
}

const GUI_ElementType& GUI_Element::GetType() const {
    return m_type;
}

const bool GUI_Element::NeedsRedraw() const {
    return m_needsRedraw;
}

const bool GUI_Element::IsActive() const{
    return m_active;
}






void GUI_Interface::OnHover(const sf::Vector2f& l_mousePos){
    GUI_Event event;
    event.m_type = GUI_EventType::Hover;
    event.m_interface = m_name.c_str();
    event.m_element = "";
    event.m_clickCoords.x = l_mousePos.x;
    event.m_clickCoords.y = l_mousePos.y;
    m_guiManager->AddEvent(event);
    SetState(GUI_ElementState::Focused);
}
void GUI_Interface::OnLeave(){
    GUI_Event event;
    event.m_type = GUI_EventType::Leave;
    event.m_interface = m_name.c_str();
    event.m_element = "";
    m_guiManager->AddEvent(event);
    SetState(GUI_ElementState::Neutral);
}

void GUI_Interface::Update(float l_dT){
    sf::Vector2f mousePos = sf::Vector2f(
    m_guiManager->GetContext()->m_eventManager->GetMousePos(
    m_guiManager->GetContext()->m_wind->GetRenderWindow()));
    if (m_beingMoved && m_moveMouseLast != mousePos){
        sf::Vector2f difference = mousePos - m_moveMouseLast;
        m_moveMouseLast = mousePos;
        sf::Vector2f newPosition = m_position + difference;
        SetPosition(newPosition);
    }
    for (auto &itr : m_elements){

        if (itr.second->NeedsRedraw()){
            if (itr.second->IsControl()){
                m_controlRedraw = true; 
            }else { 
                m_contentRedraw = true; 
            }
        }

        if (!itr.second->IsActive()){ 
            continue; 
        }
        itr.second->Update(l_dT);
        if (m_beingMoved){ continue; }
        GUI_Event event;
        event.m_interface = m_name.c_str();
        event.m_element = itr.second->m_name.c_str();
        event.m_clickCoords.x = mousePos.x;
        event.m_clickCoords.y = mousePos.y;
        if (
            IsInside(mousePos) && 
            itr.second->IsInside(mousePos) && 
            !m_titleBar.getGlobalBounds().contains(mousePos)
            ) {
            if (itr.second->GetState() != GUI_ElementState::Neutral){
                continue;
            }
            itr.second->OnHover(mousePos);
            event.m_type = GUI_EventType::Hover;
        } else if (itr.second->GetState() == GUI_ElementState::Focused){
            itr.second->OnLeave();
            event.m_type = GUI_EventType::Leave;
        }
        m_guiManager->AddEvent(event);
    }
}


void GUI_Interface::Draw(sf::RenderTarget* l_target){
    l_target->draw(m_backdrop);
    l_target->draw(m_content);
    l_target->draw(m_control);
    if (!m_showTitleBar){ 
        return; 
    }
    l_target->draw(m_titleBar);
    l_target->draw(m_visual.m_text);
}

void GUI_Interface::BeginMoving(){
    if (!m_showTitleBar || !m_movable){ return; }
    m_beingMoved = true;
    SharedContext* context = m_guiManager->GetContext();
    m_moveMouseLast = sf::Vector2f(context->m_eventManager->GetMousePos(context->m_wind->GetRenderWindow()));
}

void GUI_Interface::StopMoving(){ m_beingMoved = false; }

sf::Vector2f GUI_Interface::GetGlobalPosition() const{
    sf::Vector2f pos = m_position;
    GUI_Interface* i = m_parent;

    while (i){
        pos += i->GetPosition();
        i = i->m_parent;
    }

    return pos;
}

void GUI_Interface::ApplyStyle(){
    GUI_Element::ApplyStyle(); // Call base method.
    m_visual.m_backgroundSolid.setPosition(0.f,0.f);
    m_visual.m_backgroundImage.setPosition(0.f,0.f);
    m_titleBar.setSize(sf::Vector2f(m_style[m_state].m_size.x, 16.f));
    m_titleBar.setPosition(m_position.x,m_position.y - m_titleBar.getSize().y);
    m_titleBar.setFillColor(m_style[m_state].m_elementColor);
    m_visual.m_text.setPosition(m_titleBar.getPosition()+ m_style[m_state].m_textPadding);
    m_visual.m_glyph.setPosition(m_titleBar.getPosition()+ m_style[m_state].m_glyphPadding);
}

void GUI_Interface::Redraw(){
    if (
        m_backdropTexture->getSize().x!=m_style[m_state].m_size.x ||
        m_backdropTexture->getSize().y != m_style[m_state].m_size.y
        ){
        m_backdropTexture->create(m_style[m_state].m_size.x,
        m_style[m_state].m_size.y);
    }
    m_backdropTexture->clear(sf::Color(0, 0, 0, 0));
    ApplyStyle();
    m_backdropTexture->draw(m_visual.m_backgroundSolid);
    if (m_style[m_state].m_backgroundImage != ""){
        m_backdropTexture->draw(m_visual.m_backgroundImage);
    }
    m_backdropTexture->display();
    m_backdrop.setTexture(m_backdropTexture->getTexture());
    m_backdrop.setTextureRect(sf::IntRect(0, 0, m_style[m_state].m_size.x, m_style[m_state].m_size.y));
    SetRedraw(false);
}

void GUI_Interface::RedrawContent(){
    if (
        m_contentTexture->getSize().x != m_contentSize.x ||
        m_contentTexture->getSize().y != m_contentSize.y
            ){
    m_contentTexture->create(m_contentSize.x, m_contentSize.y);
    }
    m_contentTexture->clear(sf::Color(0, 0, 0, 0));
    for (auto &itr : m_elements){
        GUI_Element* element = itr.second;
        if (!element->IsActive() || element->IsControl()){ continue; }
        element->ApplyStyle();
        element->Draw(m_contentTexture);
        element->SetRedraw(false);
    }
    m_contentTexture->display();
    m_content.setTexture(m_contentTexture->getTexture());
    m_content.setTextureRect(
        sf::IntRect(
            m_scrollHorizontal, 
            m_scrollVertical,
            m_style[m_state].m_size.x, 
            m_style[m_state].m_size.y
            )
        );
    m_contentRedraw = false;
}

void GUI_Interface::RedrawControls(){
    if (
        m_controlTexture->getSize().x!=m_style[m_state].m_size.x ||
        m_controlTexture->getSize().y != m_style[m_state].m_size.y
        ){
        m_controlTexture->create(m_style[m_state].m_size.x,
        m_style[m_state].m_size.y);
    }
    m_controlTexture->clear(sf::Color(0, 0, 0, 0));
    for (auto &itr : m_elements){
        GUI_Element* element = itr.second;
        if (!element->IsActive() || !element->IsControl()){ continue; }
        element->ApplyStyle();
        element->Draw(m_controlTexture);
        element->SetRedraw(false);
    }
    m_controlTexture->display();
    m_control.setTexture(m_controlTexture->getTexture());
    m_control.setTextureRect(sf::IntRect(0, 0,
    m_style[m_state].m_size.x, m_style[m_state].m_size.y));
    m_controlRedraw = false;
}

void GUI_Interface::UpdateScrollHorizontal(unsigned int l_percent)
{
    if (l_percent > 100){ return; }
    m_scrollHorizontal = ((m_contentSize.x - GetSize().x) / 100) *l_percent;
    sf::IntRect rect = m_content.getTextureRect();
    m_content.setTextureRect(
        sf::IntRect(
            m_scrollHorizontal, 
            m_scrollVertical,
            rect.width,
            rect.height
            )
        );
}

void GUI_Interface::UpdateScrollVertical(unsigned int l_percent){
    if (l_percent > 100){ return; }
    m_scrollVertical = ((m_contentSize.y - GetSize().y) / 100) *l_percent;
    sf::IntRect rect = m_content.getTextureRect();
    m_content.setTextureRect(
        sf::IntRect(
            m_scrollHorizontal, 
            m_scrollVertical,
            rect.width,
            rect.height
            )
        );
}

void GUI_Interface::AdjustContentSize(const GUI_Element* l_reference)
    {
    if (l_reference){
        sf::Vector2f bottomRight = l_reference->GetPosition() + l_reference->GetSize();
        if (bottomRight.x > m_contentSize.x){
            m_contentSize.x = bottomRight.x;
            m_controlRedraw = true;
        }
        if (bottomRight.y > m_contentSize.y){
            m_contentSize.y = bottomRight.y;
            m_controlRedraw = true;
        }
        return;
    }
    sf::Vector2f farthest = GetSize();
    for (auto &itr : m_elements){
        GUI_Element* element = itr.second;
        if (!element->IsActive() || element->IsControl()){ continue; }
        sf::Vector2f bottomRight = element->GetPosition() + element->GetSize();
        if (bottomRight.x > farthest.x){
            farthest.x = bottomRight.x;
            m_controlRedraw = true;
        }
        if (bottomRight.y > farthest.y){
            farthest.y = bottomRight.y;
            m_controlRedraw = true;
        }
    }
    SetContentSize(farthest);
}

void GUI_Interface::SetContentSize(const sf::Vector2f& l_vec){
    m_contentSize = l_vec;
}

GUI_Manager::GUI_Manager(EventManager* l_evMgr,SharedContext* l_shared): 
m_eventMgr(l_evMgr),
m_context(l_shared), 
m_currentState(StateType(0))
{
    RegisterElement<GUI_Label>(GUI_ElementType::Label);
    RegisterElement<GUI_Scrollbar>(GUI_ElementType::Scrollbar);
    RegisterElement<GUI_Textfield>(GUI_ElementType::Textfield);

    m_elemTypes.emplace("Label", GUI_ElementType::Label);
    m_elemTypes.emplace("Button", GUI_ElementType::Button);
    m_elemTypes.emplace("Scrollbar", GUI_ElementType::Scrollbar);
    m_elemTypes.emplace("TextField", GUI_ElementType::Textfield);
    m_elemTypes.emplace("Interface", GUI_ElementType::Window);

    m_eventMgr->AddCallback(StateType(0),"Mouse_Left", &GUI_Manager::HandleClick, this);
    m_eventMgr->AddCallback(StateType(0),"Mouse_Left_Release", &GUI_Manager::HandleRelease, this);
    m_eventMgr->AddCallback(StateType(0),"Text_Entered", &GUI_Manager::HandleTextEntered, this);
}

GUI_Manager::~GUI_Manager(){
    m_eventMgr->RemoveCallback(StateType(0), "Mouse_Left");
    m_eventMgr->RemoveCallback(StateType(0), "Mouse_Left_Release");
    m_eventMgr->RemoveCallback(StateType(0), "Text_Entered");
    for (auto &itr : m_interfaces){
        for (auto &itr2 : itr.second){
            delete itr2.second;
        }
    }
}

bool GUI_Manager::AddInterface(const StateType& l_state, const std::string& l_name)
{

    std::cout << "ADDInterface" << std::endl;
    auto s = m_interfaces.emplace(l_state, GUI_Interfaces()).first;

    std::cout << m_interfaces.size() << std::endl;

    GUI_Interface* temp = new GUI_Interface(l_name, this);
    std::cout << m_interfaces.size() << std::endl;
    if (s->second.emplace(l_name, temp).second){ return true; }
    std::cout << m_interfaces.size() << std::endl;
    delete temp;
    std::cout << m_interfaces.size() << std::endl;
    return false;
}

GUI_Interface* GUI_Manager::GetInterface(const StateType& l_state, const std::string& l_name)
{
    auto s = m_interfaces.find(l_state);
    if (s == m_interfaces.end()){ return nullptr; }
    auto i = s->second.find(l_name);
    return (i != s->second.end() ? i->second : nullptr);
}

bool GUI_Manager::RemoveInterface(const StateType& l_state,const std::string& l_name)
    {
    auto s = m_interfaces.find(l_state);
    if (s == m_interfaces.end()){ return false; }
    auto i = s->second.find(l_name);
    if (i == s->second.end()){ return false; }
    delete i->second;
    return s->second.erase(l_name);
}

void GUI_Manager::SetCurrentState(const StateType& l_state){
    if (m_currentState == l_state){ return; }
    HandleRelease(nullptr);
    m_currentState = l_state;
}

void GUI_Manager::HandleClick(EventDetails* l_details __attribute__((unused))){
    auto state = m_interfaces.find(m_currentState);
    if (state == m_interfaces.end()){ return; }
    sf::Vector2i mousePos = m_eventMgr->GetMousePos(m_context->m_wind->GetRenderWindow());
    for (auto itr = state->second.begin(); itr != state->second.end(); ++itr)
    {
        if (!itr->second->IsInside(sf::Vector2f(mousePos))){continue;}
        if (!itr->second->IsActive()){ return; }
        itr->second->OnClick(sf::Vector2f(mousePos));
        itr->second->Focus();
        if (itr->second->IsBeingMoved()){itr->second->BeginMoving();}
        return;
    }
}

void GUI_Manager::HandleRelease(EventDetails* l_details __attribute__((unused))){
    auto state = m_interfaces.find(m_currentState);
    if (state == m_interfaces.end()){ return; }
    for (auto &itr : state->second){
        GUI_Interface* i = itr.second;
        if (!i->IsActive()){ continue; }
        if (i->GetState() == GUI_ElementState::Clicked){
            i->OnRelease();
        }
        if (i->IsBeingMoved()){ i->StopMoving(); }
    }
}

void GUI_Manager::HandleTextEntered(EventDetails* l_details){
    auto state = m_interfaces.find(m_currentState);
    if (state == m_interfaces.end()){ return; }
    for (auto &itr : state->second){
        if (!itr.second->IsActive()){ continue; }
        if (!itr.second->IsFocused()){ continue; }
        itr.second->OnTextEntered(l_details->m_textEntered);
        return;
    }
}

void GUI_Manager::AddEvent(GUI_Event l_event){
    m_events[m_currentState].push_back(l_event);
}

bool GUI_Manager::PollEvent(GUI_Event& l_event){
    if (m_events[m_currentState].empty()){ return false; }
        l_event = m_events[m_currentState].back();
        m_events[m_currentState].pop_back();
    return true;
}

void GUI_Manager::Update(float l_dT){
    sf::Vector2i mousePos = m_eventMgr->GetMousePos(m_context->m_wind->GetRenderWindow());
    auto state = m_interfaces.find(m_currentState);
    if (state == m_interfaces.end()){ return; }
    for (auto itr = state->second.begin(); itr != state->second.end(); ++itr){
        GUI_Interface* i = itr->second;
        if (!i->IsActive()){ continue; }
        i->Update(l_dT);
        if (i->IsBeingMoved()){ continue; }
        if (i->IsInside(sf::Vector2f(mousePos))){
            if (i->GetState() == GUI_ElementState::Neutral){
            i->OnHover(sf::Vector2f(mousePos));
            }
            return;
        } else if (i->GetState() == GUI_ElementState::Focused){
            i->OnLeave();
        }
    }
}

void GUI_Manager::Render(sf::RenderWindow* l_wind){

                sf::CircleShape targetDot;
            targetDot.setRadius(6);
            targetDot.setPosition(sf::Vector2f(30,65));
            targetDot.setFillColor(sf::Color::Magenta);
            targetDot.setOutlineColor(sf::Color::Magenta);  

   //          l_wind->draw(targetDot);

    auto state = m_interfaces.find(m_currentState);

    if (state == m_interfaces.end()){ 
        return; 
    }
    for (auto &itr : state->second){
        GUI_Interface* i = itr.second;
        if (!i->IsActive()){ continue; }
        if (i->NeedsRedraw()){ i->Redraw(); }
        if (i->NeedsContentRedraw()){ i->RedrawContent(); }
        if (i->NeedsControlRedraw()){ i->RedrawControls(); }
        i->Draw(l_wind);
    }
}

GUI_Element* GUI_Manager::CreateElement( const GUI_ElementType& l_id, GUI_Interface* l_owner)
{
    if (l_id == GUI_ElementType::Window){
    return new GUI_Interface("", this);
    }
    auto f = m_factory.find(l_id);
    return (f != m_factory.end() ? f->second(l_owner) : nullptr);
}

GUI_ElementType GUI_Manager::StringToType(const std::string& l_string){
    
    if(l_string == "Window"){
        return GUI_ElementType::Window;
    }else if(l_string == "Label"){
        return GUI_ElementType::Label;
    }else if(l_string == "Button"){
        return GUI_ElementType::Button;
    }else if(l_string == "Scrollbar"){
        return GUI_ElementType::Scrollbar;
    }else if(l_string == "Textfield"){
        return GUI_ElementType::Textfield;
    }else{
        return GUI_ElementType::None;
    }
}
    


bool GUI_Manager::LoadInterface(
    const StateType& l_state,
    const std::string& l_interface __attribute__((unused)), 
    const std::string& l_name
    )
{


    std::ifstream file;
    file.open(Utils::GetWorkingDirectory() + "config/MM.gui" );
    if( !file.is_open())
    {
        std::cout << "GUIManager :: Failed to load Interface" << std::endl;
        return false;
    }else{

        std::string line;

        std::string InterfaceName;
    //    std::cout << (unsigned int)l_state << " " << l_name << std::endl;

        while(std::getline(file,line)){

            std::string key;
            std::stringstream keystream(line);
            keystream >> key ;

        //    std::cout << key << std::endl;

            if (key == "Interface"){
                std::string style;
                keystream >> InterfaceName >> style;
                if (!AddInterface(l_state, l_name)){
                    std::cout << "Failed adding interface: " << l_name << std::endl;
                    return false;   
                }
                GUI_Interface* i = GetInterface(l_state, l_name);
                keystream >> *i;

                if (!LoadStyle(style, i)){
                    std::cout << "Failed loading style file: "  << style << " for interface " << l_name << std::endl;
                }

                i->SetContentSize(i->GetSize());
            } else if(key == "Element"){
                if (InterfaceName == ""){
                    std::cout << "Error: 'Element' outside or before declaration of 'Interface'!" << std::endl;
                    continue;
                }
                std::string type;
                std::string name;
                sf::Vector2f position;
                std::string style;
                keystream >> type >> name >> position.x >> position.y >> style;
                GUI_ElementType eType = StringToType(type);
                if (eType == GUI_ElementType::None){
                    std::cout << "Unknown element('" << name << "') type: '" << type << "'" << std::endl;
                    continue;
                }
                GUI_Interface* i = GetInterface(l_state, l_name);
                if (!i){ continue; }
                if (!i->AddElement(eType, name)){ continue; }
                GUI_Element* e = i->GetElement(name);
                keystream >> *e;
                e->SetPosition(position);
                if (!LoadStyle(style, e)){
                    std::cout << "Failed loading style file: " << style << " for element " << name << std::endl;
                continue;
                }
            }
        }
    }
}

SharedContext* GUI_Manager::GetContext(){
    return m_context;
}
    
const sf::Vector2f& GUI_Interface::GetPadding() const{
   return m_elementPadding;
}

void GUI_Interface::Focus(){
    ///////////////////////
    m_focused = !m_focused;
}

bool GUI_Manager::LoadStyle(const std::string& l_file, GUI_Element* l_element)
{

   // std::cout << " Loading Style : " << l_file << std::endl;

    std::ifstream file;
    file.open(Utils::GetWorkingDirectory() + "config/" + l_file);
    if( !file.is_open())
    {

        std::cout<< std::endl << "GUIManager :: Failed to load Style: " << l_file << std::endl;
    
        return false;
    }else{


        std::string line;
        std::string currentState;
        GUI_Style ParentStyle;
        GUI_Style TemporaryStyle;
        std::string type;
        while(std::getline(file,line)){

            
            std::stringstream keystream(line);
            keystream >> type;

            std::cout << type << " ";

            if (type == "State"){
                if (currentState != ""){
                    std::cout<< std::endl << "Error: 'State' keyword found inside another state!" << std::endl;
                    continue;
                }
                keystream >> currentState;
            } else if(type == "/State"){
                if (currentState == ""){
                    std::cout<< std::endl << "Error: '/State' keyword found prior to 'State'!" << std::endl;
                    continue;
                }
                GUI_ElementState state = GUI_ElementState::Neutral;
                if (currentState == "Hover"){
                    state = GUI_ElementState::Focused;
                }else if (currentState == "Clicked"){
                    state = GUI_ElementState::Clicked;
                }
                if (state == GUI_ElementState::Neutral){
                    ParentStyle = TemporaryStyle;
                    l_element->UpdateStyle(
                    GUI_ElementState::Neutral, TemporaryStyle);
                    l_element->UpdateStyle(
                    GUI_ElementState::Focused, TemporaryStyle);
                    l_element->UpdateStyle(
                    GUI_ElementState::Clicked, TemporaryStyle);
                } else {
                    l_element->UpdateStyle(state, TemporaryStyle);
                }
                    TemporaryStyle = ParentStyle;
                    currentState = "";
            } else {



                if (currentState == ""){
                    std::cout<< std::endl << "Error: '" << type << "' keyword found outside of a state!" << std::endl;
                    continue;
                }
                if (type == "Size"){
                    keystream >>TemporaryStyle.m_size.x >>TemporaryStyle.m_size.y;
                } else if (type == "BgColor"){
                    int r, g, b, a = 0;
                    keystream >> r >> g >> b >> a;
                    TemporaryStyle.m_backgroundColor = sf::Color(r,g,b,a);
                } else if (type == "BgImage"){
                    keystream >> TemporaryStyle.m_backgroundImage;
                } else if (type == "BgImageColor"){
                    int r, g, b, a = 0;
                    keystream >> r >> g >> b >> a;
                    TemporaryStyle.m_backgroundImageColor = sf::Color(r, g, b, a);
                } else if (type == "TextColor"){
                    int r, g, b, a = 0;keystream >> r >> g >> b >> a;
                    TemporaryStyle.m_textColor = sf::Color(r, g, b, a);
                }else if (type == "TextSize"){
                    keystream >> TemporaryStyle.m_textSize;
                }else if (type == "TextOriginCenter"){
                    TemporaryStyle.m_textCenterOrigin = true;
                }else if (type == "Font"){
                    keystream >> TemporaryStyle.m_textFont;
                }else if (type == "TextPadding"){
                    keystream >> TemporaryStyle.m_textPadding.x >> TemporaryStyle.m_textPadding.y;
                }else if (type == "ElementColor"){
                    int r, g, b, a = 0;
                    keystream >> r >> g >> b >> a;
                    TemporaryStyle.m_elementColor = sf::Color(r, g, b, a);
                }else if (type == "Glyph"){
                    keystream >> TemporaryStyle.m_glyph;
                }else if (type == "GlyphPadding"){
                    keystream >> TemporaryStyle.m_glyphPadding.x >> TemporaryStyle.m_glyphPadding.y;
                }else {
                    std::cout<< std::endl << "Error: style tag '" << type << "' is unknown!" << std::endl;
                } 
            }
        }
   //     std::cout << std::endl;
        return true;

    }
}


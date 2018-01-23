#ifndef C_CONTROLLER
#define C_CONTROLLER

enum class ControllingPerson{
    Player = 0,
    Computer
};

enum class UnitType{
    Unit = 0,
    Building,
    Landscape
};

class C_Controller : public C_Base{
public:
    C_Controller() : C_Base(Component::Controller){}
    void ReadIn(std::stringstream& l_stream){

        int temp;
        int temp2;
        l_stream >> temp >> temp2;
        m_controller = static_cast<ControllingPerson>(temp);
        m_active = false;
        m_type = static_cast<UnitType>(temp2);
        
    }

    ControllingPerson GetControllingPerson(){
        return m_controller;
    }

    void SetControllingPerson( ControllingPerson& l_cont){
        m_controller = l_cont;
    }

    void SetActive( const bool l_active){
        m_active = l_active;
    }

    bool IsActive(){
        return m_active;
    }


    bool IsBuilding(){
        return m_type == UnitType::Building;
    }

    bool IsUnit(){
        return m_type == UnitType::Unit;
    }

    bool IsLanscape(){
        return m_type == UnitType::Landscape;
    }

private:
    ControllingPerson m_controller;
    bool m_active;
    UnitType m_type;
};


#endif
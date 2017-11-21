#ifndef C_CONTROLLER
#define C_CONTROLLER

enum class ControllingPerson{
    Player = 0,
    Computer
};

class C_Controller : public C_Base{
public:
    C_Controller() : C_Base(Component::Controller){}
    void ReadIn(std::stringstream& l_stream){

        int temp;
        l_stream >> temp;
        m_controller = static_cast<ControllingPerson>(temp);
        m_active = false;
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

private:
    ControllingPerson m_controller;
    bool m_active;
};


#endif
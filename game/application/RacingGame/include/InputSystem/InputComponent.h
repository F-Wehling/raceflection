#pragma once

#include "ObjectSystem/GameObjectComponent.h"

#include "InputSystem/Trigger.h"
#include "InputSystem/InputDevice.h"

BEGINNAMESPACE

class GameObject;

class InputComponent : public GameObjectComponent {
public:
    inline InputComponent(InputDevice device);
    virtual ~InputComponent();

    virtual bool process(float32 dt,GameObject*);
protected:
    InputDevice m_InputDevice;
};

///
/// Sample InputComponent
///

class InputWASDComponent : public InputComponent {
public:
    InputWASDComponent(InputDevice device);
    ~InputWASDComponent();

    virtual bool process(float32 dt, GameObject * object);
private:
    Trigger::ID m_FwdTrigger;
    Trigger::ID m_LeftTrigger;
    Trigger::ID m_BackTrigger;
    Trigger::ID m_RightTrigger;
    Trigger::ID m_UpTrigger;
    Trigger::ID m_DownTrigger;
    Trigger::ID m_MouseButtonTrigger;
	Trigger::ID m_TurnLeft;
	Trigger::ID m_TurnRight;
    Trigger::ID m_TurnClockwise;
    Trigger::ID m_TurnCounterClockwise;
	Trigger::ID m_TurnUp;
	Trigger::ID m_TurnDown;
    Trigger::ID m_Nitro;
    Trigger::ID m_Reset;
};

ENDNAMESPACE

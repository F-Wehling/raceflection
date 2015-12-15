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
};

ENDNAMESPACE

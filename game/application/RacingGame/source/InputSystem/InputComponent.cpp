#include "InputSystem/InputComponent.h"
#include "InputSystem/InputDevice.h"

#include "Configuration/ConfigSettings.h"

#include "ObjectSystem/GameObject.h"

BEGINNAMESPACE

InputComponent::InputComponent(InputDevice device) :
    m_InputDevice(device)
{}

InputComponent::~InputComponent(){}

bool InputComponent::process(float32 dt, GameObject *){ return true; }


///
/// Sample Input Component
///

ConfigSettingFloat32 cfgInputVelocity("InputVelocity", "Sets the input velocity in m/s.", 5.0f);
ConfigSettingFloat32 cfgMouseSensitivity("MouseSensivity", "Sets the mouse sensivity", 20.0f);

InputWASDComponent::InputWASDComponent(InputDevice device) :
    InputComponent(device)
{
    m_FwdTrigger = m_InputDevice.addTrigger(
                    &Or<
                        &Key::IsPressed<Keyboard::Code::key_W>,
                        &Key::IsPressed<Keyboard::Code::key_UP>
                    >
                );

    m_BackTrigger = m_InputDevice.addTrigger(
                    &Or<
                        &Key::IsPressed<Keyboard::Code::key_S>,
                        &Key::IsPressed<Keyboard::Code::key_DOWN>
                    >
                );

    m_LeftTrigger = m_InputDevice.addTrigger(
                    &Or<
                        &Key::IsPressed<Keyboard::Code::key_A>,
                        &Key::IsPressed<Keyboard::Code::key_LEFT>
                    >
                );

    m_RightTrigger = m_InputDevice.addTrigger(
                    &Or<
                        &Key::IsPressed<Keyboard::Code::key_D>,
                        &Key::IsPressed<Keyboard::Code::key_RIGHT>
                    >
                );

    m_UpTrigger = m_InputDevice.addTrigger(
                        &Key::IsPressed<Keyboard::Code::key_E>
                );

    m_DownTrigger = m_InputDevice.addTrigger(
                        &Key::IsPressed<Keyboard::Code::key_Q>
                );

    m_MouseButtonTrigger = m_InputDevice.addTrigger(
                        &MouseButton::IsPressed<Mouse::Button::Right>
                );
}

InputWASDComponent::~InputWASDComponent(){}

bool InputWASDComponent::process(float32 dt, GameObject *object){
    static glm::vec3 lastMousePosition = glm::vec3(-1.0, -1.0, -1.0);
    glm::vec3 pos = object->getPosition();

    float32 frac = cfgInputVelocity * 1000.0 / dt;
    if(m_InputDevice.isTriggered(m_FwdTrigger)){
        pos += frac * object->getForward();
    }
    if(m_InputDevice.isTriggered(m_BackTrigger)){
        pos += frac * object->getBackward();
    }
    if(m_InputDevice.isTriggered(m_LeftTrigger)){
        pos += frac * object->getLeft();
    }
    if(m_InputDevice.isTriggered(m_RightTrigger)){
        pos += frac * object->getRight();
    }
    if(m_InputDevice.isTriggered(m_UpTrigger)){
        pos += frac * object->getUp();
    }
    if(m_InputDevice.isTriggered(m_DownTrigger)){
        pos += frac * object->getDown();
    }
    object->setPosition(pos);

    Point3f p = m_InputDevice.getMouse()->position();
    if(m_InputDevice.isTriggered(m_MouseButtonTrigger)){
        //rotation implementation here
    }

    lastMousePosition = glm::vec3(p.x, p.y, p.z);
}


ENDNAMESPACE

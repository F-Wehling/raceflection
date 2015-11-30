#include "InputSystem/InputDevice.h"
//
/// Input Device

BEGINNAMESPACE

InputDevice::InputDevice() :
	m_NumTriggerChecker(0),
	m_Keyboard(nullptr),
	m_Mouse(nullptr),
	m_Joystick(nullptr)
{}

Trigger::ID InputDevice::addTrigger(OnTriggerCheck callback)
{
	ASSERT(m_NumTriggerChecker < MaxTriggerChecker, "Can't add more than %d Trigger-Checking Functions to a device.", MaxTriggerChecker);
	Trigger::ID id = m_NumTriggerChecker++;
	m_TriggerChecker[id] = callback;
	return id;
}

bool InputDevice::isTriggered(Trigger::ID triggerId)
{
	ASSERT(triggerId < m_NumTriggerChecker, "For TriggerId %d is no trigger registred.", triggerId);
	return m_TriggerChecker[triggerId](this);
}

ENDNAMESPACE
#include "InputSystem/Joystick.h"

#include "OIS/OISJoyStick.h"

BEGINNAMESPACE

bool Joystick::isPressed(const ButtonID joystickButton)
{
	return m_Joystick->getJoyStickState().mButtons[joystickButton];
}

bool Joystick::isReleased(const ButtonID joystickButton)
{
	return !m_Joystick->getJoyStickState().mButtons[joystickButton];
}

bool Joystick::wentDown(const ButtonID joystickButton)
{
	return  m_Joystick->getJoyStickState().mButtons[joystickButton] && ((m_LastButtonState & (1L << joystickButton)) == 0);
}

bool Joystick::wentUp(const ButtonID joystickButton)
{
	return  !m_Joystick->getJoyStickState().mButtons[joystickButton] && ((m_LastButtonState & (1L << joystickButton)) != 0);
}

Joystick::PovID Joystick::getPOV(uint32 povIdx)
{
	ASSERT(povIdx < 4, "Joystick only support 4 POV-inputs");
	return PovID(m_Joystick->getJoyStickState().mPOV[povIdx].direction);
}

float32 Joystick::getAxis(uint32 axisIdx)
{
	ASSERT(axisIdx < (uint32)m_Joystick->getNumberOfComponents(OIS::ComponentType::OIS_Axis), "The Joystick only has %d POV-inputs", m_Joystick->getNumberOfComponents(OIS::ComponentType::OIS_Axis));
	return float32(m_Joystick->getJoyStickState().mAxes[axisIdx].abs) / float32(MaxAxisValue);
}

uint32 convert_toUint32(const std::vector<bool>& val) {
	uint32 v = 0;
	for (size_type i = 0; i < val.size(); ++i) {
		if (val[i]) {
			v |= (1 << i);
		}
	}
	return v;
}

void Joystick::capture()
{
	m_LastButtonState = convert_toUint32(m_Joystick->getJoyStickState().mButtons);
	m_Joystick->capture();
}

void Joystick::setOISJoystick(OIS::Object * keyboard)
{
	m_Joystick = static_cast<OIS::JoyStick*>(keyboard);
}

ENDNAMESPACE
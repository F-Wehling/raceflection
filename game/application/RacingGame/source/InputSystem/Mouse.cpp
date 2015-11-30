#include "InputSystem/Mouse.h"

#include "OIS/OISMouse.h"

BEGINNAMESPACE


bool Mouse::isPressed(ButtonID mouseButton)
{
	return m_Mouse->getMouseState().buttonDown(OIS::MouseButtonID(mouseButton));
}

bool Mouse::isReleased(ButtonID mouseButton)
{
	return !m_Mouse->getMouseState().buttonDown(OIS::MouseButtonID(mouseButton));
}

bool Mouse::wentDown(ButtonID mouseButton)
{
	return m_Mouse->getMouseState().buttonDown(OIS::MouseButtonID(mouseButton)) && ((m_LastButtonState & (1L << mouseButton)) == 0);
}

bool Mouse::wentUp(ButtonID mouseButton)
{
	return !m_Mouse->getMouseState().buttonDown(OIS::MouseButtonID(mouseButton)) && ((m_LastButtonState & (1L << mouseButton)) != 0);
}

Point3f Mouse::position() const
{
	Point3f pos(0,0,0);
	pos.x = float32(m_Mouse->getMouseState().X.abs) / float32(m_Mouse->getMouseState().width);
	pos.y = float32(m_Mouse->getMouseState().Y.abs) / float32(m_Mouse->getMouseState().height);
	pos.z = float32(m_Mouse->getMouseState().Z.abs) / float32(std::numeric_limits<uint16>::max());
	return pos;
}

void Mouse::capture()
{
	m_LastButtonState = m_Mouse->getMouseState().buttons;
	m_Mouse->capture();
}

void Mouse::setOISMouse(OIS::Object * mouse)
{
	m_Mouse = static_cast<OIS::Mouse*>(mouse);
}



ENDNAMESPACE
#include "InputSystem/Keyboard.h"

#include "OIS/OISKeyboard.h"

BEGINNAMESPACE

bool Keyboard::wentDown(KeyCode keyCode)
{
	return !m_KeyboardRef->isKeyDown(OIS::KeyCode(keyCode)) && m_LastState[keyCode] != 0;
}

bool Keyboard::wentUp(KeyCode keyCode)
{
	return m_KeyboardRef->isKeyDown(OIS::KeyCode(keyCode)) && m_LastState[keyCode] == 0;
}

bool Keyboard::isPressed(KeyCode keyCode)
{
	return m_KeyboardRef->isKeyDown(OIS::KeyCode(keyCode));
}

bool Keyboard::isReleased(KeyCode keyCode)
{
	return !m_KeyboardRef->isKeyDown(OIS::KeyCode(keyCode));
}

void Keyboard::capture()
{
	m_KeyboardRef->copyKeyStates((char*)m_LastState); //safe last state
	m_KeyboardRef->capture(); //and get current
}

void Keyboard::setOISKeyboard(OIS::Object * keyboard)
{
	m_KeyboardRef = static_cast<OIS::Keyboard*>(keyboard);
}

ENDNAMESPACE

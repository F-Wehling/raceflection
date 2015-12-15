#pragma once

#include "WindowSystem/WindowDesc.h"
#include "InputSystem/Keyboard.h"
#include "InputSystem/Mouse.h"
#include "InputSystem/Joystick.h"
#include "InputSystem/InputDevice.h"
#include "InputSystem/InputComponent.h"

namespace OIS {
	class InputManager;
}

BEGINNAMESPACE

/*
* The input system
*/

class Window;
class InputSystem {	
private:
	static const uint32 captureEveryNMilliseconds = 30;

	static const size_type MaxKeyboardPerWindow = 5;
	static const size_type MaxMousePerWindow = 5;
	static const size_type MaxJoystickPerWindow = 10;
	static const size_type MaxInputDevices = MaxKeyboardPerWindow + MaxMousePerWindow + MaxJoystickPerWindow;
public:
	struct InputWindowMapping {
		OIS::InputManager* m_InputManager;
		Window* window;
		uint32 numKeyboard;
		uint32 numMouse;
		uint32 numJoystick;
		Keyboard m_Keyboard[MaxKeyboardPerWindow];
		Mouse m_Mouse[MaxMousePerWindow];
		Joystick m_Joystick[MaxJoystickPerWindow];
	};
public:
	InputSystem();
    ~InputSystem();
	bool initialize();
	void shutdown();

	int32 attachWindow(Window* window);

	bool tick(float32 dt);

	Keyboard* getKeyboard(uint32 keyboardIdx = 0, uint32 windowIdx = 0);
	Mouse* getMouse(uint32 mouseIdx = 0, uint32 windowIdx = 0);
	Joystick* getJoystick(uint32 joystickIdx = 0, uint32 windowIdx = 0);

	inline void setCaptureEveryMilliseconds(float32 millisec) { m_CaptureEveryMilliseconds = millisec;  }
	inline float32 getCaputreEveryMilliseconds() const { return m_CaptureEveryMilliseconds; }
private:
	bool initializeKeyboard(InputWindowMapping& mapping, const String& vendor);
	bool initializeMouse(InputWindowMapping& mapping, const String& vendor);
	bool initializeJoystick(InputWindowMapping& mapping, const String& vendor);

private:
	InputWindowMapping m_InputWindowMapping[WindowDesc::MaxWindows]; //as we can have at most MaxWindow windows, we can only have MaxWindow input->window mappings
	uint32 m_NumInputWindowMappings;
	float32 m_LastCaptured;
	float32 m_CaptureEveryMilliseconds;
};

ENDNAMESPACE

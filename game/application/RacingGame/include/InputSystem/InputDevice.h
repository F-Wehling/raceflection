#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Joystick.h"

#include "Trigger.h"
#include "Configuration/ConfigSettings.h"

BEGINNAMESPACE

class Joystick;

//
/// An input device is a combination of at most one Keyboard, one mouse and one joystick
/// 
class InputDevice {
	friend class InputSystem;
public:
	static const uint32 MaxTriggerChecker = 256;
	typedef bool(*OnTriggerCheck)(InputDevice*);
public:
	InputDevice();

	Trigger::ID addTrigger(OnTriggerCheck callback);
	bool isTriggered(Trigger::ID triggerId);

	//connect devices 
	inline void connectDevice(Keyboard* keyboard) { m_Keyboard = keyboard; }
	inline void connectDevice(Mouse* mouse) { m_Mouse = mouse; }
	inline void connectDevice(Joystick* joystick) { m_Joystick = joystick; }

	//get device
	inline Keyboard* getKeyboard() { return m_Keyboard; }
	inline Mouse* getMouse() { return m_Mouse; }
	inline Joystick* getJoystick() { return m_Joystick; }
private:
	uint32 m_NumTriggerChecker;
	OnTriggerCheck m_TriggerChecker[MaxTriggerChecker];
private:
	//The connected devices
	Keyboard* m_Keyboard;
	Mouse* m_Mouse;
	Joystick* m_Joystick;
};

template<InputDevice::OnTriggerCheck fn1, InputDevice::OnTriggerCheck fn2>
bool And(InputDevice* device) {
	return (fn1)(device) && (fn2)(device);
}

template<InputDevice::OnTriggerCheck fn1, InputDevice::OnTriggerCheck fn2, InputDevice::OnTriggerCheck fn3>
bool And(InputDevice* device) {
	return (fn1)(device) && (fn2)(device) && (fn3)(device);
}

template<InputDevice::OnTriggerCheck fn1, InputDevice::OnTriggerCheck fn2, InputDevice::OnTriggerCheck fn3, InputDevice::OnTriggerCheck fn4>
bool And(InputDevice* device) {
	return (fn1)(device) && (fn2)(device) && (fn3)(device) && (fn4)(device);
}

template<InputDevice::OnTriggerCheck fn1, InputDevice::OnTriggerCheck fn2>
bool Or(InputDevice* device) {
	return (fn1)(device) || (fn2)(device);
}

template<InputDevice::OnTriggerCheck fn1, InputDevice::OnTriggerCheck fn2, InputDevice::OnTriggerCheck fn3, InputDevice::OnTriggerCheck fn4>
bool Or(InputDevice* device) {
	return (fn1)(device) || (fn2)(device) || (fn3)(device) || (fn4)(device);
}

namespace Key {
	template<Keyboard::KeyCode key>
	inline bool IsPressed(InputDevice* device) {
		ASSERT(device->getKeyboard() != nullptr, "Use a Key-Trigger for an inputdevice where no keyboard is attached.");
		return device->getKeyboard()->isPressed(key);
	}

	template<Keyboard::KeyCode key>
	inline bool IsReleased(InputDevice* device) {
		ASSERT(device->getKeyboard() != nullptr, "Use a Key-Trigger for an inputdevice where no keyboard is attached.");
		return device->getKeyboard()->isReleased(key);
	}

	template<Keyboard::KeyCode key>
	inline bool WentDown(InputDevice* device) {
		ASSERT(device->getKeyboard() != nullptr, "Use a Key-Trigger for an inputdevice where no keyboard is attached.");
		return device->getKeyboard()->wentUp(key);
	}

	template<Keyboard::KeyCode key>
	inline bool WentUp(InputDevice* device) {
		ASSERT(device->getKeyboard() != nullptr, "Use a Key-Trigger for an inputdevice where no keyboard is attached.");
		return device->getKeyboard()->wentDown(key);
	}
}

namespace MouseButton {
	template<Mouse::ButtonID btn>
	inline bool IsPressed(InputDevice* device) {
		ASSERT(device->getMouse() != nullptr, "Use a MouseButton-Trigger for an inputdevice where no mouse is attached.");
		return device->getMouse()->isPressed(btn);
	}
	template<Mouse::ButtonID btn>
	inline bool IsReleased(InputDevice* device) {
		ASSERT(device->getMouse() != nullptr, "Use a MouseButton-Trigger for an inputdevice where no mouse is attached.");
		return device->getMouse()->isReleased(btn);
	}

	template<Mouse::ButtonID btn>
	inline bool WentDown(InputDevice* device) {
		ASSERT(device->getMouse() != nullptr, "Use a MouseButton-Trigger for an inputdevice where no mouse is attached.");
		return device->getMouse()->wentDown(btn);
	}

	template<Mouse::ButtonID btn>
	inline bool WentUp(InputDevice* device) {
		ASSERT(device->getMouse() != nullptr, "Use a MouseButton-Trigger for an inputdevice where no mouse is attached.");
		return device->getMouse()->wentUp(btn);
	}
}

namespace JoystickButton {
	template<Joystick::ButtonID btn>
	inline bool IsPressed(InputDevice* device) {
		ASSERT(device->getJoystick() != nullptr, "Use a Joystick-Trigger for an inputdevice where no Joystick is attached.");
		return device->getJoystick()->isPressed(btn);
	}
	template<Joystick::ButtonID btn>
	inline bool IsReleased(InputDevice* device) {
		ASSERT(device->getJoystick() != nullptr, "Use a Joystick-Trigger for an inputdevice where no Joystick is attached.");
		return device->getJoystick()->isReleased(btn);
	}

	template<Joystick::ButtonID btn>
	inline bool WentDown(InputDevice* device) {
		ASSERT(device->getJoystick() != nullptr, "Use a Joystick-Trigger for an inputdevice where no Joystick is attached.");
		return device->getJoystick()->wentDown(btn);
	}

	template<Joystick::ButtonID btn>
	inline bool WentUp(InputDevice* device) {
		ASSERT(device->getJoystick() != nullptr, "Use a Joystick-Trigger for an inputdevice where no Joystick is attached.");
		return device->getJoystick()->wentUp(btn);
	}
}

namespace ConfigKey {
	template<ConfigSettingUint32* Trigger>
	inline bool IsPressed(InputDevice* device) {
		ASSERT(device->getKeyboard() != nullptr, "Use a Key-Trigger for an inputdevice where no keyboard is attached.");
		return device->getKeyboard()->isPressed(*Trigger);
	}

	template<ConfigSettingUint32* Trigger>
	inline bool IsReleased(InputDevice* device) {
		ASSERT(device->getKeyboard() != nullptr, "Use a Key-Trigger for an inputdevice where no keyboard is attached.");
		return device->getKeyboard()->isReleased(*Trigger);
	}

	template<ConfigSettingUint32* Trigger>
	inline bool WentDown(InputDevice* device) {
		ASSERT(device->getKeyboard() != nullptr, "Use a Key-Trigger for an inputdevice where no keyboard is attached.");
		return device->getKeyboard()->wentUp(*Trigger);
	}

	template<ConfigSettingUint32* Trigger>
	inline bool WentUp(InputDevice* device) {
		ASSERT(device->getKeyboard() != nullptr, "Use a Key-Trigger for an inputdevice where no keyboard is attached.");
		return device->getKeyboard()->wentDown(*Trigger);
	}
}

namespace ConfigMouseButton {
	template<ConfigSettingUint32* Trigger>
	inline bool IsPressed(InputDevice* device) {
		ASSERT(device->getMouse() != nullptr, "Use a MouseButton-Trigger for an inputdevice where no mouse is attached.");
		return device->getMouse()->isPressed(*Trigger);
	}
	template<ConfigSettingUint32* Trigger>
	inline bool IsReleased(InputDevice* device) {
		ASSERT(device->getMouse() != nullptr, "Use a MouseButton-Trigger for an inputdevice where no mouse is attached.");
		return device->getMouse()->isReleased(*Trigger);
	}
	template<ConfigSettingUint32* Trigger>
	inline bool WentDown(InputDevice* device) {
		ASSERT(device->getMouse() != nullptr, "Use a MouseButton-Trigger for an inputdevice where no mouse is attached.");
		return device->getMouse()->wentDown(*Trigger);
	}
	template<ConfigSettingUint32* Trigger>
	inline bool WentUp(InputDevice* device) {
		ASSERT(device->getMouse() != nullptr, "Use a MouseButton-Trigger for an inputdevice where no mouse is attached.");
		return device->getMouse()->wentUp(*Trigger);
	}
}

namespace ConfigJoystickButton {
	template<ConfigSettingUint32* Trigger>
	inline bool IsPressed(InputDevice* device) {
		ASSERT(device->getJoystick() != nullptr, "Use a Joystick-Trigger for an inputdevice where no Joystick is attached.");
		return device->getJoystick()->isPressed(*Trigger);
	}
	template<ConfigSettingUint32* Trigger>
	inline bool IsReleased(InputDevice* device) {
		ASSERT(device->getJoystick() != nullptr, "Use a Joystick-Trigger for an inputdevice where no Joystick is attached.");
		return device->getJoystick()->isReleased(*Trigger);
	}
	template<ConfigSettingUint32* Trigger>
	inline bool WentDown(InputDevice* device) {
		ASSERT(device->getJoystick() != nullptr, "Use a Joystick-Trigger for an inputdevice where no Joystick is attached.");
		return device->getJoystick()->wentDown(*Trigger);
	}
	template<ConfigSettingUint32* Trigger>
	inline bool WentUp(InputDevice* device) {
		ASSERT(device->getJoystick() != nullptr, "Use a Joystick-Trigger for an inputdevice where no Joystick is attached.");
		return device->getJoystick()->wentUp(*Trigger);
	}
}

ENDNAMESPACE

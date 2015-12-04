#include "InputSystem/InputSystem.h"
#include "WindowSystem/Window.h"

#include "OIS/OIS.h"

#include "Utilities/Assert.h"
#include "Logging.h"

#include <algorithm>
#include <numeric>

BEGINNAMESPACE

InputSystem::~InputSystem(){
    shutdown();
}

InputSystem::InputSystem() : 
	m_NumInputWindowMappings(0),
	m_LastCaptured(0.0),
	m_CaptureEveryMilliseconds(captureEveryNMilliseconds)
{
}

bool InputSystem::initialize()
{
	m_NumInputWindowMappings = 0;
	m_LastCaptured = 0.0;
	return true;
}

void InputSystem::shutdown()
{
	for (uint32 i = 0; i < m_NumInputWindowMappings; ++i) { //for each mapping
		InputWindowMapping& map = m_InputWindowMapping[i];
		if (!map.m_InputManager) continue;
        for (uint32 k = 0; k < map.numKeyboard; ++k) {
            OIS::Keyboard* kb = map.m_Keyboard[k].getOISKeyboard();
            if(kb) {
				map.m_InputManager->destroyInputObject( kb );
                m_InputWindowMapping[i].m_Keyboard[k].setOISKeyboard(nullptr);
            }
        }

        for (uint32 k = 0; k < map.numMouse; ++k) {
            OIS::Mouse* m = map.m_Mouse[k].getOISMouse();
            if(m) {
				map.m_InputManager->destroyInputObject( m );
                m_InputWindowMapping[i].m_Mouse[k].setOISMouse(nullptr);
            }
        }

        for (uint32 k = 0; k < map.numJoystick; ++k) {
            OIS::JoyStick* j = map.m_Joystick[k].getOISJoystick();
            if(j){
				map.m_InputManager->destroyInputObject( j );
                m_InputWindowMapping[i].m_Joystick[k].setOISJoystick(nullptr);
            }
        }
		OIS::InputManager::destroyInputSystem(map.m_InputManager);
		map.m_InputManager = nullptr;
    }
	m_NumInputWindowMappings = 0;
}

int32 InputSystem::attachWindow(Window * window)
{
	size_type handle = size_type(window->getNativeHandle());

	OIS::ParamList paramList;
	OStringStream str;
	str << handle;

#if defined OIS_WIN32_PLATFORM
    paramList.insert(std::make_pair(std::string("WINDOW"), str.str()));
	paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND")));
	paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
	paramList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
	paramList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
#elif defined OIS_LINUX_PLATFORM
    paramList.insert(std::make_pair(std::string("WINDOW"), str.str()));
    paramList.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
    paramList.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
    paramList.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
    paramList.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
#endif

	InputWindowMapping mapping;
	std::memset(&mapping, 0, sizeof(InputWindowMapping));
	mapping.m_InputManager = OIS::InputManager::createInputSystem(paramList);

	if (!mapping.m_InputManager) {
		LOG_ERROR(General, "OIS InputMapping creation failed.");
		return -1;
	}
	OIS::DeviceList deviceList = mapping.m_InputManager->listFreeDevices();

	mapping.window = window;
	
	for (auto devList : deviceList) {
		OIS::Type type = devList.first;
		String name = devList.second;

		switch (type) {
		case OIS::Type::OISKeyboard:
			if (!initializeKeyboard(mapping, name)) {
				LOG_ERROR(General, "Keyboard initialization (for %s) failed.", name.c_str());
			}
			break;
		case OIS::Type::OISMouse:
			if (!initializeMouse(mapping, name)) {
				LOG_ERROR(General, "Mouse initialization (for %s) failed.", name.c_str());
			}
			break;
		case OIS::Type::OISJoyStick:
			if(!initializeJoystick(mapping, name)){
				LOG_ERROR(General, "Joystick initialization (for %s) failed.", name.c_str());
			}
			break;
		case OIS::Type::OISMultiTouch:
			LOG_WARNING(General, "An unsupported input device (multitouch): %s", name.c_str());
			break;
		case OIS::Type::OISTablet:
			LOG_WARNING(General, "An unsupported input device (tablet): %s", name.c_str());
			break;
		case OIS::Type::OISUnknown:
			LOG_WARNING(General, "An unknown input device: %s", name.c_str());
			break;
		}
	}
	
    if (mapping.numKeyboard == 0 && mapping.numMouse == 0 && mapping.numJoystick == 0) {
		LOG_ERROR(General, "No input devices available");
		return -1;
	}

	m_InputWindowMapping[m_NumInputWindowMappings] = mapping;

	LOG_INFO(General, "An input connection has been created for the newly created window.");
	return m_NumInputWindowMappings++;
}

bool InputSystem::tick(float32 dt)
{
	m_LastCaptured += dt;
	if (m_LastCaptured > m_CaptureEveryMilliseconds) {
		for (uint32 i = 0; i < m_NumInputWindowMappings; ++i) { //for each mapping
			//caputre keyboards
			for (uint32 k = 0; k < m_InputWindowMapping[i].numKeyboard; ++k) {
				m_InputWindowMapping[i].m_Keyboard[k].capture();
			}
			//caputre mouse
			for (uint32 k = 0; k < m_InputWindowMapping[i].numMouse; ++k) {
				 m_InputWindowMapping[i].m_Mouse[k].capture();
			}
			//caputre joystick
			for (uint32 k = 0; k < m_InputWindowMapping[i].numJoystick; ++k) {
				m_InputWindowMapping[i].m_Joystick[k].capture();
			}
		}
		m_LastCaptured = 0;
		//m_LastCaptured -= m_CaptureEveryMilliseconds;
	}
	return true;
}

bool InputSystem::initializeKeyboard(InputWindowMapping & mapping, const String& vendor)
{
	if (mapping.numKeyboard >= MaxKeyboardPerWindow) {
		LOG_WARNING(General, "Can't initialize more than %d keyboards. Ignore keyboard %s", MaxKeyboardPerWindow, vendor.c_str());
		return false;
	}
	int32 keyboardIndex = mapping.numKeyboard++;
	mapping.m_Keyboard[keyboardIndex].setOISKeyboard(mapping.m_InputManager->createInputObject(OIS::OISKeyboard, false, vendor));
	return mapping.m_Keyboard[keyboardIndex].valid();
}

bool InputSystem::initializeMouse(InputWindowMapping & mapping, const String& vendor)
{
	if (mapping.numMouse >= MaxMousePerWindow) {
		LOG_WARNING(General, "Can't initialize more than %d mouse. Ignore mouse %s", MaxMousePerWindow, vendor.c_str());
		return false;
	}
	int32 mouseIndex = mapping.numMouse++;
	mapping.m_Mouse[mouseIndex].setOISMouse(mapping.m_InputManager->createInputObject(OIS::OISMouse, false, vendor));
	return mapping.m_Mouse[mouseIndex].valid();
}

bool InputSystem::initializeJoystick(InputWindowMapping & mapping, const String& vendor)
{
	if (mapping.numJoystick >= MaxJoystickPerWindow) {
		LOG_WARNING(General, "Can't initialize more than %d joystick. Ignore joystick %s", MaxJoystickPerWindow, vendor.c_str());
		return false;
	}
	int32 joystickIndex = mapping.numJoystick++;
	mapping.m_Joystick[joystickIndex].setOISJoystick(mapping.m_InputManager->createInputObject(OIS::OISJoyStick, false, vendor));
	return mapping.m_Joystick[joystickIndex].valid();
}

Keyboard * InputSystem::getKeyboard(uint32 keyboardIdx, uint32 mappingIdx)
{
	ASSERT(mappingIdx < m_NumInputWindowMappings && keyboardIdx < m_InputWindowMapping[mappingIdx].numKeyboard, "Keyboard does not exist");
	return &m_InputWindowMapping[mappingIdx].m_Keyboard[keyboardIdx];
}

Mouse * InputSystem::getMouse(uint32 mouseIdx, uint32 mappingIdx)
{
	ASSERT(mappingIdx < m_NumInputWindowMappings && mouseIdx < m_InputWindowMapping[mappingIdx].numMouse, "Mouse does not exist");
	return &m_InputWindowMapping[mappingIdx].m_Mouse[mouseIdx];
}

Joystick * InputSystem::getJoystick(uint32 joystickIdx, uint32 mappingIdx)
{
	ASSERT(mappingIdx < m_NumInputWindowMappings && joystickIdx < m_InputWindowMapping[mappingIdx].numJoystick, "Joystick does not exist");
	return &m_InputWindowMapping[mappingIdx].m_Joystick[joystickIdx];
}


ENDNAMESPACE


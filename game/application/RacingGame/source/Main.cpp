#include "Main.h"
#include "Logging.h"

#include <chrono>

#include "MemorySystem.h"

// --- TESTING
//
#include "Configuration/ConfigSettings.h"

#if OS_WINDOWS & !SHOW_CONSOLE
#	ifndef WIN32_LEAN_AND_MEAN
#		define WIN32_LEAN_AND_MEAN
#	endif
#	include <Windows.h>
	INT WinMain(HINSTANCE, HINSTANCE, LPSTR cmdLine, INT){
		int argc = __argc;
		const char** argv = (const char**)(__argv);
#	else //If we need a console window it will be linked to int main(...)
	int main(int argc, const char* argv[]) {
#	endif
	return NS_NAME::main(argc, argv);
}
		
#include "Multithreading/JobScheduler.h"
#include "RenderSystem/RenderSystem.h"
#include "RenderSystem/RenderContext.h"
#include "WindowSystem/WindowSystem.h"
#include "InputSystem/InputSystem.h"

BEGINNAMESPACE

const tchar* sAppClassName = "RacingGameAppClass";

ConfigSettingUint32 cfgActionKey("AnyAction", "Defines a Trigger to execute an action", Keyboard::Code::key_W);
ConfigSettingUint32 cfgRenderEngineType("RenderEngine", "Defines which underlying rendering should be used", RenderEngineType::OpenGL);

typedef ProxyAllocator<LinearAllocator, policy::NoSync, policy::NoBoundsChecking, policy::NoTracking, policy::NoTagging> ApplicationAllocator;

const size_type gAppMemorySize = MEGABYTE(4);
ApplicationAllocator gAppAlloc("ApplicationAllocator"); //A linear allocator for all dynamic allocations of the Main application
Main gApplication;

//
/// Simple FPS Count
const size_type tick_samples = 100;
const float32 oneSecond = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::seconds(1)).count();
size_type tickIdx = 0;
float32 tickSum = 0.0;
float32 ticklist[tick_samples] = { 0 };
float32 CalcFPS(float32 newTick) {
	tickSum -= ticklist[tickIdx];
	tickSum += newTick;
	ticklist[tickIdx] = newTick;
	tickIdx = (tickIdx + 1) % tick_samples;

	return (oneSecond * tick_samples) / tickSum;
}
///

int32 main(int32 argc, const ansichar* argv[]) {
	struct Guard {
		Guard(Main* app) : ref(app){}
		~Guard() {
			if (ref) ref->shutdown();
		}
		Main* ref;
	};

	Main* app = Main::Startup(argc, argv);
	Guard _(app); //shutdown on main leave
	if (!app->initialize()) {
		LOG_ERROR(General, "Initialization failed.");
		return 1;
	}
	return app->execute();
}

Main::Main() :
	m_Argc(0),
	m_Argv(nullptr),
	m_AnimationSystem(nullptr),
	m_AudioSystem(nullptr),
	m_PhysicSystem(nullptr),
	m_RenderSystem(nullptr),
	m_ScriptSystem(nullptr),
	m_Running(false)
{}

Main::~Main()
{
	m_Running = false;
	shutdown();
}


void Main::shutdown()
{
	//shutdown
	if (m_InputSystem) m_InputSystem->shutdown();
	if (m_RenderSystem) m_InputSystem->shutdown();
	if (m_WindowSystem) m_WindowSystem->shutdown();

	//and free
	//eng_delete(m_ScriptSystem, gAppAlloc);
	eng_delete(m_RenderSystem, gAppAlloc);
	//eng_delete(m_PhysicSystem, gAppAlloc);
	//eng_delete(m_AudioSystem, gAppAlloc);
	//eng_delete(m_AnimationSystem, gAppAlloc);
	eng_delete(m_InputSystem, gAppAlloc);
	eng_delete(m_WindowSystem, gAppAlloc);

	JobScheduler::Shutdown();

	m_Argc = 0;
	m_Argv = nullptr;
}

Main* Main::Startup(int32 argc, const ansichar * argv[])
{
	gAppAlloc.initialize(gAppMemorySize); //initialize the allocator to manage the gAppMemory
	//write parameter
	gApplication.m_Argc = argc;
	gApplication.m_Argv = argv;
	std::fill(ticklist, ticklist + tick_samples, float32(0)); //initialize with 0'es
	

#	if OS_WINDOWS

	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(WNDCLASS));

	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = 0;
	wc.hCursor = 0;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hInstance = GetModuleHandle(0);
	wc.lpfnWndProc = (WNDPROC)&Win32Window::processMessage;
	wc.lpszMenuName = 0;
	wc.lpszClassName = sAppClassName;
	wc.style = CS_DBLCLKS;

	if (!RegisterClass(&wc)) {
		LOG_ERROR(General, "WindowsApplication register failed: %d", GetLastError());
		return false;
	}
#	endif

	//return application-ptr
	return &gApplication;
}

bool Main::initialize()
{
	//create the subsystems in Application's memory 
	//managed by the ApplicationAllocator policies 
	//m_AnimationSystem = eng_new(AnimationSystem, gAppAlloc);
	//m_AudioSystem = eng_new(AudioSystem, gAppAlloc);
	m_InputSystem = eng_new(InputSystem, gAppAlloc);
	//m_PhysicSystem = eng_new(PhysicSystem, gAppAlloc);
	m_RenderSystem = eng_new(RenderSystem, gAppAlloc);
	//m_ScriptSystem = eng_new(ScriptSystem, gAppAlloc);
	m_WindowSystem = eng_new(WindowSystem, gAppAlloc);

	JobScheduler::Initialize(); //startup the jobsystem

	if (!m_WindowSystem->initialize()) {
		LOG_ERROR(General, "The windowsystem initialization failed.");
		return false;
	}
	if (!m_InputSystem->initialize()) {
		LOG_ERROR(General, "The input system initialization failed.");
		return false;
	}

	//we need a temporary window to initialize opengl
	Window* tmpWin = m_WindowSystem->openWindow();
    if(!tmpWin) {
        LOG_ERROR(General, "Temporalwindowcreation failed.");
        return false;
    }
	RenderContext* rc = tmpWin->createContext(RenderEngineType::Enum((uint32)cfgRenderEngineType)); //create an OpenGL context
	if (!rc) {
		LOG_ERROR(Renderer, "Context creation for temp-Window failed.");
		return false;
	}
	rc->makeCurrent();

	if (!m_RenderSystem->initialize(RenderEngineType::Enum((uint32)cfgRenderEngineType))) {
		LOG_ERROR(Renderer, "The renderer initialization failed.");
		return false;
	}
	m_WindowSystem->destroyWindow(tmpWin);
	m_Running = true;

	return true;
}

int32 Main::execute()
{
	bool b = loop();
	shutdown();
	return b ? 0 : 1;
}

bool Main::loop()
{
	//create a window
	Window * mainWindow = m_WindowSystem->openWindow();
	if (!mainWindow) return false;
	m_RenderSystem->attachWindow(mainWindow);

	//
	/// INPUT EXAMPLE
	int32 inputIndex = m_InputSystem->attachWindow(mainWindow);
	if (inputIndex == -1) return false;

	InputDevice device;
	device.connectDevice(m_InputSystem->getKeyboard());
	device.connectDevice(m_InputSystem->getMouse());
    //device.connectDevice(m_InputSystem->getJoystick());

	Trigger::ID trggrReturn = device.addTrigger(&Key::WentUp<Keyboard::Code::key_RETURN>);
	Trigger::ID trggrLeftClick = device.addTrigger(&MouseButton::WentUp<Mouse::Button::Left>);
    //Trigger::ID trggrXPressed = device.addTrigger(&JoystickButton::WentDown<Joystick::Button::Button1>);
	Trigger::ID trggrConfigKey = device.addTrigger(&ConfigKey::WentDown<&cfgActionKey>);

	Trigger::ID trggrChoord = device.addTrigger(
	&Chord<
		&Map<
			&Key::IsPressed<Keyboard::Code::key_LSHIFT>,
			&Key::IsPressed<Keyboard::Code::key_RSHIFT>
		>,
		&Chord<
			&Key::IsReleased<Keyboard::Code::key_LCONTROL>,
			&Key::IsReleased<Keyboard::Code::key_RCONTROL>
		>,
		&Key::WentDown<Keyboard::Code::key_A>
	>);
	/*
	trggrChoord = device.addTrigger(
		&Map<
		&Key::IsPressed<Keyboard::Code::key_LSHIFT>,
		&Key::IsPressed<Keyboard::Code::key_RSHIFT>
		>
	);
	//*/
	///END INPUT EXAMPLE
	//

#	if DEBUG_BUILD || SHOW_DEBUG_TITLE
	const ansichar* dbg_WindowTitleTemplate = "DEBUG: CurrentFPS (%.3f)";
	ansichar dbg_WindowTitle[128];
	float32 accumDt = 0.0, lastTitleUpdate = 0.0;
#	endif
	typedef std::chrono::high_resolution_clock clock;
	typedef clock::time_point time_point;
	typedef clock::duration time_duration;
	time_point start = clock::now(), current = start, last = start;

	while (m_Running) {
		current = clock::now();
		float32 dt = float32(std::chrono::duration_cast<std::chrono::microseconds>(current - last).count());
		float32 fps = CalcFPS(dt); //get the fps estimation

#	if DEBUG_BUILD || SHOW_DEBUG_TITLE
		accumDt += dt;
		if (accumDt - lastTitleUpdate > oneSecond / 2) {
		std::sprintf(dbg_WindowTitle, dbg_WindowTitleTemplate, fps);
			lastTitleUpdate = accumDt;
			mainWindow->setTitle(dbg_WindowTitle);
		}
#	endif
		if (!m_WindowSystem->tick(dt)) {
			m_Running = false;
		}

		if (!m_InputSystem->tick(dt)) {
			m_Running = false;
		}

		if (!m_RenderSystem->tick(dt)) {
			//Rendering failed -> shutdown
			m_Running = false;
		}

		if (device.isTriggered(trggrReturn)) {
			LOG_INFO(General, "Key RETURN pressed");
		}

		if (device.isTriggered(trggrLeftClick)) {
			LOG_INFO(General, "Mouse CLICKED");
		}

        /*
		if (device.isTriggered(trggrXPressed)) {
			LOG_INFO(General, "X Pressed");
		}
        */

		if (device.isTriggered(trggrChoord)) {
			LOG_INFO(General, "Chord triggered");
		}

		if (device.isTriggered(trggrConfigKey)) {
			LOG_INFO(General, "Configurated key");
			cfgActionKey = Keyboard::Code::key_A; 
		}

		mainWindow->swapBuffers();
		m_Running = m_Running && !mainWindow->isClosed();
		last = current;
	}

	return false;
}

ENDNAMESPACE

#pragma once

BEGINNAMESPACE

class AnimationSystem;
class AudioSystem;
class InputSystem;
class PhysicSystem;
class RenderSystem;
class ScriptSystem;
class WindowSystem;

class Main {
public:
	static Main* Startup(int32 argc, const ansichar** argv);
public:
	Main();
	~Main();
	bool initialize();
	int32 execute();
	void shutdown();
private:
	bool loop();
private:
	//application parameter
	int32 m_Argc;
	const ansichar** m_Argv;

	//Systems
	AnimationSystem* m_AnimationSystem;
	AudioSystem* m_AudioSystem;
	InputSystem* m_InputSystem;
	PhysicSystem* m_PhysicSystem;
	RenderSystem* m_RenderSystem;
	ScriptSystem* m_ScriptSystem;
	WindowSystem* m_WindowSystem;

	//Running?!
	std::atomic_bool m_Running;
};

int32 main(int32 argc, const ansichar** argv);

ENDNAMESPACE
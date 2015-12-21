#pragma once

BEGINNAMESPACE

class AnimationSystem;
class AudioSystem;
class InputSystem;
class ObjectSystem;
class PackageSystem;
class PhysicsSystem;
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

    AnimationSystem* getAnimationSystemPtr(){return m_AnimationSystem;}
    AudioSystem* getAudioSystemPtr(){return m_AudioSystem;}
    InputSystem* getInputSystemPtr(){return m_InputSystem;}
    ObjectSystem* getObjectSystemPtr(){return m_ObjectSystem;}
    PhysicsSystem* getPhysicsSystemPtr(){return m_PhysicsSystem;}
    RenderSystem* getRenderSystemPtr(){return m_RenderSystem;}
    ScriptSystem* getScriptSystemPtr(){return m_ScriptSystem;}
    WindowSystem* getWindowSystemPtr(){return m_WindowSystem;}
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
    ObjectSystem* m_ObjectSystem;
	PackageSystem* m_PackageSystem;
    PhysicsSystem* m_PhysicsSystem;
	RenderSystem* m_RenderSystem;
	ScriptSystem* m_ScriptSystem;
	WindowSystem* m_WindowSystem;

	//Running?!
	std::atomic_bool m_Running;
};

int32 main(int32 argc, const ansichar** argv);

ENDNAMESPACE

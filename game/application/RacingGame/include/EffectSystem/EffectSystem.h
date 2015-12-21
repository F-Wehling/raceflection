#pragma once

BEGINNAMESPACE

class EffectSystem {
public:
	EffectSystem();

	bool initialize();

private:
	static void nvFXErrorCallback(const ansichar* error);
	static void nvFXMessageCallback(const ansichar* message);
	static void nvFXIncludeCallback(const ansichar* includeName, FILE*& fp, const ansichar* &buf);
};

ENDNAMESPACE

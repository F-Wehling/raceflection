#pragma once

#include <Utilities/Handle.h>

BEGINNAMESPACE

class PackageSpec;

typedef Handle<15, 1> EffectHandle;

extern EffectHandle InvalidEffectHandle;

class EffectSystem {
public:
	EffectSystem();

	bool initialize(int32 api);
	void shutdown();

	bool createEffect(const ansichar* effectSource);
	
	void createEffectLibraryFromPackageSpec(const PackageSpec* spec);
	
private:
	static void nvFXErrorCallback(const ansichar* error);
	static void nvFXMessageCallback(const ansichar* message);
	static void nvFXIncludeCallback(const ansichar* includeName, FILE*& fp, const ansichar* &buf);
};

ENDNAMESPACE

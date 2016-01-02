#pragma once

#include "MemorySystem.h"
#include "PackageSpec.h"

#if DEBUG_BUILD
#include "Container/Array.h"
#endif

BEGINNAMESPACE

class Main;
class Filesystem;
class PackageSystem;
class PackageSpec;
struct Job;

class PackageSystem {
    struct TrackedPackage {
		ansichar packageFile[255];
        uint32 time_stamp;
		Byte* packageMemory;
		PackageSpec* packageInstance;
		int32 _state;
    };

	typedef struct {
		ansichar _importFile[256];
		Job* _importJob;
		Byte* _importMemory;
		uint32 _importSize;
		PackageSpec* _pkg;
		PackageSystem* _refSystem;
		TrackedPackage* _replacePkg;
		int32 _state;
	} ImportHandle;
public:
	PackageSystem(Main* mainRef);
	~PackageSystem();
	void importPackage(const ansichar* importFile);

	bool initialize();
	void shutdown();

    void tick(float32 dt);

private:
	void startImportPackage(const ansichar* importFile, TrackedPackage* replace);
	bool isFinished(ImportHandle& hdl);
	bool interprete(ImportHandle& hdl);

	static void Import(Job* job, const void* data);

private:
	Main* m_MainRef;

	typedef PackageSpec::PackageAllocator PackageAllocator;
	typedef ProxyAllocator<PoolAllocator, policy::NoSync, policy::NoBoundsChecking, policy::NoTracking, policy::NoTagging> ImportHandleAllocator;

	PackageAllocator m_PkgAllocator;
	ImportHandleAllocator m_ImportHdlAllocator;
	Filesystem* m_Filesystem;

	typedef DynArray<ImportHandle> ImportHandles_t;
	ImportHandles_t m_ImportHandles;
	typedef DynArray<TrackedPackage> TrackedPackages_t;
	TrackedPackages_t m_TrackedPackages;
};

ENDNAMESPACE

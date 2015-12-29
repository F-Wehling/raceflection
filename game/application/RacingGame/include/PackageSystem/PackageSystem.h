#pragma once

#include "MemorySystem.h"

#if DEBUG_BUILD
#include "Container/Array.h"
#endif

BEGINNAMESPACE

class PackageSystem;
class PackageSpec;
struct Job;

typedef struct {
	ansichar _importFile[256];
	Job* _importJob;
	Byte* _importMemory;
	uint32 _importSize;
	PackageSpec* _pkg;
	PackageSystem* _refSystem;
} ImportHandle;

class PackageSystem {
public:
	PackageSystem();
	ImportHandle* startImportPackage(const ansichar* importFile);

    void tick(float32 dt);

	bool isFinished(ImportHandle* hdl);
	PackageSpec* interprete(ImportHandle* hdl);

private:
		
	static void Import(Job* job, const void* data);

private:
	typedef ProxyAllocator<PoolAllocator, policy::NoSync, policy::NoBoundsChecking, policy::NoTracking, policy::NoTagging> ImportHandleAllocator;
	typedef ProxyAllocator<LinearAllocator, policy::NoSync, policy::NoBoundsChecking, policy::NoTracking, policy::NoTagging> PackageAllocator;
	PackageAllocator m_PkgAllocator;
	ImportHandleAllocator m_ImportHdlAllocator;

#   if DEBUG_BUILD
    enum PackageStatus{
        UP_TO_DATE,
        RELOADING,
        FINISHED_LOADING
    };
    struct TrackPackageTime {
        uint32 status;
        uint32 time_stamp;
        PackageSpec* spec;
        ImportHandle handle;
    };
    typedef DynArray<TrackPackageTime> TrackedPackageTimer;
#   endif
};

ENDNAMESPACE

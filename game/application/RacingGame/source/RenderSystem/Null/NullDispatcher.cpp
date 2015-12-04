#include "RenderSystem/Null/NullDispatcher.h"

#include "MemorySystem.h"
#include "Allocator/PoolAllocator.h"

BEGINNAMESPACE

typedef NullDispatcher dispatcher;

void NullDispatcher::IgnoreCall(const void* data) {}

void NullDispatcher::Initialize() {
	//use standard initialization
}

ENDNAMESPACE
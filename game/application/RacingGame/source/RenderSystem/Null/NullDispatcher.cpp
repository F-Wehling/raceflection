#include "RenderSystem/Null/NullDispatcher.h"

#include "MemorySystem.h"
#include "Allocator/PoolAllocator.h"

BEGINNAMESPACE

#if USE_NULLRENDERER

IMPL_DISPATCHER(Draw) }
IMPL_DISPATCHER(DrawIndexed) }
IMPL_DISPATCHER(CopyConstantBufferData) }

#endif

ENDNAMESPACE
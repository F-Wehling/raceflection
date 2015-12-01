#include <RenderSystem/RenderBucket.h>

BEGINNAMESPACE

extern thread_local CommandAllocator gtl_CommandAllocator;
extern thread_local size_type gtl_RenderBucketOffset = 0;
extern thread_local size_type gtl_RenderBucketRemaining = 0;

ENDNAMESPACE
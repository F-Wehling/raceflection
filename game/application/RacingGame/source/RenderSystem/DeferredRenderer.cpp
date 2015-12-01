#include <RenderSystem/DeferredRenderer.h>
#include <Utilities/FloatCompressor.h>
#include <CompileTime/Templates.h>

#include <Utilities/Number.h>
#include <Logging/Logging.h>

#include <Multithreading/JobScheduler.h>
#include <Multithreading/ParallelFor.h>

#include <vector>

BEGINNAMESPACE

DeferredRenderer::GBufferKey DeferredRenderer::GenerateGBufferKey(const BoundingBox& aabb, MaterialHandle material, uint8 pass) {
	static const uint8 MAX_PASS = MaxUnsignedWithNBits<uint32, _GenGBufferKey::PassCount>::value;
	ASSERT(pass <= MAX_PASS, "The G-Buffer is only capable of %d render passes", MAX_PASS);

	union {	_GenGBufferKey g; GBufferKey k; };

	g.preSort = 0;
	g.material = material.index;
	
	float32 computeDepthHere = 0.0f;
	g.depth = Float16Compressor::compress(computeDepthHere);

	CutToNBits<GBufferKey, _GenGBufferKey::PassCount> cutter;
	cutter.set = pass;
	g.pass = (cutter.get);

	g.postSort = 0;
	return k;
}

void DeferredRenderer::render(float32 dt, Scene * scene)
{
}

ENDNAMESPACE
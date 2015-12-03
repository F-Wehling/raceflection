#include "RenderSystem/OpenGL/OpenGLBackend.h"
#include "RenderSystem/OpenGL/OpenGLDriver.h"
#include "Configuration/Configuration.h"

#include "Utilities/Assert.h"
#include "Utilities/SourceInfo.h"

#include "MemorySystem.h"

#include "Logging.h"

#include <ACGL/ACGL.hh>
#include <ACGL/OpenGL/Objects.hh>

BEGINNAMESPACE

ConfigSettingBool sCfgDebugContext("DebugContext", "Defines wheter a debug context should be created or not", DEBUG_BUILD == 1);

////////////////////////////////
////// RESOURCE POOOL //////////
////////////////////////////////

namespace ogl = ACGL::OpenGL;

typedef ogl::ArrayBuffer VertexBuffer;
typedef ogl::ElementArrayBuffer IndexBuffer;
typedef ogl::VertexArrayObject VertexArrayObject;
typedef ogl::ShaderProgram ShaderProgram;
typedef ogl::FrameBufferObject RenderTarget;
typedef ogl::UniformBuffer ConstantBuffer;

//Number of objects

typedef ProxyAllocator < PoolAllocator, policy::NoSync, policy::NoBoundsChecking, policy::NoTracking, policy::NoTagging> PoolAlloc;



struct ResourcePool_t{
	struct Manager_t{
		typedef ProxyAllocator < PoolAllocator, policy::NoSync, policy::NoBoundsChecking, policy::NoTracking, policy::NoTagging> PoolAlloc;
		PoolAlloc VertexBuffer;
		PoolAlloc IndexBuffer;
		PoolAlloc VertexArrayObject;
		PoolAlloc ShaderProgram;
		PoolAlloc RenderTarget;
		PoolAlloc ConstantBuffer;

		Manager_t() :
			VertexBuffer("VertexBufferManager"),
			IndexBuffer("IndexBufferManager"),
			VertexArrayObject("VertexArrayObjectManager"),
			ShaderProgram("ShaderProgramManager"),
			RenderTarget("RenderTargetManager"),
			ConstantBuffer("ConstantBufferManager"){}
	} Manager;
	VertexBuffer* VertexBuffer;
	IndexBuffer* IndexBuffer;
	VertexArrayObject* VertexArrayObject;
	ShaderProgram* ShaderProgram;
	RenderTarget* RenderTarget;
	ConstantBuffer* ConstantBuffer;

} ResourcePool;


////////////////////////////////

# define INIT_MANAGER(Object) ResourcePool.Manager.Object.initialize(sizeof(Object) * (1 << Object ## Handle::IndexBitCount), sizeof(Object), alignof(Object)); ResourcePool.Object = (Object*)ResourcePool.Manager.Object.getStart()

typedef Handle<VertexBufferHandle::IndexBitCount, VertexBufferHandle::GenerationBitCount> VertexArrayObjectHandle;

bool GLBackend::StartupBackend() {
	if (!ACGL::init(sCfgDebugContext)) {
		LOG_ERROR(Renderer, "ACGL initialization failed.");
		return false;
	}

	INIT_MANAGER(VertexBuffer);
	INIT_MANAGER(IndexBuffer);
	INIT_MANAGER(VertexArrayObject);
	INIT_MANAGER(ShaderProgram);
	INIT_MANAGER(RenderTarget);
	INIT_MANAGER(ConstantBuffer);
		
	return true;
}

bool GLBackend::ShutdownBackend() {
	return true;
}

bool GLBackend::InitializeBackend()
{
	return true;
}


//
/// Every VB also creates a VAO
void CreateVertexArrayObject() {
	eng_new(VertexArrayObject, ResourcePool.Manager.VertexArrayObject);
}

VertexBufferHandle GLBackend::CreateStaticVertexBuffer(size_type bufferSize, Byte* pInitialData) {
	VertexBuffer* vb = eng_new(VertexBuffer, ResourcePool.Manager.VertexBuffer);
	vb->setData(bufferSize, pInitialData, GL_STATIC_DRAW);
	
	VertexBufferHandle h = { VertexBufferHandle::_Handle_type(std::distance(ResourcePool.VertexBuffer, vb)),0 }; //generate a handle for this object

	CreateVertexArrayObject(); //create a VAO
	VertexArrayObject* vao = ResourcePool.VertexArrayObject + h.index;	
	vao->attachAllAttributes(ogl::ConstSharedArrayBuffer(vb, [](...) {}));
	return h;
}

VertexBufferHandle GLBackend::CreateDynamicVertexBuffer(size_type bufferSize, Byte * pInitialData) {
	VertexBuffer* vb = eng_new(VertexBuffer, ResourcePool.Manager.VertexBuffer);

	vb->setData(bufferSize, pInitialData, GL_DYNAMIC_DRAW);

	VertexBufferHandle h = { VertexBufferHandle::_Handle_type(std::distance(ResourcePool.VertexBuffer, vb)),0 }; //generate a handle for this object

	CreateVertexArrayObject(); //create a VAO
	VertexArrayObject* vao = ResourcePool.VertexArrayObject + h.index;
	vao->attachAllAttributes(ogl::ConstSharedArrayBuffer(vb, [](...) {}));
	return h;
}

//
/// Draw
void GLBackend::Draw(uint32 vertexCount, uint32 startVertex, VertexBufferHandle vbHdl, VertexLayoutHandle vbLayout) {
	VertexArrayObject& vao = ResourcePool.VertexArrayObject[vbHdl.index]; //In opengl the VB is automaticall linked into the VAO with the same index
	ASSERT(startVertex == 0, "startVertex isn't 0. OpenGL doesn't support this!");
	
}

void GLBackend::DrawIndexed(uint32 indexCount, uint32 startIndex, uint32 baseVertex, VertexBufferHandle vbHdl, IndexBufferHandle ibHdl, VertexLayoutHandle vbLayout) {
	VertexArrayObject& vao = ResourcePool.VertexArrayObject[vbHdl.index];
	vao.drawRangeElements(startIndex, indexCount);
}

//
/// ConstantBuffer
ConstantBufferHandle GLBackend::CreateConstantBuffer() {
	ConstantBuffer* cb = eng_new(ConstantBuffer, ResourcePool.Manager.ConstantBuffer);
	
	ConstantBufferHandle cbHdl = { ConstantBufferHandle::_Handle_type(std::distance(ResourcePool.ConstantBuffer, cb)), 0 };
	return cbHdl;
}

void GLBackend::CopyConstantBufferData(ConstantBufferHandle cbHdl, const void * data, uint32 size){
	ConstantBuffer& buffer = ResourcePool.ConstantBuffer[cbHdl.index];
	buffer.setData(size, data);
}


//
/// Render Targets
RenderTargetHandle GLBackend::CreateRenderTarget()
{
	RenderTarget* rt = eng_new(RenderTarget, ResourcePool.Manager.RenderTarget);

	RenderTargetHandle rtHdl = { RenderTargetHandle::_Handle_type(std::distance(ResourcePool.RenderTarget, rt)), 0 };
	return rtHdl;
}

void GLBackend::ClearRenderTarget(RenderTargetHandle rbHdl) {
	RenderTarget& renderTarget = ResourcePool.RenderTarget[rbHdl.index]; //Access the RenderTarget via hdl
	renderTarget.clearBuffers();
}

//
/// Screen
void GLBackend::ClearScreen() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void GLBackend::ScreenSetClearColor(float32 r, float32 g, float32 b, float32 a) {
	glClearColor(r, g, b, a);
}

ENDNAMESPACE
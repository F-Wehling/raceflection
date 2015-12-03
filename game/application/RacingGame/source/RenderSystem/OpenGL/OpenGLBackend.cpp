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
        PoolAlloc VertexBufferMgr;
        PoolAlloc IndexBufferMgr;
        PoolAlloc VertexArrayObjectMgr;
        PoolAlloc ShaderProgramMgr;
        PoolAlloc RenderTargetMgr;
        PoolAlloc ConstantBufferMgr;

		Manager_t() :
            VertexBufferMgr("VertexBufferManager"),
            IndexBufferMgr("IndexBufferManager"),
            VertexArrayObjectMgr("VertexArrayObjectManager"),
            ShaderProgramMgr("ShaderProgramManager"),
            RenderTargetMgr("RenderTargetManager"),
            ConstantBufferMgr("ConstantBufferManager"){}
	} Manager;
    VertexBuffer* m_VertexBuffer;
    IndexBuffer* m_IndexBuffer;
    VertexArrayObject* m_VertexArrayObject;
    ShaderProgram* m_ShaderProgram;
    RenderTarget* m_RenderTarget;
    ConstantBuffer* m_ConstantBuffer;

} ResourcePool;


////////////////////////////////

# define INIT_MANAGER(Object) ResourcePool.Manager.Object ## Mgr.initialize(sizeof(Object) * (1 << Object ## Handle::IndexBitCount), sizeof(Object), alignof(Object)); ResourcePool.m_ ## Object = (Object*)ResourcePool.Manager.Object ## Mgr.getStart()

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
    eng_new(VertexArrayObject, ResourcePool.Manager.VertexArrayObjectMgr);
}

VertexBufferHandle GLBackend::CreateStaticVertexBuffer(size_type bufferSize, Byte* pInitialData) {
    VertexBuffer* vb = eng_new(VertexBuffer, ResourcePool.Manager.VertexBufferMgr);
	vb->setData(bufferSize, pInitialData, GL_STATIC_DRAW);
	
    VertexBufferHandle h = { VertexBufferHandle::_Handle_type(std::distance(ResourcePool.m_VertexBuffer, vb)),0 }; //generate a handle for this object

	CreateVertexArrayObject(); //create a VAO
    VertexArrayObject* vao = ResourcePool.m_VertexArrayObject + h.index;
	vao->attachAllAttributes(ogl::ConstSharedArrayBuffer(vb, [](...) {}));
	return h;
}

VertexBufferHandle GLBackend::CreateDynamicVertexBuffer(size_type bufferSize, Byte * pInitialData) {
    VertexBuffer* vb = eng_new(VertexBuffer, ResourcePool.Manager.VertexBufferMgr);

	vb->setData(bufferSize, pInitialData, GL_DYNAMIC_DRAW);

    VertexBufferHandle h = { VertexBufferHandle::_Handle_type(std::distance(ResourcePool.m_VertexBuffer, vb)),0 }; //generate a handle for this object

	CreateVertexArrayObject(); //create a VAO
    VertexArrayObject* vao = ResourcePool.m_VertexArrayObject + h.index;
	vao->attachAllAttributes(ogl::ConstSharedArrayBuffer(vb, [](...) {}));
	return h;
}

//
/// Draw
void GLBackend::Draw(uint32 vertexCount, uint32 startVertex, VertexBufferHandle vbHdl, VertexLayoutHandle vbLayout) {
    VertexArrayObject& vao = ResourcePool.m_VertexArrayObject[vbHdl.index]; //In opengl the VB is automaticall linked into the VAO with the same index
	ASSERT(startVertex == 0, "startVertex isn't 0. OpenGL doesn't support this!");
	
}

void GLBackend::DrawIndexed(uint32 indexCount, uint32 startIndex, uint32 baseVertex, VertexBufferHandle vbHdl, IndexBufferHandle ibHdl, VertexLayoutHandle vbLayout) {
    VertexArrayObject& vao = ResourcePool.m_VertexArrayObject[vbHdl.index];
	vao.drawRangeElements(startIndex, indexCount);
}

//
/// ConstantBuffer
ConstantBufferHandle GLBackend::CreateConstantBuffer() {
    ConstantBuffer* cb = eng_new(ConstantBuffer, ResourcePool.Manager.ConstantBufferMgr);
	
    ConstantBufferHandle cbHdl = { ConstantBufferHandle::_Handle_type(std::distance(ResourcePool.m_ConstantBuffer, cb)), 0 };
	return cbHdl;
}

void GLBackend::CopyConstantBufferData(ConstantBufferHandle cbHdl, const void * data, uint32 size){
    ConstantBuffer& buffer = ResourcePool.m_ConstantBuffer[cbHdl.index];
	buffer.setData(size, data);
}


//
/// Render Targets
RenderTargetHandle GLBackend::CreateRenderTarget()
{
    RenderTarget* rt = eng_new(RenderTarget, ResourcePool.Manager.RenderTargetMgr);

    RenderTargetHandle rtHdl = { RenderTargetHandle::_Handle_type(std::distance(ResourcePool.m_RenderTarget, rt)), 0 };
	return rtHdl;
}

void GLBackend::ClearRenderTarget(RenderTargetHandle rbHdl) {
    RenderTarget& renderTarget = ResourcePool.m_RenderTarget[rbHdl.index]; //Access the RenderTarget via hdl
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

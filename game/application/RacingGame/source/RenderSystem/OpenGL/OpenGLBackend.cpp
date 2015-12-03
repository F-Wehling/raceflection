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

//Vertex Buffer
static const size_type MaxVertexBuffer = 1 << VertexBufferHandle::IndexBitCount;
VertexBuffer* gVertexBuffer;
size_type gCurrentVBCount = 0;
Byte gVertexBufferStorage[sizeof(VertexBuffer) * MaxVertexBuffer];
PoolAlloc gVertexBufferManager("VertexBufferManager");

//Index Buffer 
static const size_type MaxIndexBuffer = 1 << IndexBufferHandle::IndexBitCount;
IndexBuffer* gIndexBuffer;
size_type gCurrentIBCount = 0;
Byte gIndexBufferStorage[sizeof(IndexBuffer) * MaxIndexBuffer];
PoolAlloc gIndexBufferManager("IndexBufferManager");

//VertexArrayObjects
static const size_type MaxVertexArrays = 1 << VertexBufferHandle::IndexBitCount;
VertexArrayObject* gVertexArrayObjects;
size_type gCurrentVAOCount = 0;
Byte gVertexArrayObjectStorage[sizeof(VertexArrayObject) * MaxVertexArrays];
PoolAlloc gVertexArrayManager("VertexArrayObjectManager");

//Shader program storage
static const size_type MaxShaderPrograms = 1 << ShaderProgramHandle::IndexBitCount;
ShaderProgram* gShaderProgram;
size_type gCurrentSPCount = 0;
Byte gShaderProgramStorage[sizeof(ShaderProgram) * MaxShaderPrograms];
PoolAlloc gShaderProgramManager("ShaderProgramManager");

//Render Target
static const size_type MaxRenderTargets = 1 << RenderTargetHandle::IndexBitCount;
RenderTarget* gRenderTarget;
size_type gCurrentRTCount = 0;
Byte gRenderTargetStorage[sizeof(RenderTarget) * MaxRenderTargets];
PoolAlloc gRenderTargetManager("RenderTargetManager");

//Constant Buffer
static const size_type MaxConstantBuffer = 1 << ConstantBufferHandle::IndexBitCount;
ConstantBuffer* gConstantBuffer;
size_type gCurrentCBCount = 0;
Byte gConstantBufferStorage[sizeof(ConstantBuffer) * MaxConstantBuffer];
PoolAlloc gConstantBufferManager("ConstantBufferManager");


////////////////////////////////


bool GLBackend::StartupBackend() {
	if (!ACGL::init(sCfgDebugContext)) {
		LOG_ERROR(Renderer, "ACGL initialization failed.");
		return false;
	}

	gVertexBuffer = (VertexBuffer*)gVertexBufferStorage;
	gIndexBuffer = (IndexBuffer*)gIndexBufferStorage;
	gVertexArrayObjects = (VertexArrayObject*)gVertexArrayObjectStorage;
	gShaderProgram = (ShaderProgram*)gShaderProgramStorage;
	gRenderTarget = (RenderTarget*)gRenderTargetStorage;
	gConstantBuffer = (ConstantBuffer*)gConstantBufferStorage;

	gVertexBufferManager.initialize(sizeof(VertexBuffer), alignof(VertexBuffer), gVertexBuffer, sizeof(VertexBuffer) * MaxVertexBuffer);
	gIndexBufferManager.initialize(sizeof(IndexBuffer), alignof(IndexBuffer), gIndexBuffer, sizeof(IndexBuffer) * MaxIndexBuffer);
	gVertexArrayManager.initialize(sizeof(VertexArrayObject), alignof(VertexArrayObject), gVertexArrayObjects, sizeof(VertexArrayObject) * MaxVertexArrays);
	gShaderProgramManager.initialize(sizeof(ShaderProgram), alignof(ShaderProgram), gShaderProgram, sizeof(ShaderProgram) * MaxShaderPrograms);
	gRenderTargetManager.initialize(sizeof(RenderTarget), alignof(RenderTarget), gRenderTarget, sizeof(RenderTarget) * MaxRenderTargets);
	gConstantBufferManager.initialize(sizeof(ConstantBuffer), alignof(ConstantBuffer), gConstantBuffer, sizeof(ConstantBuffer) * MaxConstantBuffer);
	
	return true;
}

bool GLBackend::ShutdownBackend() {
	return true;
}

bool GLBackend::InitializeBackend()
{
	glClearColor(0.4, 0.4, 0.4, 1.0);
	return true;
}


//
/// Every VB also creates a VAO
void CreateVertexArrayObject() {
	eng_new(VertexArrayObject, gVertexArrayManager);
}

VertexBufferHandle GLBackend::CreateStaticVertexBuffer(size_type bufferSize, Byte* pInitialData) {
	VertexBuffer* vb = eng_new(VertexBuffer, gVertexBufferManager);
	++gCurrentVBCount;
	vb->setData(bufferSize, pInitialData, GL_STATIC_DRAW);
	
	VertexBufferHandle h = { std::distance(gVertexBuffer, vb),0 }; //generate a handle for this object

	CreateVertexArrayObject(); //create a VAO
	VertexArrayObject* vao = gVertexArrayObjects + h.index;	
	vao->attachAllAttributes(ogl::ConstSharedArrayBuffer(vb, [](...) {}));
	return h;
}

VertexBufferHandle GLBackend::CreateDynamicVertexBuffer(size_type bufferSize, Byte * pInitialData) {
	VertexBuffer* vb = eng_new(VertexBuffer, gVertexBufferManager);
	++gCurrentVBCount;
	vb->setData(bufferSize, pInitialData, GL_DYNAMIC_DRAW);

	VertexBufferHandle h = { std::distance(gVertexBuffer, vb),0 }; //generate a handle for this object

	CreateVertexArrayObject(); //create a VAO
	VertexArrayObject* vao = gVertexArrayObjects + h.index;	
	vao->attachAllAttributes(ogl::ConstSharedArrayBuffer(vb, [](...) {}));
	return h;
}

//
/// Draw
void GLBackend::Draw(uint32 vertexCount, uint32 startVertex, VertexBufferHandle vbHdl, VertexLayoutHandle vbLayout) {
	VertexArrayObject& vao = gVertexArrayObjects[vbHdl.index]; //In opengl the VB is automaticall linked into the VAO with the same index
	ASSERT(startVertex == 0, "startVertex isn't 0. OpenGL doesn't support this!");
	
}

void GLBackend::DrawIndexed(uint32 indexCount, uint32 startIndex, uint32 baseVertex, VertexBufferHandle vbHdl, IndexBufferHandle ibHdl, VertexLayoutHandle vbLayout) {
	VertexArrayObject& vao = gVertexArrayObjects[vbHdl.index];
	vao.drawRangeElements(startIndex, indexCount);
}

//
/// ConstantBuffer
ConstantBufferHandle GLBackend::CreateConstantBuffer() {
	ConstantBuffer* cb = eng_new(ConstantBuffer, gConstantBufferManager);
	++gCurrentCBCount;

	ConstantBufferHandle cbHdl = { std::distance(gConstantBuffer, cb), 0 };
	return cbHdl;
}

void GLBackend::CopyConstantBufferData(ConstantBufferHandle cbHdl, const void * data, uint32 size){
	ConstantBuffer& buffer = gConstantBuffer[cbHdl.index];
	buffer.setData(size, data);
}


//
/// Render Targets
RenderTargetHandle GLBackend::CreateRenderTarget()
{
	return RenderTargetHandle();
}

void GLBackend::ClearRenderTarget(RenderTargetHandle rbHdl) {
	RenderTarget& renderTarget = gRenderTarget[rbHdl.index]; //Access the RenderTarget via hdl
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
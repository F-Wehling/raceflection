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
typedef ogl::FrameBufferObject FrameBufferObject;

//Number of objects
static const size_type MaxVertexBuffer = 1 << VertexBufferHandle::IndexBitCount;
static const size_type MaxIndexBuffer = 1 << IndexBufferHandle::IndexBitCount;
static const size_type MaxVertexArrays = 1 << VertexBufferHandle::IndexBitCount;
static const size_type MaxShaderPrograms = 1 << ShaderProgramHandle::IndexBitCount;

typedef ProxyAllocator < PoolAllocator, policy::NoSync, policy::NoBoundsChecking, policy::NoTracking, policy::NoTagging> PoolAlloc;

//Vertex Buffer
VertexBuffer* gVertexBuffer;
Byte gVertexBufferStorage[sizeof(VertexBuffer) * MaxVertexBuffer];
PoolAlloc gVertexBufferManager("VertexBufferManager");

//Index Buffer 
IndexBuffer* gIndexBuffer;
Byte gIndexBufferStorage[sizeof(IndexBuffer) * MaxIndexBuffer];
PoolAlloc gIndexBufferManager("IndexBufferManager");

//VertexArrayObjects
VertexArrayObject* gVertexArrayObjects;
Byte gVertexArrayObjectStorage[sizeof(VertexArrayObject) * MaxVertexArrays];
PoolAlloc gVertexArrayManager("VertexArrayObjectManager");

//Shader program storage
ShaderProgram* gShaderProgram;
Byte gShaderProgramStorage[sizeof(ShaderProgram) * MaxShaderPrograms];
PoolAlloc gShaderProgramManager("ShaderProgramManager");


////////////////////////////////


bool GLBackend::StartupBackend() {
	return true;
}

bool GLBackend::ShutdownBackend() {
	return true;
}

bool GLBackend::InitializeBackend()
{
	if (!ACGL::init(sCfgDebugContext)) {
		LOG_ERROR(Renderer, "ACGL initialization failed.");
	}

	gVertexBuffer = (VertexBuffer*)gVertexBufferStorage;
	gIndexBuffer = (IndexBuffer*)gIndexBufferStorage;
	gVertexArrayObjects = (VertexArrayObject*)gVertexArrayObjectStorage;
	gShaderProgram = (ShaderProgram*)gShaderProgramStorage;
	gVertexBufferManager.initialize(sizeof(VertexBuffer), alignof(VertexBuffer), gVertexBuffer, sizeof(VertexBuffer) * MaxVertexBuffer);
	gIndexBufferManager.initialize(sizeof(IndexBuffer), alignof(IndexBuffer), gIndexBuffer, sizeof(IndexBuffer) * MaxIndexBuffer);
	gVertexArrayManager.initialize(sizeof(VertexArrayObject), alignof(VertexArrayObject), gVertexArrayObjects, sizeof(VertexArrayObject) * MaxVertexArrays);
	gShaderProgramManager.initialize(sizeof(ShaderProgram), alignof(ShaderProgram), gShaderProgram, sizeof(ShaderProgram) * MaxShaderPrograms);

	return true;
}

bool GLBackend::AppQuery()
{
	return true;
}

VertexBufferHandle GLBackend::CreateStaticVertexBuffer(size_type bufferSize, Byte* pInitialData) {
	VertexBuffer* vb = eng_new(VertexBuffer, gVertexBufferManager);

	vb->setData(bufferSize, pInitialData, GL_STATIC_DRAW);
	
	VertexBufferHandle h = { std::distance(gVertexBuffer, vb),0 }; //generate a handle for this object
	return h;
}

VertexBufferHandle GLBackend::CreateDynamicVertexBuffer(size_type bufferSize, Byte * pInitialData) {
	VertexBuffer* vb = eng_new(VertexBuffer, gVertexBufferManager);

	vb->setData(bufferSize, pInitialData, GL_DYNAMIC_DRAW);

	VertexBufferHandle h = { std::distance(gVertexBuffer, vb),0 }; //generate a handle for this object
	return h;
}

//
/// Draw
void GLBackend::Draw(uint32 vertexCount, uint32 startVertex, VertexBufferHandle vbHdl, VertexLayoutHandle vbLayout) {

}

void GLBackend::DrawIndexed(uint32 indexCount, uint32 startIndex, uint32 baseVertex, VertexBufferHandle vbHdl, IndexBufferHandle ibHdl, VertexLayoutHandle vbLayout) {

}

//
/// ConstantBuffer
void GLBackend::CopyConstantBufferData(ConstantBufferHandle cbHdl, const void * data, uint32 size){

}


ENDNAMESPACE
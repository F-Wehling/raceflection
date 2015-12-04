#include "RenderSystem/OpenGL/OpenGLBackend.h"
#include "RenderSystem/OpenGL/OpenGLDispatcher.h"
#include "RenderSystem/OpenGL/OpenGLDriver.h"
#include "RenderSystem/OpenGL/OpenGLConversion.h"

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

typedef ogl::RenderBuffer RenderBuffer;
typedef ogl::TextureBase Texture;
typedef ogl::Texture1D Texture1D;
typedef ogl::Texture2D Texture2D;
typedef ogl::Texture3D Texture3D;
typedef ogl::TextureRectangle TextureRectangle;
typedef ogl::TextureCubeMap TextureCube;


//Number of objects
struct ResourcePool_t{
	struct Manager_t{
		typedef ProxyAllocator < PoolAllocator, policy::NoSync, policy::NoBoundsChecking, policy::NoTracking, policy::NoTagging> PoolAlloc;
        PoolAlloc VertexBufferMgr;
        PoolAlloc IndexBufferMgr;
        PoolAlloc VertexArrayObjectMgr;
        PoolAlloc ShaderProgramMgr;
        PoolAlloc RenderTargetMgr;
        PoolAlloc ConstantBufferMgr;
		PoolAlloc RenderBufferMgr;
		PoolAlloc Texture1DMgr;
		PoolAlloc Texture2DMgr;
		PoolAlloc Texture3DMgr;
		PoolAlloc TextureCubeMgr;

		Manager_t() :
            VertexBufferMgr("VertexBufferManager"),
            IndexBufferMgr("IndexBufferManager"),
            VertexArrayObjectMgr("VertexArrayObjectManager"),
            ShaderProgramMgr("ShaderProgramManager"),
            RenderTargetMgr("RenderTargetManager"),
            ConstantBufferMgr("ConstantBufferManager"),
			RenderBufferMgr("RenderBufferManager"),
			Texture1DMgr("Texture1DManager"),
			Texture2DMgr("Texture2DManager"),
			Texture3DMgr("Texture3DManager"),
			TextureCubeMgr("TextureCubeManager")
		{}
	} Manager;
    VertexBuffer* m_VertexBuffer;
    IndexBuffer* m_IndexBuffer;
    VertexArrayObject* m_VertexArrayObject;
    ShaderProgram* m_ShaderProgram;
    RenderTarget* m_RenderTarget;
    ConstantBuffer* m_ConstantBuffer;
	RenderBuffer* m_RenderBuffer;
	Texture1D* m_Texture1D;
	Texture2D* m_Texture2D;
	Texture3D* m_Texture3D;
	TextureCube* m_TextureCube;
} ResourcePool;

// To ignore the ACGL Naming of every type we define several names here and use them 
String _acglNames[] = {
	"a", "aa", "aaa", "aaaa", "aaaaa", "aaaaaa", "aaaaaaa", "aaaaaaaaa", "aaaaaaaaaa",
	"b", "bb", "bbb", "bbbb", "bbbbb", "bbbbbb", "bbbbbbb", "bbbbbbbbb", "bbbbbbbbbb",
	"c", "cc", "ccc", "cccc", "ccccc", "cccccc", "ccccccc", "ccccccccc", "cccccccccc",
	"d", "dd", "ddd", "dddd", "ddddd", "dddddd", "ddddddd", "ddddddddd", "dddddddddd",
	"e", "ee", "eee", "eeee", "eeeee", "eeeeee", "eeeeeee", "eeeeeeeee", "eeeeeeeeee",
	"f", "ff", "fff", "ffff", "fffff", "ffffff", "fffffff", "fffffffff", "ffffffffff",
	"g", "gg", "ggg", "gggg", "ggggg", "gggggg", "ggggggg", "ggggggggg", "gggggggggg",
	"h", "hh", "hhh", "hhhh", "hhhhh", "hhhhhh", "hhhhhhh", "hhhhhhhhh", "hhhhhhhhhh",
	"i", "ii", "iii", "iiii", "iiiii", "iiiiii", "iiiiiii", "iiiiiiiii", "iiiiiiiiii",
	"j", "jj", "jjj", "jjjj", "jjjjj", "jjjjjj", "jjjjjjj", "jjjjjjjjj", "jjjjjjjjjj",
	"k", "kk", "kkk", "kkkk", "kkkkk", "kkkkkk", "kkkkkkk", "kkkkkkkkk", "kkkkkkkkkk",
	"l", "ll", "lll", "llll", "lllll", "llllll", "lllllll", "lllllllll", "llllllllll",
	"m", "mm", "mmm", "mmmm", "mmmmm", "mmmmmm", "mmmmmmm", "mmmmmmmmm", "mmmmmmmmmm",
	"n", "nn", "nnn", "nnnn", "nnnnn", "nnnnnn", "nnnnnnn", "nnnnnnnnn", "nnnnnnnnnn",
	"o", "oo", "ooo", "oooo", "ooooo", "oooooo", "ooooooo", "ooooooooo", "oooooooooo",
	"p", "pp", "ppp", "pppp", "ppppp", "pppppp", "ppppppp", "ppppppppp", "pppppppppp",
	"q", "qq", "qqq", "qqqq", "qqqqq", "qqqqqq", "qqqqqqq", "qqqqqqqqq", "qqqqqqqqqq",
	"r", "rr", "rrr", "rrrr", "rrrrr", "rrrrrr", "rrrrrrr", "rrrrrrrrr", "rrrrrrrrrr",
	"s", "ss", "sss", "ssss", "sssss", "ssssss", "sssssss", "sssssssss", "ssssssssss",
	"t", "tt", "ttt", "tttt", "ttttt", "tttttt", "ttttttt", "ttttttttt", "tttttttttt",
	"u", "uu", "uuu", "uuuu", "uuuuu", "uuuuuu", "uuuuuuu", "uuuuuuuuu", "uuuuuuuuuu",
	"v", "vv", "vvv", "vvvv", "vvvvv", "vvvvvv", "vvvvvvv", "vvvvvvvvv", "vvvvvvvvvv",
	"w", "ww", "www", "wwww", "wwwww", "wwwwww", "wwwwwww", "wwwwwwwww", "wwwwwwwwww",
	"x", "xx", "xxx", "xxxx", "xxxxx", "xxxxxx", "xxxxxxx", "xxxxxxxxx", "xxxxxxxxxx",
	"y", "yy", "yyy", "yyyy", "yyyyy", "yyyyyy", "yyyyyyy", "yyyyyyyyy", "yyyyyyyyyy",
	"z", "zz", "zzz", "zzzz", "zzzzz", "zzzzzz", "zzzzzzz", "zzzzzzzzz", "zzzzzzzzzz"
};

////////////////////////////////

# define INIT_MANAGER(Object) \
	ResourcePool.Manager.Object ## Mgr.initialize(sizeof(Object) * (1 << Object ## Handle::IndexBitCount), sizeof(Object), alignof(Object)); \
	ResourcePool.m_ ## Object = (Object*)ResourcePool.Manager.Object ## Mgr.getStart()

typedef Handle<VertexBufferHandle::IndexBitCount, VertexBufferHandle::GenerationBitCount> VertexArrayObjectHandle;
typedef Handle<5, 3> RenderBufferHandle;
typedef Handle<5, 3> Texture1DHandle;
typedef Handle<16, 16> Texture2DHandle;
typedef Handle<5, 3> Texture3DHandle;
typedef Handle<5, 3> TextureCubeHandle;

bool GLBackend::startupBackend() {
	if (!ACGL::init(sCfgDebugContext)) {
		LOG_ERROR(Renderer, "ACGL initialization failed.");
		return false;
	}

	GLDispatcher::Initialize(); //initialize the dispatcher

	INIT_MANAGER(VertexBuffer);
	INIT_MANAGER(IndexBuffer);
	INIT_MANAGER(VertexArrayObject);
	INIT_MANAGER(ShaderProgram);
	INIT_MANAGER(RenderTarget);
	INIT_MANAGER(ConstantBuffer);
	INIT_MANAGER(RenderBuffer);
	INIT_MANAGER(Texture1D);
	INIT_MANAGER(Texture2D);
	INIT_MANAGER(Texture3D);
	INIT_MANAGER(TextureCube);

	return true;
}

void GLBackend::shutdownBackend() {

}

bool GLBackend::initializeContext()
{
	return true;
}


//
/// Every VB also creates a VAO
void CreateVertexArrayObject() {
    eng_new(VertexArrayObject, ResourcePool.Manager.VertexArrayObjectMgr);
}

VertexBufferHandle GLBackend::createStaticVertexBuffer(size_type bufferSize, Byte* pInitialData) {
    VertexBuffer* vb = eng_new(VertexBuffer, ResourcePool.Manager.VertexBufferMgr);
	vb->setData(bufferSize, pInitialData, GL_STATIC_DRAW);
	
    VertexBufferHandle h = { VertexBufferHandle::_Handle_type(std::distance(ResourcePool.m_VertexBuffer, vb)),0 }; //generate a handle for this object

	CreateVertexArrayObject(); //create a VAO
    VertexArrayObject* vao = ResourcePool.m_VertexArrayObject + h.index;
	vao->attachAllAttributes(ogl::ConstSharedArrayBuffer(vb, [](...) {}));
	return h;
}

VertexBufferHandle GLBackend::createDynamicVertexBuffer(size_type bufferSize, Byte * pInitialData) {
    VertexBuffer* vb = eng_new(VertexBuffer, ResourcePool.Manager.VertexBufferMgr);

	vb->setData(bufferSize, pInitialData, GL_DYNAMIC_DRAW);

    VertexBufferHandle h = { VertexBufferHandle::_Handle_type(std::distance(ResourcePool.m_VertexBuffer, vb)),0 }; //generate a handle for this object

	CreateVertexArrayObject(); //create a VAO
    VertexArrayObject* vao = ResourcePool.m_VertexArrayObject + h.index;
	vao->attachAllAttributes(ogl::ConstSharedArrayBuffer(vb, [](...) {}));
	return h;
}

void GLBackend::destroyVertexBuffer(VertexBufferHandle vb)
{
}

IndexBufferHandle GLBackend::createIndexBuffer(size_type bufferSize, void * pInitialData)
{
	return IndexBufferHandle();
}

void GLBackend::destroyIndexBuffer(IndexBufferHandle ib)
{
}

//
/// ConstantBuffer
ConstantBufferHandle GLBackend::createConstantBuffer() {
    ConstantBuffer* cb = eng_new(ConstantBuffer, ResourcePool.Manager.ConstantBufferMgr);
	
    ConstantBufferHandle cbHdl = { ConstantBufferHandle::_Handle_type(std::distance(ResourcePool.m_ConstantBuffer, cb)), 0 };
	return cbHdl;
}


//
/// Render Targets
Texture* CreateRenderTexture1D(uint32 width, RenderTextureTypeFlags type) {
	Texture* renderTexture = eng_new(Texture1D, ResourcePool.Manager.Texture1DMgr)(width, glGetInternalFormat(type));
	return renderTexture;
}
Texture* CreateRenderTexture2D(uint32 width, uint32 height, RenderTextureTypeFlags type) {
	Texture* renderTexture = eng_new(Texture2D, ResourcePool.Manager.Texture2DMgr)(glm::vec2(width, height), glGetInternalFormat(type));
	return renderTexture;
}
Texture* CreateRenderTexture3D(uint32 width, uint32 height, uint32 depth, RenderTextureTypeFlags type) {
	Texture* renderTexture = eng_new(Texture3D, ResourcePool.Manager.Texture3DMgr)(glm::vec3(width, height, depth), glGetInternalFormat(type));
	return renderTexture;
}
Texture* CreateRenderTextureCube(uint32 width, uint32 height, RenderTextureTypeFlags type) {
	Texture* renderTexture = eng_new(TextureCube, ResourcePool.Manager.TextureCubeMgr)(glm::vec2(width, height), glGetInternalFormat(type));
	return renderTexture;
}
RenderBuffer* CreateRenderBuffer(RenderBufferTypeFlags type) {
	RenderBuffer* renderBuffer = eng_new(RenderBuffer, ResourcePool.Manager.RenderBufferMgr)(glGetInternalFormat(type));
	return renderBuffer;
}

RenderTargetHandle GLBackend::createRenderTarget(RenderTargetLayout rtl)
{
    RenderTarget* rt = eng_new(RenderTarget, ResourcePool.Manager.RenderTargetMgr);
	
	for (int32 tex = 0; tex < rtl.numRenderTextures; ++tex) {
		//decide which texture should be created
		Texture* renderTexture;
		if ((rtl.bufferFlags[tex] & RenderTargetLayout::_1D) != 0) {
			ASSERT(rtl.width > 0, "For a 1D texture: please specify the width");
			renderTexture = CreateRenderTexture1D(rtl.width, rtl.textureTypes[tex]);
		}
		else if ((rtl.bufferFlags[tex] & RenderTargetLayout::_3D) != 0) {
			ASSERT(rtl.width > 0 && rtl.height > 0 && rtl.depth > 0, "For a 3D texture: please specify the width, height and depth");
			renderTexture = CreateRenderTexture3D(rtl.width, rtl.height, rtl.depth, rtl.textureTypes[tex]);
		}
		else if ((rtl.bufferFlags[tex] & RenderTargetLayout::_CUBE) != 0) {
			ASSERT(rtl.width > 0 && rtl.height > 0, "For a Cube texture: please specify the width, height and depth");
			renderTexture = CreateRenderTextureCube(rtl.width, rtl.height, rtl.textureTypes[tex]);
		}
		else {
			ASSERT(rtl.width > 0 && rtl.height > 0, "For a 2D texture: please specify the width, height and depth");
			renderTexture = CreateRenderTexture2D(rtl.width, rtl.height, rtl.textureTypes[tex]);
		}
		
		//decide where to bind
		if ((rtl.bufferFlags[tex] & RenderTargetLayout::DEPTH_ATTACHMENT) != 0)
			rt->setDepthTexture(ogl::ConstSharedTextureBase(renderTexture, [](...) {}));
		else if ((rtl.bufferFlags[tex] & RenderTargetLayout::STENCIL_ATTACHMENT) != 0)
			LOG_ERROR(Renderer, "Stencil attachment is only supported via binding a type of DEPTH_STENCIL");
		else if ((rtl.bufferFlags[tex] & RenderTargetLayout::STENCIL_ATTACHMENT) != 0)
			rt->setDepthTexture(ogl::ConstSharedTextureBase(renderTexture, [](...) {}));
		else
			rt->attachColorTexture(_acglNames[tex], ogl::ConstSharedTextureBase(renderTexture, [](...) {}));
	}

	for (int32 buf = 0; buf < rtl.numRenderBuffer; ++buf) {
		RenderBuffer* renderBuffer = CreateRenderBuffer(rtl.bufferTypes[buf]);

		renderBuffer->setSize(rtl.width, rtl.height);

		//decide where to bind
		if ((rtl.bufferFlags[buf] & RenderTargetLayout::DEPTH_ATTACHMENT) != 0){
			rt->setDepthRenderBuffer(ogl::ConstSharedRenderBuffer(renderBuffer, [](...) {}));
		}
		else if ((rtl.bufferFlags[buf] & RenderTargetLayout::STENCIL_ATTACHMENT) != 0) {
			LOG_ERROR(Renderer, "Stencil attachment is only supported via binding a type of DEPTH_STENCIL");
		}
		else if ((rtl.bufferFlags[buf] & RenderTargetLayout::DEPTH_STENCIL_ATTACHMENT) != 0) {
			rt->setDepthRenderBuffer(ogl::ConstSharedRenderBuffer(renderBuffer, [](...) {}));
		}
		else {
			rt->attachColorRenderBuffer(_acglNames[buf + rtl.numRenderTextures], ogl::ConstSharedRenderBuffer(renderBuffer, [](...) {}));
		}
	}

	ASSERT(rt->validate(), "The Render Target definition is invalid! See output.");

    RenderTargetHandle rtHdl = { RenderTargetHandle::_Handle_type(std::distance(ResourcePool.m_RenderTarget, rt)), 0 };
	return rtHdl;
}


//
/// Dispatcher
//
void GLBackend::Draw(uint32 vertexCount, uint32 startVertex, VertexBufferHandle vbHdl, VertexLayoutHandle vbLayout) {
	VertexArrayObject& vao = ResourcePool.m_VertexArrayObject[vbHdl.index]; //In opengl the VB is automaticall linked into the VAO with the same index
	ASSERT(startVertex == 0, "startVertex isn't 0. OpenGL doesn't support this!");

}

void GLBackend::DrawIndexed(uint32 indexCount, uint32 startIndex, uint32 baseVertex, VertexBufferHandle vbHdl, IndexBufferHandle ibHdl, VertexLayoutHandle vbLayout) {
	VertexArrayObject& vao = ResourcePool.m_VertexArrayObject[vbHdl.index];
	vao.drawRangeElements(startIndex, indexCount);
}

void GLBackend::ClearRenderTarget(RenderTargetHandle rbHdl) {
    RenderTarget& renderTarget = ResourcePool.m_RenderTarget[rbHdl.index]; //Access the RenderTarget via hdl
	renderTarget.clearBuffers();
}

void GLBackend::CopyConstantBufferData(ConstantBufferHandle cbHdl, const void * data, uint32 size) {
	ConstantBuffer& buffer = ResourcePool.m_ConstantBuffer[cbHdl.index];
	buffer.setData(size, data);
}

void GLBackend::ClearScreen() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void GLBackend::ScreenSetClearColor(float32 r, float32 g, float32 b, float32 a) {
	glClearColor(r, g, b, a);
}


ENDNAMESPACE

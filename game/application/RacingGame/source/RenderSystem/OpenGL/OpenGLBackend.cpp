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

#include "nvDDS.h"
#include "Utilities/ByteStream.h"

#include "ResourceSpec.h"

//The effect system relies on nvFX
//create the correct opengl-objects for the effect-system
#include "FxLib.h"
#include "FxLibEx.h"
//----

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
typedef ogl::Shader Shader;
typedef ogl::FrameBufferObject RenderTarget;
typedef ogl::UniformBuffer ConstantBuffer;

typedef ogl::RenderBuffer RenderBuffer;
typedef ogl::TextureBase Texture;
typedef ogl::Texture1D Texture1D;
typedef ogl::Texture2D Texture2D;
typedef ogl::Texture3D Texture3D;
typedef ogl::TextureRectangle TextureRectangle;
typedef ogl::TextureCubeMap TextureCube;

typedef ogl::ArrayBuffer::AttributeVec VertexElementAttributeVec;


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
		PoolAlloc VertexElementAttributeVecMgr;
		PoolAlloc ShaderMgr;

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
			TextureCubeMgr("TextureCubeManager"),
			VertexElementAttributeVecMgr("VertexElementAttributeVecManager"),
			ShaderMgr("ShaderManager")
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
	VertexElementAttributeVec* m_VertexElementAttributeVec;
	Shader* m_Shader;
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
	ResourcePool.Manager.Object ## Mgr.initialize((1 << Object ## Handle::IndexBitCount), sizeof(Object), alignof(Object)); \
	ResourcePool.m_ ## Object = (Object*)ResourcePool.Manager.Object ## Mgr.getStart()

typedef Handle<VertexBufferHandle::IndexBitCount, VertexBufferHandle::GenerationBitCount> VertexArrayObjectHandle;
typedef Handle<5, 3> RenderBufferHandle;
typedef Handle<5, 3> Texture1DHandle;
typedef Handle<16, 16> Texture2DHandle;
typedef Handle<5, 3> Texture3DHandle;
typedef Handle<5, 3> TextureCubeHandle;
typedef Handle<5, 3> VertexElementAttributeVecHandle;
typedef Handle<20, 12> ShaderHandle;

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
	INIT_MANAGER(VertexElementAttributeVec);
	INIT_MANAGER(Shader);

	return true;
}

void GLBackend::shutdownBackend() {

}

bool GLBackend::initializeContext()
{
    glEnable(GL_DEPTH_TEST);
    // glDisable(GL_DEPTH_TEST);
    // glDisable(GL_STENCIL_TEST);
	// glDisable(GL_CULL_FACE);
	return true;
}

VertexBuffer* CreateStaticVertexBuffer(size_type bufferSize, Byte* pInitialData) {
    VertexBuffer* vb = eng_new(VertexBuffer, ResourcePool.Manager.VertexBufferMgr);
	vb->setData(bufferSize, pInitialData, GL_STATIC_DRAW);
    return vb;
}

VertexBuffer* CreateDynamicVertexBuffer(size_type bufferSize, Byte * pInitialData) {
    VertexBuffer* vb = eng_new(VertexBuffer, ResourcePool.Manager.VertexBufferMgr);
	vb->setData(bufferSize, pInitialData, GL_DYNAMIC_DRAW);
    return vb;
}

void UploadIndexData(IndexBuffer* ib, size_type indexBufferSize, void* pInitialData, IndexTypeFlags type, uint32 bufferUsage) {
    size_type sizePerIndex = type == IndexType::_16Bit ? sizeof(uint16) : sizeof(uint32);
    GLenum idxType =  (type == IndexType::_16Bit ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT);
    size_type bufferSize = indexBufferSize;
    ib->setData(bufferSize, pInitialData, bufferUsage);
}

IndexBuffer* CreateIndexBufferStatic(size_type indexBufferSize, void * pInitialData, IndexTypeFlags type)
{
	GLenum idxType = (type == IndexType::_16Bit ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT);
    IndexBuffer* ib = eng_new(IndexBuffer, ResourcePool.Manager.IndexBufferMgr)(idxType);
	UploadIndexData(ib, indexBufferSize, pInitialData, type, GL_STATIC_DRAW);
	return ib;
}

IndexBuffer* CreateIndexBufferDynamic(size_type indexBufferSize, void * pInitialData, IndexTypeFlags type)
{
	GLenum idxType = (type == IndexType::_16Bit ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT);
    IndexBuffer* ib = eng_new(IndexBuffer, ResourcePool.Manager.IndexBufferMgr)(idxType);
	UploadIndexData(ib, indexBufferSize, pInitialData, type, GL_DYNAMIC_DRAW);
	return ib;
}
//*/abStride


VertexElementAttributeVec* CreateVertexElementAttributeVec(const VertexLayoutSpec *specification) {
	VertexElementAttributeVec& veav = *eng_new(VertexElementAttributeVec, ResourcePool.Manager.VertexElementAttributeVecMgr);

	veav.resize(specification->numberOfElements);

	uint32 offset = 0;
	for (uint32 i = 0; i < specification->numberOfElements; ++i) {
		typedef VertexElementAttributeVec::value_type Attribute;
		uint32 size = glGetTypeSize(VertexAttribType::Enum(specification->elementType[i])) * specification->elementCount[i];
        Attribute a = { _acglNames[i], glGetType(VertexAttribType::Enum(specification->elementType[i])), specification->elementCount[i], offset, GL_FALSE, 0, GL_FALSE };
		offset += size;
		veav[i] = a;
	}

	/*
	struct Attribute
	{
    std::string name;       // humtypean readable name, can be used to match the attribute to shader programs
	GLenum      type;       // GL_FLOAT, GL_UNSIGNED_BYTE etc.
	GLint       size;       // #elements per attribute, size in bytes would be: size*sizeof(type)
	GLuint      offset;     // offset in bytes into the array
	GLboolean   normalized; // int types can get normalzed to 0..1 / -1..1 by GL, useful e.g. for colors
	GLuint      divisor;    // vertex divisor for instancing, supported since OpenGL 3.3. Default is 0 (== off)
	GLboolean   isIntegerInShader; // should the data get read as vec or ivec ?
	};
	*/
	return &veav;
}

GeometryHandle GLBackend::createGeometry(const GeometrySpec* specification) {

	ASSERT(specification->numberOfVertexBuffer <= GeometrySpec::MaxVertexBuffer, "Only %d vertex-buffers per geometry allowed.", GeometrySpec::MaxVertexBuffer);
	VertexArrayObject* vao = eng_new(VertexArrayObject, ResourcePool.Manager.VertexArrayObjectMgr)(glGetPrimitiveType(PrimitiveType::Enum(specification->primitiveType)));
	
	bool dynamic = (specification->bufferUsage == BufferUsage::DynamicDraw);
	auto VBCreator = dynamic ? &CreateDynamicVertexBuffer : &CreateStaticVertexBuffer;
	auto IBCreator = dynamic ? &CreateIndexBufferDynamic : &CreateIndexBufferStatic;

	uint32 offset = 0;
	for (uint32 i = 0; i < specification->numberOfVertexBuffer; ++i) {
		uint32 size = specification->vertexStride[i] * specification->numberOfVerticesPerBuffer;
		VertexBuffer* buf = VBCreator(size, specification->vertexData + offset);
		VertexElementAttributeVec* vec = CreateVertexElementAttributeVec(specification->vertexLayout +i);

        for (uint32 j = 0; j < vec->size(); ++j) {
            buf->defineAttribute(vec->at(j));
        }

        if(!buf->isValid())
        {
            LOG_ERROR(Renderer, "VertexBuffer is invalid. See log.");
			return InvalidGeometryHandle;
        }
        vao->attachAllAttributes(ogl::ConstSharedArrayBuffer(buf, [](...) {}));
    }

    IndexBuffer* ib = IBCreator(specification->indexBufferSize, specification->indexData, specification->numberOfVerticesPerBuffer * specification->numberOfVertexBuffer <= MAX_PER_16BIT ? IndexType::_16Bit : IndexType::_32Bit);

    if(!ib->isValid()){
        LOG_ERROR(Renderer,"IndexBuffer is invalid. See log.");
        return InvalidGeometryHandle;
    }

	vao->attachElementArrayBuffer(ogl::ConstSharedElementArrayBuffer(ib, [](...) {}));

	
	GeometryHandle gh = { getElementIndex(vao, ResourcePool.Manager.VertexArrayObjectMgr), 0 };
	return gh;
}

bool GLBackend::updateGeometry(GeometryHandle handle, const GeometrySpec * specification)
{
	ASSERT(specification->numberOfVertexBuffer <= GeometrySpec::MaxVertexBuffer, "Only %d vertex-buffers per geometry allowed.", GeometrySpec::MaxVertexBuffer);	
	VertexArrayObject* vao = getNthElement<VertexArrayObject>(handle.index, ResourcePool.Manager.VertexArrayObjectMgr);

	uint32 offset = 0;
	for (uint32 i = 0; i < specification->numberOfVertexBuffer; ++i) {
		uint32 size = specification->vertexStride[i] * specification->numberOfVerticesPerBuffer;
		VertexBuffer* buf = (VertexBuffer*)vao->getAttributes()[i].arrayBuffer.get();
		buf->setData(size, specification->vertexData + offset);
		
		if (!buf->isValid())
		{
			LOG_ERROR(Renderer, "VertexBuffer is invalid. See log.");
			return false;
		}
	}

	IndexBuffer* ib = (IndexBuffer*)vao->getElementArrayBuffer().get();
	UploadIndexData(ib, specification->indexBufferSize, specification->indexData, (specification->numberOfVerticesPerBuffer * specification->numberOfVertexBuffer <= MAX_PER_16BIT) ? IndexType::_16Bit : IndexType::_32Bit, ib->getUsage());

	return true;
}

VertexLayoutHandle GLBackend::createVertexLayout(const VertexLayoutSpec* specification)
{
	VertexElementAttributeVec* veav = CreateVertexElementAttributeVec(specification);
	VertexLayoutHandle vlh = { getElementIndex(veav, ResourcePool.Manager.VertexElementAttributeVecMgr), 0 };
	return vlh;
}

ConstantBufferHandle GLBackend::createConstantBuffer(nvFX::ICstBuffer * effectBuffer, uint32 size)
{
	uint32 glBuff = 0;
	glGenBuffers(1, &glBuff);
	glBindBuffer(GL_UNIFORM_BUFFER, glBuff);
	glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STREAM_DRAW);
	effectBuffer->setGLBuffer(glBuff);
	effectBuffer->update();
	ConstantBufferHandle hdl = { glBuff, 0 };
	return hdl;
}

//
/// ConstantBuffer
ConstantBufferHandle GLBackend::createConstantBuffer(ConstantBufferSpec specification) {
    ConstantBuffer* cb = eng_new(ConstantBuffer, ResourcePool.Manager.ConstantBufferMgr);

	cb->bindBufferBase(specification.location);
    ConstantBufferHandle cbHdl = { getElementIndex(cb, ResourcePool.Manager.ConstantBufferMgr), 0 };
	return cbHdl;
}

void GLBackend::destroyConstantBufferFX(ConstantBufferHandle hdl)
{
	uint32 glBuff = hdl.index;
	glDeleteBuffers(1, &glBuff);
}

void GLBackend::destroyConstantBuffer(ConstantBufferHandle hdl)
{
	ConstantBuffer* buf = getNthElement<ConstantBuffer>(hdl.index, ResourcePool.Manager.ConstantBufferMgr);
	eng_delete(buf, ResourcePool.Manager.ConstantBufferMgr);
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
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
    RenderTargetHandle rtHdl = { getElementIndex(rt, ResourcePool.Manager.RenderTargetMgr), 0 };
	return rtHdl;
}

TextureHandle GLBackend::createTexture(const TextureSpec * specification)
{
    ByteBuffer buffer((const ansichar*)specification->m_TextureData, specification->m_DataSize);
    std::istream textureStream(&buffer);
	nv_dds::CDDSImage img;
	img.load(textureStream, true);

	
	TextureHandle handle = { 0,0 };
	switch (img.get_type()) {
	case nv_dds::TextureType::TextureFlat:
	{
		Texture2D* renderTexture = eng_new(Texture2D, ResourcePool.Manager.Texture2DMgr)();
		renderTexture->bind();
		img.upload_texture2D();
		handle.index = getElementIndex(renderTexture, ResourcePool.Manager.Texture2DMgr);
	}
		break;
	case nv_dds::TextureType::TextureCubemap:
	{
		TextureCube* renderTexture = eng_new(TextureCube, ResourcePool.Manager.TextureCubeMgr)();
		renderTexture->bind();
		img.upload_textureCubemap();
		handle.index = getElementIndex(renderTexture, ResourcePool.Manager.TextureCubeMgr);
	}
		break;
	case nv_dds::TextureType::Texture3D:
	{
		Texture3D* renderTexture = eng_new(Texture3D, ResourcePool.Manager.Texture3DMgr)();
		renderTexture->bind();
		img.upload_texture2D();
		handle.index = getElementIndex(renderTexture, ResourcePool.Manager.Texture3DMgr);
	}
		break;
	case nv_dds::TextureType::TextureNone:
		LOG_ERROR(General, "Texture specification invalid. No DDS texture.");
		return InvalidTextureHandle;
	}
	return handle;
}

bool GLBackend::updateTexture(TextureHandle handle, const TextureSpec * specification)
{
	ByteBuffer buffer((const ansichar*)specification->m_TextureData, specification->m_DataSize);
	std::istream textureStream(&buffer);
	nv_dds::CDDSImage img;
	img.load(textureStream, true);

	
	switch (img.get_type()) {
	case nv_dds::TextureType::TextureFlat:
	{
		Texture2D* renderTexture = getNthElement<Texture2D>(handle.index,ResourcePool.Manager.Texture2DMgr);
		renderTexture->bind();
		img.upload_texture2D();
		handle.index = getElementIndex(renderTexture, ResourcePool.Manager.Texture2DMgr);
	}
	break;
	case nv_dds::TextureType::TextureCubemap:
	{
		TextureCube* renderTexture = getNthElement<TextureCube>(handle.index, ResourcePool.Manager.TextureCubeMgr);
		renderTexture->bind();
		img.upload_textureCubemap();
		handle.index = getElementIndex(renderTexture, ResourcePool.Manager.TextureCubeMgr);
	}
	break;
	case nv_dds::TextureType::Texture3D:
	{
		Texture3D* renderTexture = getNthElement<Texture3D>(handle.index, ResourcePool.Manager.Texture3DMgr);
		renderTexture->bind();
		img.upload_texture2D();
		handle.index = getElementIndex(renderTexture, ResourcePool.Manager.Texture3DMgr);
	}
	break;
	case nv_dds::TextureType::TextureNone:
		LOG_ERROR(General, "Texture specification invalid. No DDS texture.");
		return false;
	}
	return true;
}

ShaderProgramHandle GLBackend::createShaderProgram(ShaderProgramSpec specification)
{
	ShaderProgram* shaderProgram = eng_new(ShaderProgram, ResourcePool.Manager.ShaderProgramMgr);

	if (specification.data[ShaderProgramSpec::VERTEX_SHADER] != nullptr) { 
		Shader* shader = eng_new(Shader, ResourcePool.Manager.ShaderMgr)(GL_VERTEX_SHADER);
		ASSERT(shader->setSource(specification.data[ShaderProgramSpec::VERTEX_SHADER]), "VertexShader compilation error! See output.");
		shaderProgram->attachShader(ogl::ConstSharedShader(shader, [](...) {}));
	}

	if (specification.data[ShaderProgramSpec::FRAGMENT_SHADER] != nullptr) {
		Shader* shader = eng_new(Shader, ResourcePool.Manager.ShaderMgr)(GL_FRAGMENT_SHADER);
		ASSERT(shader->setSource(specification.data[ShaderProgramSpec::FRAGMENT_SHADER]), "FragmentShader compilation error! See output.");
		shaderProgram->attachShader(ogl::ConstSharedShader(shader, [](...) {}));
	}

	if (specification.data[ShaderProgramSpec::GEOMETRY_SHADER] != nullptr) {
		Shader* shader = eng_new(Shader, ResourcePool.Manager.ShaderMgr)(GL_GEOMETRY_SHADER);
		ASSERT(shader->setSource(specification.data[ShaderProgramSpec::GEOMETRY_SHADER]), "GeometryShader compilation error! See output.");
		shaderProgram->attachShader(ogl::ConstSharedShader(shader, [](...) {}));
	}

	if (specification.data[ShaderProgramSpec::TESSELLATION_CONTROL_SHADER] != nullptr) {
		Shader* shader = eng_new(Shader, ResourcePool.Manager.ShaderMgr)(GL_TESS_CONTROL_SHADER);
		ASSERT(shader->setSource(specification.data[ShaderProgramSpec::TESSELLATION_CONTROL_SHADER]), "TessellationControlShader compilation error! See output.");
		shaderProgram->attachShader(ogl::ConstSharedShader(shader, [](...) {}));
	}

	if (specification.data[ShaderProgramSpec::TESSELLATION_EVALUATION_SHADER] != nullptr) {
		Shader* shader = eng_new(Shader, ResourcePool.Manager.ShaderMgr)(GL_TESS_EVALUATION_SHADER);
		ASSERT(shader->setSource(specification.data[ShaderProgramSpec::TESSELLATION_EVALUATION_SHADER]), "TessellationEvaluationShader compilation error! See output.");
		shaderProgram->attachShader(ogl::ConstSharedShader(shader, [](...) {}));
	}

	ASSERT(shaderProgram->link(), "ShaderProgram linkage error! See output.");

	ShaderProgramHandle spHdl = { getElementIndex(shaderProgram, ResourcePool.Manager.ShaderProgramMgr), 0 };

	return spHdl;
}


//
/// Dispatcher
//
void GLBackend::Draw(uint32 vertexCount, uint32 startVertex, VertexBufferHandle vbHdl, VertexLayoutHandle vbLayout) {
	
}

void GLBackend::DrawIndexed(uint32 indexCount, uint32 startIndex, uint32 baseVertex, VertexBufferHandle vbHdl, IndexBufferHandle ibHdl, VertexLayoutHandle vlHdl) {

}

void GLBackend::DrawGeometry(uint32 indexCount, uint32 startIndex, GeometryHandle geoHdl) {
	VertexArrayObject* vao = getNthElement<VertexArrayObject>(geoHdl.index, ResourcePool.Manager.VertexArrayObjectMgr);
	uint32 count = indexCount <= 0 ? vao->getIndexCount() : indexCount;
	vao->bind();
	size_type sizePerIndex = vao->getIndexType() == GL_UNSIGNED_SHORT ? sizeof(uint16) : sizeof(uint32);
	glDrawRangeElements(vao->getMode(), startIndex, startIndex + indexCount, indexCount, vao->getIndexType(), (GLvoid*)(sizePerIndex * startIndex));
	glBindVertexArray(0);
}

void GLBackend::ActivateShader(ShaderProgramHandle shaderProgram)
{
	ShaderProgram* program = getNthElement<ShaderProgram>(shaderProgram.index, ResourcePool.Manager.ShaderProgramMgr);
	program->use();
}

void GLBackend::ClearRenderTarget(RenderTargetHandle rbHdl) {
    RenderTarget* renderTarget = getNthElement<RenderTarget>(rbHdl.index, ResourcePool.Manager.RenderTargetMgr); //Access the RenderTarget via hdl
	renderTarget->clearBuffers();
	glBindFramebuffer(GL_FRAMEBUFFER, 0); //Stateless -> so reset to default
}

void GLBackend::CopyConstantBufferData(ConstantBufferHandle cbHdl, const void * data, uint32 size) {	
	ConstantBuffer* buffer = getNthElement<ConstantBuffer>(cbHdl.index, ResourcePool.Manager.ConstantBufferMgr);
	buffer->setData(size, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0); //Stateless -> reset
	
}

void GLBackend::ClearScreen() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void GLBackend::ScreenSetClearColor(float32 r, float32 g, float32 b, float32 a) {
	glClearColor(r, g, b, a);
}


ENDNAMESPACE

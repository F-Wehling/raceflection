#include "RenderSystem/HardwareResourcePool.h"

#include <ACGL/OpenGL/Objects.hh>

BEGINNAMESPACE

//Define storage buffer
namespace ogl = ACGL::OpenGL;

typedef ogl::VertexArrayObject VertexArrayObject;
typedef ogl::ShaderProgram ShaderProgram;
typedef ogl::FrameBufferObject FrameBufferObject;

VertexArrayObject* gVertexArrayObjects; //combination of Vertex and Index Arrays
ShaderProgram* gShaderProgram;//[HardwareResourcePool::MaxShaders];

HardwareResourcePool::HardwareResourcePool() :
	m_VertexArrayObjects("VertexArrayObjects"),
	m_ShaderPrograms("ShaderProgram")
{
	//gVertexArrayObjects = eng_new_array(VertexArrayObject[MaxVertexArrays]);
	//gShaderProgram = eng_new_array(ShaderProgram[MaxShaderPrograms]);

	//m_VertexArrayObjects.initialize(sizeof(VertexArrayObject), alignof(VertexArrayObject), gVertexArrayObjects, sizeof(VertexArrayObject) * HardwareResourcePool::MaxVertexArrays);
	//m_ShaderPrograms.initialize(sizeof(ShaderProgram), alignof(ShaderProgram), gShaderProgram, sizeof(ShaderProgram) * HardwareResourcePool::MaxShaderPrograms);

}

HardwareResourcePool::~HardwareResourcePool()
{
	//eng_delete_array(gVertexArrayObjects);
	//eng_delete_array(gShaderProgram);
}

VertexArrayHandle HardwareResourcePool::getVertexArray() {
	VertexArrayObject* vab = eng_new(VertexArrayObject, m_VertexArrayObjects);
	VertexArrayHandle hdl = { (uint32)std::distance(gVertexArrayObjects, vab), 0 };
	return hdl;
}

void HardwareResourcePool::freeVertexArray(VertexArrayHandle hdl)
{
	VertexArrayObject* vab = gVertexArrayObjects + hdl.vertex_buffer.index;
	eng_delete(vab, m_VertexArrayObjects);
}

RenderTargetHandle HardwareResourcePool::getRenderTarget() {
    RenderTargetHandle hdl = { 0, 0 };
    return hdl;
}

ENDNAMESPACE


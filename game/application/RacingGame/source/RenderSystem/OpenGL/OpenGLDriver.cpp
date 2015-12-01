#include "RenderSystem/OpenGL/OpenGLDriver.h"

#include "Logging/Logging.h"


BEGINNAMESPACE

namespace gl_backend {

	OpenGLDriver::OpenGLDriver()
		:m_IsInitialized(false)
	{
	}

	OpenGLDriver::~OpenGLDriver()
	{
	}

	bool OpenGLDriver::initialize()
	{

		return true;
	}

	bool OpenGLDriver::isInitialized() const
	{
		return m_IsInitialized;
	}
}

ENDNAMESPACE
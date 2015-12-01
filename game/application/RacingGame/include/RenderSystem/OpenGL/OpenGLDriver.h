#pragma once

BEGINNAMESPACE

namespace gl_backend {
	class OpenGLDriver {
	private:
		OpenGLDriver();
	public:
		~OpenGLDriver();

		bool initialize();
		bool isInitialized() const;
	private:
		bool m_IsInitialized;
	};
}

ENDNAMESPACE
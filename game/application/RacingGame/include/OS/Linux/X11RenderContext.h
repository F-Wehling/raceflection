#pragma once
//This class is only valid for Windows OS
#if OS_LINUX

#include "RenderSystem/RenderContext.h"

typedef struct __GLXcontextRec* GLXContext;

BEGINNAMESPACE

class X11Window;

class X11RenderContextGL : public RenderContext {
public:
    X11RenderContextGL(X11Window* contextForWindow);
    ~X11RenderContextGL();
protected:
	virtual void swapBuffer();
	virtual void makeCurrent();
	virtual bool valid();
private:
    GLXContext m_Context;

    X11Window * m_RefWindow;
};

typedef X11RenderContextGL PlatformGLContext;

ENDNAMESPACE

#endif

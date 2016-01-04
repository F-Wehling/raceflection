#if OS_LINUX

#include "OS/Linux/X11RenderContext.h"
#include "OS/Linux/X11Window.h"
#include "WindowSystem/WindowDesc.h"

#include <X11/Xlib.h>
#include <GL/glx.h>

#include "Logging.h"

BEGINNAMESPACE

static bool isExtensionSupported(const char *extList, const char *extension)
{
  const char *start;
  const char *where, *terminator;

  /* Extension names should not have spaces. */
  where = strchr(extension, ' ');
  if (where || *extension == '\0')
    return false;

  /* It takes a bit of care to be fool-proof about parsing the
     OpenGL extensions string. Don't be fooled by sub-strings,
     etc. */
  for (start=extList;;) {
    where = strstr(start, extension);

    if (!where)
      break;

    terminator = where + strlen(extension);

    if ( where == start || *(where - 1) == ' ' )
      if ( *terminator == ' ' || *terminator == '\0' )
        return true;

    start = terminator;
  }
  return false;
}

X11RenderContextGL::X11RenderContextGL(X11Window* contextForWindow)
	: m_RefWindow(contextForWindow)
{
    Display* display = (Display*)(contextForWindow->getDisplay());
    XVisualInfo* vi = (XVisualInfo*)(contextForWindow->getVisualInfo());
    GLXFBConfig fbConfig = (GLXFBConfig)(contextForWindow->getFBConfig());
    ::Window window = (::Window)(contextForWindow->getNativeHandle());
    WindowDesc desc = contextForWindow->getWindowDesc();

    GLXContext TempContext=glXCreateContext(display,vi,0,GL_TRUE);
    if(TempContext==0)
    {
          LOG_ERROR(Renderer,"Error: Could not create an OpenGL rendering context.");
          return;
    }
    glXMakeCurrent(display,window,TempContext);

    //Typdefinition von wglCreateContextAttribsARB
    typedef GLXContext (* GLXCREATECONTEXTATTRIBSARBPROC)(Display *dpy,GLXFBConfig config,GLXContext share_context,Bool direct,const int *attrib_list);
    //abfragen, ob die Funktion existiert
    GLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB = (GLXCREATECONTEXTATTRIBSARBPROC)glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");

    if(glXCreateContextAttribsARB == NULL)//wenn es diese Funktion nicht gibt, dann hat der -->Treiber<-- keinen OpenGL3 support
    {
      LOG_ERROR(Renderer,"Error: There is no OpenGL3 context support.");
      return;
    }
    int32 minor=0;
    int32 major=0;
    glGetIntegerv(GL_MAJOR_VERSION,&major);
    glGetIntegerv(GL_MINOR_VERSION,&minor);
    int32 attribs[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, major,
        GLX_CONTEXT_MINOR_VERSION_ARB, minor,
        GLX_CONTEXT_FLAGS_ARB,GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB | GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB
    #   if DEBUG_BUILD
        | GLX_CONTEXT_DEBUG_BIT_ARB
    #   endif
        ,
        0
    };


    for (int32 i=minor; i>=0;--i)
    {
      attribs[3]=i;
      m_Context=glXCreateContextAttribsARB(display, fbConfig, NULL, true, attribs);
      if (!m_Context)//versuche ein OpenGL3.i fähigen Context zu erstellen
      {
        if (i<=0)
        {
          glXDestroyContext(display,TempContext);
          LOG_ERROR(Renderer,"Error: Couldn't create OpenGL3 context.");
          return;
        }
      }
      break;
    }
    glXMakeCurrent(display,window,m_Context);

    // test
    glClearColor(0.4,0.4,0.4,1.0);

    glXDestroyContext(display,TempContext);
}

X11RenderContextGL::~X11RenderContextGL()
{
    Display* disp = (Display*) m_RefWindow->getDisplay();
    glXMakeCurrent( disp, 0, 0 );
    glXDestroyContext( disp, m_Context );
}

void X11RenderContextGL::_swapBuffer()
{
    Display* disp = (Display*) m_RefWindow->getDisplay();
    ::Window win = (::Window) m_RefWindow->getNativeHandle();
    glXSwapBuffers(disp, win);
}

void X11RenderContextGL::_makeCurrent()
{
    Display* disp = (Display*) m_RefWindow->getDisplay();
    ::Window win = (::Window) m_RefWindow->getNativeHandle();
    glXMakeCurrent(disp, win, m_Context);
}

bool X11RenderContextGL::_valid()
{
    return m_Context != nullptr;
}

ENDNAMESPACE

#endif

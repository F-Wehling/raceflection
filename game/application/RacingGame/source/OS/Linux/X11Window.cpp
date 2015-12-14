#if OS_LINUX

#include "OS/Linux/X11Window.h"
#include "OS/Linux/X11RenderContext.h"

#include "Logging.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <GL/glx.h>


BEGINNAMESPACE

::Display* sX11Display = nullptr;
X11::Window sX11FocusWindow = 0;
::XContext sX11Context = 0;

extern const tchar* sAppClassName;

X11Window::X11Window() : Window() {
    //glfwInit();
}

X11Window::~X11Window() {
}

bool X11Window::_impl_open(WindowDesc desc)
{
    union {
        X11::XVisualInfo_a* _vi;
        XVisualInfo* vi;
    };

    if( !sX11Display ) {
        sX11Display = XOpenDisplay(0);
    }
    if( sX11Context == 0 ){
        sX11Context = XUniqueContext();
    }
    m_Display  = sX11Display;

    if(!m_Display){
        LOG_ERROR(General, "Cannt connect to X server");
        return false;
    }


    int32 visual_attribs[] = {
        GLX_X_RENDERABLE , 1,
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_RENDER_TYPE     , GLX_RGBA_BIT,
        GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
        GLX_RED_SIZE        , desc.red_bits,
        GLX_GREEN_SIZE      , desc.green_bits,
        GLX_BLUE_SIZE       , desc.blue_bits,
        GLX_ALPHA_SIZE      , desc.alpha_bits,
        GLX_DEPTH_SIZE      , desc.depth_bits,
        GLX_STENCIL_SIZE    , desc.stencil_bits,
        GLX_DOUBLEBUFFER    , True,
        //GLX_SAMPLE_BUFFERS  , 1,
        //GLX_SAMPLES         , 4,
        None
    };

    int32 fbcount;
    GLXFBConfig* fbc = glXChooseFBConfig(m_Display, DefaultScreen(m_Display), visual_attribs, &fbcount);

    if(!fbc) {
        LOG_ERROR(General,"Failed to rectrieve a framebuffer config");
        return false;
    }

    int32 best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;

    for (int32 i=0; i<fbcount; ++i) {
        XVisualInfo *vi = glXGetVisualFromFBConfig( m_Display, fbc[i] );
        if ( vi )
        {
          int32 samp_buf, samples;
          glXGetFBConfigAttrib( m_Display, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf );
          glXGetFBConfigAttrib( m_Display, fbc[i], GLX_SAMPLES       , &samples  );

          if ( best_fbc < 0 || samp_buf && samples > best_num_samp )
                best_fbc = i, best_num_samp = samples;
          if ( worst_fbc < 0 || !samp_buf || samples < worst_num_samp )
                worst_fbc = i, worst_num_samp = samples;
        }
    }

    GLXFBConfig bestFbc = fbc[ best_fbc ];

    // Be sure to free the FBConfig list allocated by glXChooseFBConfig()
    XFree( fbc );

    m_FBConfig = bestFbc;

    // Get a visual
    vi = glXGetVisualFromFBConfig( m_Display, bestFbc );
    m_VisualInfo = _vi;

    XSetWindowAttributes swa;
    Colormap cmap;
    swa.colormap = cmap = XCreateColormap( m_Display,
                                             RootWindow( m_Display, vi->screen ),
                                             vi->visual, AllocNone );
    swa.background_pixmap = None ;
    swa.border_pixel      = 0;
    swa.event_mask        = StructureNotifyMask;

    //int32 ret = XChangeWindowAttributes(display, window, CWBorderPixel|CWColormap|CWEventMask, &swa);

    m_Window = XCreateWindow(
                    m_Display,
                    RootWindow( m_Display, vi->screen ),
                    desc.posX, desc.posY, desc.width, desc.height,
                    0,
                    vi->depth,
                    InputOutput, vi->visual, CWBorderPixel|CWColormap|CWEventMask, &swa
                );

    XStoreName( m_Display, m_Window, desc.title);

    uint32 ctx = sX11Context;

    XSaveContext(m_Display, m_Window, sX11Context, (XPointer) this);
/*

    XWMHints *hints;
    hints.x = desc.posX;
    hints.y = desc.posY;
    hints.width = desc.width;
    hints.height = desc.height;
    hints.flags = USPosition|USSize;

    XWMHints *startup_state;
    startup_state = XAllocWMHints();
    startup_state->initial_state = NormalState;
    startup_state->flags = StateHint;

    XSetWMProperties(display, window, &textprop, &textprop, NULL, 0, &hints, startup_state, NULL);

    XFree(startup_state);
*/
    //if ( !window )
    //{
    //    return;
    //}
    //
    // => https://www.opengl.org/wiki/Tutorial:_OpenGL_3.0_Context_Creation_%28GLX%29
    //

    // Done with the visual info data
    //XFree( vi );

    //m_Window = (void*)XCreateSimpleWindow((x11Display*)m_Display, DefaultRootWindow((x11Display*)m_Display), desc.posX, desc.posY, desc.width, desc.height, 0,0, 0);

    XMapWindow(m_Display, m_Window);

    Atom WM_DELETE_WINDOW = XInternAtom(m_Display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(m_Display, m_Window, &WM_DELETE_WINDOW, 1);

    XSelectInput(m_Display, m_Window, StructureNotifyMask);

    XEvent event;
    do{XNextEvent(m_Display, &event); }while(event.type != MapNotify);

	return true;
}

bool X11Window::_impl_isClosed()
{
    return m_Window == 0;
}

void X11Window::_impl_setSize(int32 width, int32 height)
{
    if (m_Desc.resizable)
    {
       // Update window size restrictions to match new window size

       XSizeHints* hints = XAllocSizeHints();

       hints->flags |= (PMinSize | PMaxSize);
       hints->min_width  = hints->max_width  = width;
       hints->min_height = hints->max_height = height;

       XSetWMNormalHints(m_Display, m_Window, hints);
       XFree(hints);
    }

    XResizeWindow(m_Display, m_Window, width, height);
    XFlush(m_Display);
}

void X11Window::_impl_getSize(int32 & width, int32 & height) const
{
	width = m_Width; 
	height = m_Height;
}

void X11Window::_impl_setPosition(int32 xPos, int32 yPos)
{
    XMoveWindow(m_Display, m_Window, xPos, yPos);
    XFlush(m_Display);
    m_XPos = xPos;
    m_YPos = yPos;
}

void X11Window::_impl_getPosition(int32 & xpos, int32 & ypos) const
{
	xpos = m_XPos;
	ypos = m_YPos;
}

void X11Window::_impl_getFramebufferSize(int32 & width, int32 & height) const
{
	width = m_Width;
	height = m_Height;
}

void X11Window::_impl_setTitle(const ansichar * title)
{
    XStoreName(m_Display, m_Window, title);
    XSetIconName(m_Display, m_Window, title);
}

void X11Window::_impl_minimize()
{    
    XIconifyWindow(m_Display, m_Window, DefaultScreen(m_Display));
}

void X11Window::_impl_maximize()
{
    XMapWindow(m_Display, m_Window);
}

void X11Window::_impl_restore()
{
    XMapWindow(m_Display, m_Window);
}

bool X11Window::_impl_isMinimized() const
{

}

bool X11Window::_impl_isMaximized() const
{

}

void X11Window::_impl_show()
{
    XMapRaised(m_Display, m_Window);
    XFlush(m_Display);
}

void X11Window::_impl_hide()
{
    XUnmapWindow(m_Display, m_Window);
    XFlush(m_Display);
}

bool X11Window::_impl_isVisible() const
{

}

bool X11Window::_impl_hasFocus() const
{
    return sX11FocusWindow == m_Window;
}

bool X11Window::_impl_isResizeable() const
{
	return m_Desc.resizable;
}

bool X11Window::_impl_isDecorated() const
{
	return m_Desc.decorated;
}

void X11Window::_impl_getOpenGLContext(int32 & major, int32 & minor, int32 & revision) const
{
}

bool X11Window::_impl_isForwardCompatible() const
{
	return false;
}

bool X11Window::_impl_isDebugContext() const
{
	return false;
}

FramebufferDesc X11Window::_impl_getWindowFramebufferDesc() const
{
	return FramebufferDesc();
}

void X11Window::_impl_swapBuffers()
{
	if (m_RenderContext) m_RenderContext->swapBuffer();
}

void X11Window::_impl_destroy()
{
    XDeleteContext(m_Display, m_Window, sX11Context);
    XUnmapWindow(m_Display, m_Window);
    XDestroyWindow(m_Display, m_Window);
    m_Window = 0;
}

void * X11Window::_impl_getNativeHandle()
{
    return (void*)m_Window;
}

bool X11Window::_impl_createContext(RenderEngineTypeFlags contextType)
{
    static_assert(sizeof(X11RenderContextGL) <= sizeof(ContextStorage), "The render-context storage is too small");
	switch (contextType) {
    case RenderEngineType::OpenGL:
        m_RenderContext = new (__contextStorage)X11RenderContextGL(this);
		if (m_RenderContext->valid()) return true;
		m_RenderContext->~RenderContext();//free
		break;
    case RenderEngineType::Null:
        m_RenderContext = new (__contextStorage)X11RenderContextNull(this);
        return true;
	default: 
        LOG_ERROR(Renderer, "No context for %s available.", RenderEngineType::toString(contextType));
		return false;
	}
	return false;
}

ENDNAMESPACE

#endif

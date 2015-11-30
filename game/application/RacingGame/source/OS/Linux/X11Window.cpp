#if OS_LINUX

#include "OS/Linux/X11Window.h"
#include "OS/Linux/X11RenderContext.h"

#include "Logging.h"

#include <X11/Xlib.h>
#include <GL/glx.h>

BEGINNAMESPACE

typedef Display x11Display;
typedef ::Window x11Window;

extern const tchar* sAppClassName;

X11Window::X11Window() : Window() {
    //glfwInit();
}

X11Window::~X11Window() {
}

bool X11Window::_impl_open(WindowDesc desc)
{
    Display* display  = XOpenDisplay(0);
    m_Display = (void*)display;
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
    GLXFBConfig* fbc = glXChooseFBConfig(display, DefaultScreen(display), visual_attribs, &fbcount);

    if(!fbc) {
        LOG_ERROR(General,"Failed to rectrieve a framebuffer config");
        return false;
    }

    int32 best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;

    for (int32 i=0; i<fbcount; ++i) {
        XVisualInfo *vi = glXGetVisualFromFBConfig( display, fbc[i] );
        if ( vi )
        {
          int32 samp_buf, samples;
          glXGetFBConfigAttrib( display, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf );
          glXGetFBConfigAttrib( display, fbc[i], GLX_SAMPLES       , &samples  );

          if ( best_fbc < 0 || samp_buf && samples > best_num_samp )
                best_fbc = i, best_num_samp = samples;
          if ( worst_fbc < 0 || !samp_buf || samples < worst_num_samp )
                worst_fbc = i, worst_num_samp = samples;
        }
    }

    GLXFBConfig bestFbc = fbc[ best_fbc ];

    // Be sure to free the FBConfig list allocated by glXChooseFBConfig()
    XFree( fbc );

    m_FBConfig = (void*)bestFbc;

    // Get a visual
    XVisualInfo *vi = glXGetVisualFromFBConfig( display, bestFbc );
    m_VisualInfo = (void*) vi;

    XSetWindowAttributes swa;
    Colormap cmap;
    swa.colormap = cmap = XCreateColormap( display,
                                             RootWindow( display, vi->screen ),
                                             vi->visual, AllocNone );
    swa.background_pixmap = None ;
    swa.border_pixel      = 0;
    swa.event_mask        = StructureNotifyMask;

    //int32 ret = XChangeWindowAttributes(display, window, CWBorderPixel|CWColormap|CWEventMask, &swa);

    m_Window = (void*) XCreateWindow(
                display,
                RootWindow( display, vi->screen ),
                desc.posX, desc.posY, desc.width, desc.height,
                0,
                vi->depth,
                InputOutput, vi->visual, CWBorderPixel|CWColormap|CWEventMask, &swa );

    XTextProperty textprop;
    textprop.value = (unsigned char*)desc.title;
    textprop.encoding = XA_STRING;
    textprop.format = 8;
    textprop.nitems = strlen(desc.title);

    XWMHints *startup_state;
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

    //if ( !window )
    //{
    //    return;
    //}
    //
    // => https://www.opengl.org/wiki/Tutorial:_OpenGL_3.0_Context_Creation_%28GLX%29
    //

    // Done with the visual info data
    XFree( vi );

    //m_Window = (void*)XCreateSimpleWindow((x11Display*)m_Display, DefaultRootWindow((x11Display*)m_Display), desc.posX, desc.posY, desc.width, desc.height, 0,0, 0);

    XMapWindow((x11Display*)m_Display, (x11Window)m_Window);

    XSelectInput((x11Display*)m_Display, (x11Window)m_Window, StructureNotifyMask);

    XEvent event;
    do{XNextEvent((x11Display*)m_Display, &event); }while(event.type != MapNotify);

	return true;
}

bool X11Window::_impl_isClosed()
{
}

void X11Window::_impl_setSize(int32 width, int32 height)
{

}

void X11Window::_impl_getSize(int32 & width, int32 & height) const
{
	width = m_Width; 
	height = m_Height;
}

void X11Window::_impl_setPosition(int32 xPos, int32 yPos)
{
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

}

void X11Window::_impl_minimize()
{

}

void X11Window::_impl_maximize()
{

}

void X11Window::_impl_restore()
{

}

bool X11Window::_impl_isMinimized() const
{

}

bool X11Window::_impl_isMaximized() const
{

}

void X11Window::_impl_show()
{

}

void X11Window::_impl_hide()
{

}

bool X11Window::_impl_isVisible() const
{

}

bool X11Window::_impl_hasFocus() const
{
	return false;
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

}

void * X11Window::_impl_getNativeHandle()
{
    return (void*)m_Window;
}

bool X11Window::_impl_createContext(ContextTypeFlags contextType)
{
    static_assert(sizeof(X11RenderContextGL) <= sizeof(ContextStorage), "The render-context storage is too small");
	switch (contextType) {
	case ContextType::OpenGL:
        m_RenderContext = new (__contextStorage)X11RenderContextGL(this);
		if (m_RenderContext->valid()) return true;
		m_RenderContext->~RenderContext();//free
		break;
	default: 
		LOG_ERROR(Renderer, "No context for %s available.", ContextType::toString(contextType));
		return false;
	}
	return false;
}
/*
//
/// message queue
int32 X11Window::processMessage(HWND hWnd, uint32 msg, WPARAM wParam, LPARAM lParam)
{
    X11Window* window = reinterpret_cast<X11Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	if (!window) return DefWindowProc(hWnd, msg, wParam, lParam); // return if window-ptr is 0

	switch (msg) {
	case WM_CREATE:
		break;
	case WM_DESTROY:
		if (window->m_delegates) window->m_delegates->onFocusChanged;
		if (window->m_delegates) window->m_delegates->onMinimizeResized;
		if (window->m_delegates) window->m_delegates->onPositionChanged;
		break;
	case WM_ACTIVATE:
		return 0;
	case WM_ACTIVATEAPP:
		break;
	case WM_NCACTIVATE:
		break;
	case WM_NCPAINT:
		break;
	case WM_CLOSE:
		if (window->m_delegates) window->m_delegates->onClose.invoke(window);
		break;
	case WM_SHOWWINDOW:
		/*
		switch (lParam) {
		case SW_PARENTCLOSING: window->listener.signal(window, WindowEvent::Generate(WindowEvent::ParentMinimize)); break;
		case SW_PARENTOPENING: window->listener.signal(window, WindowEvent::Generate(WindowEvent::ParentRestore)); break;
		default:;
		}
		switch (wParam) {
		case TRUE: window->listener.signal(window, WindowEvent::Generate(WindowEvent::Show, true));; break;
		case FALSE: window->listener.signal(window, WindowEvent::Generate(WindowEvent::Close, false)); break;
		default:;
		}
        //* /
		break;
	case WM_SIZE:
	{
		/*
		switch (wParam) {
		case SIZE_MAXHIDE:
			window->listener.signal(window, WindowEvent::Generate(WindowEvent::ParentMaximize));
			break;
		case SIZE_MAXIMIZED:
			window->listener.signal(window, WindowEvent::Generate(WindowEvent::Maximize));
			break;
		case SIZE_MINIMIZED:
			window->listener.signal(window, WindowEvent::Generate(WindowEvent::Minimize));
			break;
		default:;
		}
        //* /
		int32 newWidth = (int32)(int16)(LOWORD(lParam));
		int32 newHeight = (int32)(int16)(HIWORD(lParam));
		WindowDesc desc = window->getWindowDesc();
		if (!desc.decorated) {
			window->adjustWindowRegion(newWidth, newHeight);
		}
		if (window->m_delegates) window->m_delegates->onResize(window, Size2i(newWidth, newHeight));
	}break;
	case WM_MOVE:
	{
		int32 newX = (int32)(int16)(LOWORD(lParam));
		int32 newY = (int32)(int16)(HIWORD(lParam));
		if (window->m_delegates) window->m_delegates->onPositionChanged(window, Point2i(newX, newY));
	}
	case WM_SIZING:
		break;
	default:;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
*/

ENDNAMESPACE

#endif

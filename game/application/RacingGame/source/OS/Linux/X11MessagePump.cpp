#if OS_LINUX

#include "OS/Linux/X11Window.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>

BEGINNAMESPACE

extern ::Display* sX11Display;
extern X11::Window sX11FocusWindow;
extern ::XContext sX11Context;

bool processEvent(XEvent* event);
bool pumpMessages() {
    if(sX11Display == nullptr) return true; //nothing to update

    int32 count = XPending(sX11Display);
    while (count--)
    {
        XEvent event;
        XNextEvent(sX11Display, &event);
        processEvent(&event);
    }
    return true;
}

X11Window* findWindowByHandle(X11::Window handle)
{
    union {
        X11Window* window;
        XPointer ptr;
    };

    int32 ctx = sX11Context;
    Display* dsp = sX11Display;

    if (XFindContext(sX11Display, handle, sX11Context, &ptr) != 0)
    {
        return NULL;
    }

    return window;
}

bool processEvent(XEvent* event) {
    X11Window* window = NULL;

   if (event->type != GenericEvent)
   {
       window = findWindowByHandle(event->xany.window);
       if (window == NULL)
       {
           // This is an event for a window that has already been destroyed
           return true;
       }
   }

   switch (event->type)
   {
       case EnterNotify:
       {
       }

       case LeaveNotify:
       {
       }

       case MotionNotify:
       {
       }

       case ConfigureNotify:
       {
           break;
       }

       case ClientMessage:
       {

           break;
       }

       case MapNotify:
       {
           break;
       }

       case UnmapNotify:
       {
           break;
       }

       case FocusIn:
       {
           sX11FocusWindow = event->xany.window;

           break;
       }

       case FocusOut:
       {
           if(sX11FocusWindow == event->xany.window)
               sX11FocusWindow = 0;
           break;
       }

       case Expose:
       {
           break;
       }

       case PropertyNotify:
       {
           break;
       }

       case SelectionClear:
       {
           break;
       }

       case SelectionRequest:
       {
           break;
       }

       case DestroyNotify:
           return true;

       case GenericEvent:
       {
       }

       default:
       {
       /*
           switch (event->type)
           {
               case RRScreenChangeNotify:
               {
                   XRRUpdateConfiguration(event);
                   break;
               }
           }
        */
           break;
       }
   }
   return true;
}

ENDNAMESPACE
#endif

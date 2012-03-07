#define _GNU_SOURCE

#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xcursor/Xcursor.h>
#include <X11/cursorfont.h>
#include <X11/Xlibint.h>
#include <X11/XKBlib.h>
#include <xcb/xcb.h>

#include "layout.h"


static int (*realXCloseDisplay)(Display *);
static Cursor (*realXCreateFontCursor)(Display *, unsigned int);
static int (*realXNextEvent)(Display *, XEvent *);
static int (*realXDefineCursor)(Display *dpy, Window win, Cursor cur);

static Cursor s_std_xterm_cursor;


int XCloseDisplay(Display *dpy)
{
	destroy_layouts();

	if (!realXCloseDisplay)
		realXCloseDisplay = dlsym(RTLD_NEXT, "XCloseDisplay");

	return realXCloseDisplay(dpy);
}


/*
 Here we need to remember XID of xterm cursors to change them to our cursors.
 */
Cursor XCreateFontCursor(Display *dpy, unsigned int shape)
{
	if (shape == XC_xterm) {
		static int is_first_start = True;
		if (is_first_start) {
			init_layouts(dpy);
			is_first_start = False;
		}
	}

	if (!realXCreateFontCursor)
		realXCreateFontCursor = dlsym(RTLD_NEXT, "XCreateFontCursor");

	return realXCreateFontCursor(dpy, shape);
}


/*
 Here we change the xterm cursors to our cursors.
 */
int XDefineCursor(Display *dpy, Window win, Cursor cur)
{
	if (!realXDefineCursor)
		realXDefineCursor = dlsym(RTLD_NEXT, "XDefineCursor");

	return realXDefineCursor(dpy, win, cur);
}


int XNextEvent(Display *dpy, XEvent *event)
{
	int eventType = event->type;

	if (!realXNextEvent)
		realXNextEvent = dlsym(RTLD_NEXT, "XNextEvent");

	int result = realXNextEvent(dpy, event);

	if (eventType == GenericEvent) {
		XkbEvent *ev = (XkbEvent *) event;
		if (ev->any.xkb_type == XkbStateNotify) {
			printf("The layout was changed\n");
			//XDefineCursor(dpy, DefaultRootWindow(dpy), xtermCursor);
			//XSync(dpy, False);
		}
	}

	return result;
}


#if 0
void ShowEvent(int type)
{
	const char *strType;
	switch (type) {
	case KeyPress:
		strType = "KeyPress";
		break;
	case KeyRelease:
		strType = "KeyRelease";
		break;
	case ButtonPress:
		strType = "ButtonPress";
		break;
	case ButtonRelease:
		strType = "ButtonRelease";
		break;
	case MotionNotify:
		strType = "MotionNotify";
		break;
	case EnterNotify:
		strType = "EnterNotify";
		break;
	case LeaveNotify:
		strType = "LeaveNotify";
		break;
	case FocusIn:
		strType = "FocusIn";
		break;
	case FocusOut:
		strType = "FocusOut";
		break;
	case KeymapNotify:
		strType = "KeymapNotify";
		break;
	case GraphicsExpose:
		strType = "GraphicsExpose";
		break;
	case NoExpose:
		strType = "NoExpose";
		break;
	case CirculateRequest:
		strType = "CirculateRequest";
		break;
	case ConfigureRequest:
		strType = "ConfigureRequest";
		break;
	case MapRequest:
		strType = "MapRequest";
		break;
	case ResizeRequest:
		strType = "ResizeRequest";
		break;
	case CirculateNotify:
		strType = "CirculateNotify";
		break;
	case ConfigureNotify:
		strType = "ConfigureNotify";
		break;
	case CreateNotify:
		strType = "CreateNotify";
		break;
	case DestroyNotify:
		strType = "DestroyNotify";
		break;
	case GravityNotify:
		strType = "GravityNotify";
		break;
	case MapNotify:
		strType = "MapNotify";
		break;
	case MappingNotify:
		strType = "MappingNotify";
		break;
	case ReparentNotify:
		strType = "ReparentNotify";
		break;
	case UnmapNotify:
		strType = "UnmapNotify";
		break;
	case VisibilityNotify:
		strType = "VisibilityNotify";
		break;
	case ColormapNotify:
		strType = "ColormapNotify";
		break;
	case ClientMessage:
		strType = "ClientMessage";
		break;
	case PropertyNotify:
		strType = "PropertyNotify";
		break;
	case SelectionClear:
		strType = "SelectionClear";
		break;
	case SelectionNotify:
		strType = "SelectionNotify";
		break;
	case SelectionRequest:
		strType = "SelectionRequest";
		break;
	default:
		strType = "? UnknownEvent ?";
	}
	printf("Event type: %d - %s\n", type, strType);
}
#endif


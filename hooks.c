#define _GNU_SOURCE

#include <dlfcn.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <X11/Xlibint.h>

static int isXTermCursor = 0;


static Cursor (*realXCreateFontCursor)(Display *, unsigned int);


Cursor XCreateFontCursor(Display *dpy, unsigned int shape)
{
	printf("Replace: %s, shape: %d\n", __func__, shape);

	Cursor cursor;

	if (shape == XC_xterm) {
		return XcursorFilenameLoadCursor(dpy, "/home/quendi/default"); 
	}
	else {
		if (!realXCreateFontCursor)
			realXCreateFontCursor = dlsym(RTLD_NEXT, "XCreateFontCursor");
		cursor = realXCreateFontCursor(dpy, shape);
	}

	return cursor;
}




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


static int (*realXNextEvent)(Display *, XEvent *);


int XNextEvent(Display *dpy, XEvent *event)
{
//	ShowEvent(event->type);
	if (event->type > 36) {
//		printf("Unknown Event\n");
	}

	if (!realXNextEvent)
		realXNextEvent = dlsym(RTLD_NEXT, "XNextEvent");

	return realXNextEvent(dpy, event);
}






static int (*realXDefineCursor)(Display *dpy, Window win, Cursor cur);


int XDefineCursor(Display *dpy, Window win, Cursor cur)
{
	printf("New cursor XID: %ld\n", cur);
	if (!realXDefineCursor)
		realXDefineCursor = dlsym(RTLD_NEXT, "XDefineCursor");

	return realXDefineCursor(dpy, win, cur);
}


#define _GNU_SOURCE

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/XKBlib.h>
#include <X11/Xcursor/Xcursor.h>

#include "layout.h"


static layout_t s_layouts[XkbNumKbdGroups];
static int s_layout_count;


Cursor load_cursor_from_file(Display *dpy, const char *layout_name)
{
	char cursor_name[50];
	sprintf(cursor_name, "%s.xcursor", layout_name);

	char *ch;
	while (ch = strchr(cursor_name, ' '))
		*ch = '_';

	char file_name[300];
	sprintf(file_name, "%s/.cache/libXtextcursorchanger/%s", getenv("HOME"), cursor_name);
	printf("%s\n", file_name);	

	return XcursorFilenameLoadCursor(dpy, file_name);
}


void init_layouts(Display *dpy)
{
	XkbDescRec desc;
	memset(&desc, 0, sizeof desc);
	desc.device_spec = XkbUseCoreKbd;
	XkbGetControls(dpy, XkbGroupsWrapMask, &desc);
	XkbGetNames(dpy, XkbGroupNamesMask, &desc);

	s_layout_count = desc.ctrls->num_groups;

	char *names[XkbNumKbdGroups];
	XGetAtomNames(dpy, desc.names->groups, s_layout_count, names);
	XkbFreeControls(&desc, XkbGroupsWrapMask, True);
	XkbFreeNames(&desc, XkbGroupNamesMask, True);

	for (int i = 0; i < s_layout_count; i++) {
		s_layouts[i].name = names[i];
		s_layouts[i].name_size = strlen(names[i]);
		s_layouts[i].cursor = load_cursor_from_file(dpy, names[i]);
	}
}


void destroy_layouts()
{
	for (; s_layout_count; s_layout_count--) {
		if (s_layouts[s_layout_count - 1].name) {
			XFree(s_layouts[s_layout_count - 1].name);
			s_layouts[s_layout_count - 1].name = NULL;
		}
	}
}


inline int layout_count()
{
	return s_layout_count;
}


int current_layout_index(Display *dpy)
{
	XkbStateRec state;
	memset(&state, 0, sizeof state);
	XkbGetState(dpy, XkbUseCoreKbd, &state);

	return state.group;
}


const layout_t *current_layout(Display *dpy)
{
	return layout(current_layout_index(dpy));
}


inline const layout_t *layout(int index)
{
	return &s_layouts[index];
}


#ifndef __LAYOUT_H__
#define __LAYOUT_H__

#include <X11/Xlib.h>


typedef struct _layout_t {
	char *name;
	size_t name_size;
	Cursor cursor;
}
layout_t;


void init_layouts(Display *d);
void destroy_layouts();

int layout_count();
int current_layout_index(Display *d);
const layout_t *current_layout(Display *d);
const layout_t *layout(int index);


#endif //_LAYOUT_H__


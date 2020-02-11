#include <stdlib.h>
#include <string.h>

#include "dat.h"
#include "fncs.h"

void
sappend(Slice *s, void *data)
{
	if(s->size+1 >= s->cap){
		s->cap = s->cap == 0 ? 64 : s->cap * 2;
		s->data = realloc(s->data, s->cap * s->width);
	}
	/* We don't know if this data may overlap */
	memmove(s->data+(s->size*s->width), data, s->width);
	s->size++;
}

int
sremove(Slice *s, int index)
{
	int left, right, tail;

	if(index >= s->size)
		return -1;
	if(index == s->size-1)
		goto done;

	left = index;
	right = left+1;
	tail = s->size - right;
	memmove(s->data+left*s->width, s->data+right*s->width, tail*s->width);

done:
	s->size--;
	return 0;
}

void*
sgrab(Slice *s, int index)
{
	return s->data+index*s->width;
}
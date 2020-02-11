#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "dat.h"
#include "fncs.h"

static unsigned long
string2hash(char *s)
{
	int i;
	unsigned long hash;
	hash = 7;
	for(;*s;s++)
		hash = hash*31 + *s;
	return hash;
}

Hmap*
allocmap(int size)
{
	Hmap *h = calloc(1, sizeof(Hmap));
	h->size = size;
	h->nodes = calloc(size, sizeof(Hnode));
	return h;
}

void
mapinsert(Hmap *h, char *key, void *val)
{
	Hnode *n;

	n = h->nodes+(string2hash(key)%h->size);
	assert(n != nil);
	for(;;){
		if(n->key == nil)
			goto empty;
		if(strcmp(key, n->key) == 0)
			goto found;
		if(n->next == nil)
			break;
		n = n->next;
	}

	/* create new node */
	n->next = calloc(1, sizeof(Hnode));
	n = n->next;

empty:
	n->key = strdup(key);

found:
	n->val = val;
}

void*
mapget(Hmap *h, char *key)
{
	Hnode *n;

	n = h->nodes+(string2hash(key)%h->size);
	for(;n!=nil;n=n->next){
		if(n->key == nil)
			continue;
		if(strcmp(key, n->key) == 0){
			return n->val;
		}
	}

	return nil;
}

int
mapdel(Hmap *h, char *key)
{
	Hnode *n;
	n = h->nodes+(string2hash(key)%h->size);
	for(;n!=nil;n=n->next){
		if(n->key == nil)
			continue;
		if(strcmp(key, n->key) == 0){
			free(n->key);
			n->key = nil;
			return 1;
		}
	}

	return 0;
}

int
mapdump(Hmap *h, void **buf, int size)
{
	Hnode *n;
	int i, c;

	for(i=c=0;i<h->size;i++)
		for(n=h->nodes+i;n!=nil && n->key!=nil;n=n->next){
			if(c >= size)
				goto done;
			buf[c++] = n->val;
		}
done:
	return c;
}

int
mapdumpkey(Hmap *h, char **buf, int size)
{
	Hnode *n;
	int i, c;

	for(i=c=0;i<h->size;i++)
		for(n=h->nodes+i;n!=nil && n->key!=nil;n=n->next){
			if(c >= size)
				goto done;
			buf[c++] = n->key;
		}
done:
	return c;
}

void
mapclear(Hmap *h)
{
	Hnode *n;
	int i;

	for(i=0;i<h->size;i++)
		for(n=h->nodes+i;n!=nil;n=n->next)
			if(n->key != nil){
				free(n->key);
				n->key=nil;
			}
}

void
freemap(Hmap *h)
{
	mapclear(h);
	free(h->nodes);
	free(h);
}

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "dat.h"
#include "fncs.h"

/* map[string]int */
static Hmap* map = nil;

Slice sobj = { .width = sizeof(Object) };
Slice spair = { .width = sizeof(Pair) };

/* Store object for Get */
static Object *out = nil;

void
debugObj(void)
{
	int i;
	Object *o;

	o = sobj.data;
	for(i=0;i<sobj.size;i++){
		fprintf(stderr, "%s ", o[i].name);
	}
	fprintf(stderr, "\n");
}

void
debugPair(void)
{
	int i, j;
	Object *o;

	o = sobj.data;
	for(j=0;j<sobj.size;j++)
	for(i=0;i<o[j].npair;i++){
		fprintf(stderr, "%s ",o[j].pairs[i].key);
	}
	fprintf(stderr, "\n");
}

void
regPair(char *key, void *val, unsigned int type)
{
	Pair p;
	p.key = strdup(key);
	p.type = type;
	switch(type){
	case STR:
		p.svalue = strdup(val);
		break;
	case NUM:
		p.ivalue = *(int*)val;
		break;
	}
	sappend(&spair, &p);
}

void
copyPair(Pair *dst, Pair *src, int numPair)
{
	memcpy(dst, src, sizeof(Pair)*numPair);
}

void
updateObj(char *name)
{
	Object *o;
	Pair *tmpPair;
	int i, j;
	int oi;

	if(map == nil)
		return;
	if((oi = (int)mapget(map, name)) == 0)
		goto done;

	o = sgrab(&sobj, oi-1);

	tmpPair = spair.data;

	/* this assert catches out of sync between the map and slice */
	assert(strcmp(name, o->name) == 0);

	for(i=0;i<spair.size;i++)
	for(j=0;j<o->npair;j++)
	if(strcmp(tmpPair[i].key, o->pairs[j].key) == 0){
		/* Type may of changed so we clean up first */
		if(o->pairs[j].type == STR)
			free(o->pairs[j].svalue);
		o->pairs[j].type = tmpPair[i].type;
		switch(tmpPair[i].type){
		case STR:
			o->pairs[j].svalue = tmpPair[i].svalue;
			break;
		case NUM:
			o->pairs[j].ivalue = tmpPair[i].ivalue;
			break;
		case OBJ:
			o->pairs[j].ovalue = tmpPair[i].ovalue;
			break;
		default:
			assert("Pair type corruption" == 0); //panic; obvious sign of memory corruption
		}
	}

done:
	spair.size = 0;
}

void
regObj(char *name)
{
	Object *o;
	Object new;
	int oi;

	if(map == nil){
		map = allocmap(MAP);
	}
	new.npair = spair.size;
	new.pairs = malloc(sizeof(Pair)*spair.size);
	copyPair(new.pairs, spair.data, spair.size);
	oi = (int)mapget(map, name);
	if(oi == 0){
		new.name = strdup(name);
		sappend(&sobj, &new);
		o = sgrab(&sobj, sobj.size-1);
		mapinsert(map, name, sobj.size);
	} else {
		o = sgrab(&sobj, (void*)(oi-1));
		free(o->pairs);
		memmove(o, &new, sizeof new);
	}
	spair.size = 0;
}

void
cacheObj(char *name)
{
	int oi;

	if(map == nil){
		out = nil;
		return;
	}
	oi = (int)mapget(map, name);
	if(oi == 0){
		out = nil;
		return;
	}
	out = sgrab(&sobj, oi-1);
}

Object*
uncacheObj(void)
{
	return out;
}

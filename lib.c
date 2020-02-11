#include <string.h>
#include <stdio.h>
#include <stdatomic.h>
#include <unistd.h>

#include "yak.tab.h"
#include "dat.h"
#include "fncs.h"

int readInputForLexer(char *buffer, int *read, int req);

static int readOff;
static char *text;

static int infd;

int (*reader)(char*, int*,int) = nil;

int
readinstr(char *buffer, int *read, int req)
{
	int toRead = req;
	int bytesLeft = strlen(text+readOff);
	int i;
	if(toRead > bytesLeft)
		toRead = bytesLeft;

	for(i=0; i<toRead; i++)
		buffer[i] = text[readOff+i];

	*read = toRead;
	readOff += toRead;
	return 0;
}

int
readinfd(char *buffer, int *hasread, int req)
{
	ssize_t n;
	char buf[8192];

	*hasread = 0;
	for(;req > 0;){
		switch((n = read(infd, buf, req < sizeof buf ? req : sizeof buf))){
		case 0:
			return 0;
		default:
			memcpy(buffer, buf, n);
			*hasread+=n;
			req-=n;
			buffer+=n;
			break;
		}
	}
	return 0;	
}

/*
  * crude locking; We're not multithreaded but the caller might be.
  */
atomic_int lock = 0;

int
dbput(char *s)
{
	if(lock)
		return 1;
	lock++;
	reader = readinstr;
	readOff = 0;
	text = s;
	yyparse();
	lock--;
	return 0;
}

Object* 
dbget(char *s)
{
	Object *o;

	if(s == nil)
		return uncacheObj();

	if(lock)
		return nil;

	lock++;
	reader = readinstr;
	readOff = 0;
	text = s;
	yyparse();
	o = uncacheObj();
	lock--;
	return o;
}

int
fdbput(int fd)
{
	if(lock)
		return 1;
	lock++;
	reader = readinfd;
	infd = fd;
	yyparse();
	lock--;
	return 0;
}

Object*
dbdump(int *n)
{
	Object *o;
	extern Slice sobj;
	if(lock)
		return nil;
	lock++;
	*n = sobj.size;
	o  = sobj.data;
	lock--;
	return o;
}

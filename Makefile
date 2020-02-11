O=o
TARG=psan
CFLAGS=-c -Wall -Wno-missing-braces -fpic

OFILES=\
	yak.tab.$O\
	lex.yy.$O\
	lib.$O\
	lang.$O\
	hash.$O\
	slice.$O\

LFILE=lift.l

YFILE=yak.y

all: dynamic static
dynamic:	$(OFILES)
	$(CC) -shared -o lib$(TARG).so $(OFILES)

static:		$(OFILES)
	ar rcs lib$(TARG).a $(OFILES)

%.$O:	%.c
	$(CC) $(CFLAGS) $*.c

lex.yy.$O:	$(LFILE)
	flex $(LFILE)
	$(CC) $(CFLAGS) $*.c

yak.tab.$O:	$(YFILE)
	bison -d $(YFILE)
	$(CC) $(CFLAGS) $*.c

install: dynamic
	cp libpsan.so /usr/lib64/
	cp psan.h /usr/include/

clean:
	rm -f *.$O yak.tab.* lex.yy.c lib$(TARG).so lib$(TARG).a


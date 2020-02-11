typedef struct Object Object;
typedef struct Pair Pair;

struct Pair {
	char *key;
	unsigned int type;
	union{
		char *svalue;
		int ivalue;
		Object *ovalue;
	};
};


struct Object {
	char *name;
	int npair;
	Pair *pairs;
};

typedef struct Slice Slice;
struct Slice{
	int cap; /* Capacity of slice in terms of width */
	int size; /* Number of elements in slice */
	int width; /* Size of elemnt in slice */
	void *data;
};

int		dbput(char*);
Object*	dbget(char*);
Object* 	dbdump(int*);
int		fdbput(int);

void		sappend(Slice*,void*);
int		sremove(Slice*,int);
void*	sgrab(Slice*, int);
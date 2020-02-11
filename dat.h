typedef struct Object Object;
typedef struct Pair Pair;

#define nil  (void*)0

enum{
	MAP = 64,
};

enum{
	STR,
	NUM,
	OBJ,
};

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


/*
 * Simple hashmap implementation.
 * Hnode key must be non nil.
 */
typedef struct Hmap Hmap;
typedef struct Hnode Hnode;
struct Hmap{
	int size;
	int (*hashfn)(void*);
	struct Hnode{
		char *key;
		void *val;
		Hnode *next;
	} *nodes;
};

typedef struct Slice Slice;
struct Slice{
	int cap; /* Capacity of slice in terms of width */
	int size; /* Number of elements in slice */
	int width; /* Size of elemnt in slice */
	void *data;
};


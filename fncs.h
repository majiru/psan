void		regPair(char*,void*,unsigned int);
void		regObj(char*);
void		updateObj(char*);
void		rollPair(void);
void		cacheObj(char*);
Object*		uncacheObj(void);

/* map.c */
void mapclear(Hmap*);
int mapdump(Hmap*, void**,int);
int mapdel(Hmap*,char*);
void *mapget(Hmap*,char*);
void mapinsert(Hmap*,char*,void*);
Hmap *allocmap(int);

/* slice.c */
void		sappend(Slice*,void*);
int		sremove(Slice*,int);
void*	sgrab(Slice*, int);
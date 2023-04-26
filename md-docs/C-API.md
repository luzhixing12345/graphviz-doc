
# C-API

```c
/* graphs */
Agraph_t *agopen(char *name, Agdesc_t desc, Agdisc_t * disc);
int agclose(Agraph_t * g);
Agraph_t *agread(void *chan, Agdisc_t * disc);
Agraph_t *agmemread(const char *cp);
void agreadline(int);
void agsetfile(char *);
Agraph_t *agconcat(Agraph_t * g, void *chan, Agdisc_t * disc);
int agwrite(Agraph_t * g, void *chan);
int agisdirected(Agraph_t * g);
int agisundirected(Agraph_t * g);
int agisstrict(Agraph_t * g);
int agissimple(Agraph_t * g);

/* nodes */
Agnode_t *agnode(Agraph_t * g, char *name, int createflag);
Agnode_t *agidnode(Agraph_t * g, IDTYPE id, int createflag);
Agnode_t *agsubnode(Agraph_t * g, Agnode_t * n, int createflag);
Agnode_t *agfstnode(Agraph_t * g);
Agnode_t *agnxtnode(Agraph_t * g, Agnode_t * n);
Agnode_t *aglstnode(Agraph_t * g);
Agnode_t *agprvnode(Agraph_t * g, Agnode_t * n);

Agsubnode_t *agsubrep(Agraph_t * g, Agnode_t * n);
int agnodebefore(Agnode_t *u, Agnode_t *v); /* we have no shame */

/* edges */
Agedge_t *agedge(Agraph_t * g, Agnode_t * t, Agnode_t * h,
			char *name, int createflag);
Agedge_t *agidedge(Agraph_t * g, Agnode_t * t, Agnode_t * h,
              IDTYPE id, int createflag);
Agedge_t *agsubedge(Agraph_t * g, Agedge_t * e, int createflag);
Agedge_t *agfstin(Agraph_t * g, Agnode_t * n);
Agedge_t *agnxtin(Agraph_t * g, Agedge_t * e);
Agedge_t *agfstout(Agraph_t * g, Agnode_t * n);
Agedge_t *agnxtout(Agraph_t * g, Agedge_t * e);
Agedge_t *agfstedge(Agraph_t * g, Agnode_t * n);
Agedge_t *agnxtedge(Agraph_t * g, Agedge_t * e, Agnode_t * n);

/* generic */
Agraph_t *agraphof(void* obj);
Agraph_t *agroot(void* obj);
int agcontains(Agraph_t *, void *);
char *agnameof(void *);
int agrelabel(void *obj, char *name);	/* scary */
int agrelabel_node(Agnode_t * n, char *newname);
int agdelete(Agraph_t * g, void *obj);
long agdelsubg(Agraph_t * g, Agraph_t * sub);	/* could be agclose */
int agdelnode(Agraph_t * g, Agnode_t * arg_n);
int agdeledge(Agraph_t * g, Agedge_t * arg_e);
int agobjkind(void *);

/* strings */
char *agstrdup(Agraph_t *, char *);
char *agstrdup_html(Agraph_t *, char *);
int aghtmlstr(char *);
char *agstrbind(Agraph_t * g, char *);
int agstrfree(Agraph_t *, char *);
char *agcanon(char *, int);
char *agstrcanon(char *, char *);
char *agcanonStr(char *str);  /* manages its own buf */
```

```c
char *agnameof(void *); // 获取 Agraph_t *g 的名字
```

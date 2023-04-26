
> 翻译自: https://graphviz.org/pdf/cgraph.pdf

> 有关 graphviz 的C库API 的使用 C-API 部分

# Cgraph-pdf

## 前言

示例代码在 example/ 下, 在本文的对应章节均有具体代码示例, 在您按照前文配置好环境之后可直接编译运行

```bash
cd example
make
./example1
```

除此之外 example/ 下还有一个 `ast.dot`, 这是一个辅助用的dot文件示例, 您可以使用 vscode 查看或运行 `./example0` 查看

![20230426141650](https://raw.githubusercontent.com/learner-lu/picbed/master/20230426141650.png)

所有的函数都是 `ag` 开头, 变量类型是 `Ag` 开头, 指针多.

## 介绍

Cgraph是一个用于图形编程的C库。它定义了由带属性的节点、边和子图组成的图形所需的数据类型和操作。属性可以是方便的文件I/O的字符串名称-值对，也可以是内部的C数据结构，用于高效的算法实现。

**Cgraph旨在表示图形**；它不是像最短路径或网络流这样的更高级算法的库。我们将这些视为在Cgraph之上编写的更高级别的库。在Cgraph的设计中，努力追求时间和空间效率。基本（未带属性）图形表示每个节点需要104个字节，每个边需要64个字节，因此存储具有数百万对象的图形是合理的。对于带属性的图形，Cgraph还维护一个内部共享字符串池，因此如果图形的所有节点都具有 `color=red` ，则只会创建“color”和“red”的一个副本。还有其他专家可以利用的技巧以获得平坦的编码效率。例如，有方法来内联边列表遍历和内部数据结构访问的指令。

Cgraph使用Phong Vo的字典库 `libcdt` 来存储节点和边集。该库提供了一个统一的接口，可用于哈希表和伸展树，其API可用于一般编程（如在Cgraph程序中存储多重集合、哈希表、列表和队列）。

## Graph Objects

几乎所有的Cgraph编程都可以通过指向这些数据类型的指针来完成:

- `Agraph_t`: 一个图形或子图
- `Agnode_t`: 特定图形或子图中的**节点**
- `Agedge_t`: 特定图形或子图中的**边**
- `Agsym_t`: 字符串-值对属性的**描述符**
- `Agrec_t`: 图形对象的内部C数据记录属性

Cgraph负责自己的内存管理；Cgraph数据结构的分配和释放始终通过Cgraph调用完成。

## Graphs

顶层图形（也称为根图）定义了节点、边、子图、数据字典和其他信息。图形具有名称和两个属性：它是有向的还是无向的，以及它是否是严格的（禁止多重边）。可以指定一个图形是简单的（既没有多重边也没有环），或者可以具有多重边但不能具有环。

**请注意，节点、边和子图存在于一个根图中。它们不能独立于该图形使用，也不能附加到另一个根图中。**

以下示例使用约定，即 `G和g` 为 `Agraph_t*`（图指针），`n、u、v、w` 为 `Agnode_t*`（节点指针），`e、f` 为 `Agedge_t*`（边指针）。

要创建一个新的空顶级有向图：

```c
Agraph_t *g;
g = agopen("G", Agdirected, NULL);
```

```c
#include <graphviz/cgraph.h>

Agraph_t *agopen(char *name, Agdesc_t desc, Agdisc_t * disc);
```

agopen的第一个参数可以是任何字符串，Cgraph不会对其进行解释，在将图形写入文件时记录和保留。

第二个参数表示图形的类型，应为如下四个变量之一

- Agdirected: 有向图
- Agstrictdirected: 严格有向图(禁止多重边)
- Agundirected: 无向图
- Agstrictundirected: 严格无向图(禁止多重边)

第三个参数是一个可选的指向一组方法的指针，用于覆盖Cgraph的某些默认行为，在大多数情况下可以为空指针NULL

您可以通过 `agnameof(g)` 获得图形的名称，并通过函数 `agisdirected(g)` 和 `agisstrict(g)` 获得其属性。

> 见 example1.c

您还可以通过读取文件来构建一个新的图形, 图形的名称、类型和包括属性在内的内容取决于文件内容。（第二个参数与上面提到的agopen的可选方法指针相同, 传 NULL 即可）：

> 见 example2.c

```c
Agraph_t *agread(void *chan, Agdisc_t * disc);

g = agread(stdin,NULL); // 从标准输入中读

FILE *fp = fopen("ast.dot","r");
g = agread(fp, NULL); // 利用 FILE *fp 读文件
```

有时将图形表示为字符字符串str是很方便的。在这种情况下，可以使用以下方式创建图形：

```c
Agraph_t *agmemread(const char *cp);

Agraph_t *g;
char *str = "digraph { A -> B; }";
g = agmemread(str);
```

在上述代码中，我们首先定义了一个字符指针str，其中保存了一个表示有向图的字符串。然后，我们使用agmemread()函数将该字符串转换为一个Agraph_t类型的图形对象，并将其赋值给指针变量g

您可以将图形的表示写入文件：

> 见 example3.c

```c
int agwrite(Agraph_t * g, void *chan);

agwrite(g,stdout); // 直接在 标准输出 中输出

FILE *fp = fopen("x.dot","w");
agwrite(g, fp); // 利用 FILE *fp 写文件
```

agwrite()函数创建了一个图形内容和属性的外部表示（除了内部属性），稍后可以通过在同一个文件上调用agread来重构它。

> 事实上如果需要在内部属性和外部字符串之间进行转换, 采用这种读写文件的方式要方便很多

下面的三个函数可以获取数量

> 见 example4.c

```c
int agnnodes(Agraph_t * g); // 节点数量
int agnedges(Agraph_t * g); // 边数量
int agnsubg(Agraph_t * g); // 子图数量
```

**注意最后需要使用** `agclose(g)` **删除图形及其关联的数据结构并释放它们的内存**

还有一个有趣但不太常用的函数可以将图形文件的内容连接到现有的图形上, agconcat()函数将标准输入中的图形内容附加到现有图形g的末尾，并返回连接后的图形对象

```c
g = agconcat(g,stdin,NULL);
```

## Nodes

在Cgraph中，节点通常由**唯一的字符串名称和由Cgraph分配的唯一的内部整数ID**来标识。（为方便起见，您还可以通过将节点名称设置为NULL来创建“匿名”节点。）即使在无向图中，节点也具有入边和出边集合。

一旦您拥有一个图形g，可以按以下方式创建或查找节点：

> 见 example5.c

```c
Agnode_t *agnode(Agraph_t * g, char *name, int createflag);

Agnode_t *n;
n = agnode(g,"node28",1);
n = agnode(g,"node11",0);
```

第一个参数是要在其中创建节点的图形或子图。第二个参数是节点的名称, 如果为 NULL 则认为是一个**匿名节点**

- 当第三个参数为 1 时，有则返回节点, 如果节点不存在，则创建该节点。
- 当第三个参数为 0 时，Cgraph搜索以查找具有给定名称的现有节点，如果找不到则返回NULL

函数agdegree可以给出节点在（子）图g中的度数，其中in和out选择边集。

```c
int agdegree(Agraph_t * g, Agnode_t * n, int in, int out);
int agcountuniqedges(Agraph_t * g, Agnode_t * n, int in, int out);
```

- agdegree(g,n,TRUE,FALSE)返回入度。
- agdegree(g,n,FALSE,TRUE)返回出度。
- agdegree(g,n,TRUE,TRUE)返回它们的和。

函数agcountuniqedges与agdegree相同，除非最后两个参数都为TRUE。在这种情况下，一个环路只计数一次。

agnameof(n)返回节点的可打印字符串名称, 对于匿名边/节点返回 NULL

```c
char *agnameof(void *);
```

请注意，由于各种原因，**该字符串可能是一个临时缓冲区，可能会被后续调用覆盖**。因此，以下用法是**不安全的**：

```c
printf("%s %s\n",agnameof(agtail(e)),agnameof(aghead(e)));
```

因为当计算printf的参数时，缓冲区可能会被覆盖。需要分开写

可以使用agdelnode(g,n)从图或子图中删除节点。

> 见 example5.c

## Edges

**当创建一条边时，第一个节点将被用作尾节点(tail)，第二个节点将被用作头节点(head)。这适用于有向图和无向图**

> 这里的头尾稍微需要记忆一下, a -> b 中 a 是尾(箭头尾部), b 是头(箭头头部)

边是节点对：在有向图中是有序对，在无向图中是无序对。

为了方便起见，有一个通用的边数据结构 `Agedge_t` 用于两种类型，其端点是tail和head字段。

```c
Agedge_t *agedge(Agraph_t * g, Agnode_t * t, Agnode_t * h, char *name, int createflag);

Agedge_t *e;
Agnode_t *n1, *n2;
n1 = agnode(g, "node1", TRUE);
n2 = agnode(g, "node2", TRUE);
e = agedge(g, n1, n2, "e28", TRUE);
```

在上面的代码中，我们首先使用agnode()函数创建了两个节点n1和n2，然后使用agedge()函数创建了它们之间的一条边e。agedge()函数的第一个参数是创建边的图或子图，第二个和第三个参数是边的起点和终点，第四个参数是边的标签（可以为NULL），第五个参数表示如果边不存在，则创建它(False则查询, 同 agnode)。

n1和n2必须属于同一个图或子图。边的“名称”（更正确地说是标识符）被视为特定节点对之间边的唯一标识符。也就是说，**在任何给定的n1和n2之间，最多只能有一条名为e28的边，但是可能会有许多其他连接其他节点的边名为e28**

如果使用 agedge 查找一个 NULL 的边, 如果 n1 n2 之间不存在边则返回 NULL, 如果存在则返回最后被创建的边.(见example7.c)

> 虽然说可以重名, 但是从编写程序的角度来说用实现点,边的唯一 UID 应该也不算困难

可以使用agtail(e)和aghead(e)返回e的端点

> 见 example6.c

由于边被实现为边对，因此有两个指向同一边的有效指针，因此简单的指针比较 (e1 == e2) 对于边的相等性是不起作用的, 可以使用宏 `ageqedge` 来做判断

```c
// 是一个宏, 但可以类似看作如下函数定义
int ageqedge(Agedge_t *e0, Agedge_t *e1);
```

对于 node 和 edge 有如下的一些函数可以使用, 根据函数名不难猜测用法

> 见 example7.c

```c
Agedge_t *agfstin(Agraph_t * g, Agnode_t * n);
Agedge_t *agnxtin(Agraph_t * g, Agedge_t * e);
Agedge_t *agfstout(Agraph_t * g, Agnode_t * n);
Agedge_t *agnxtout(Agraph_t * g, Agedge_t * e);
Agedge_t *agfstedge(Agraph_t * g, Agnode_t * n);
Agedge_t *agnxtedge(Agraph_t * g, Agedge_t * e, Agnode_t * n);
```

e->node的值是与从中获取e的节点相对应的“其他”端点。也就是说，如果e是节点n的出边（n是e的tail），则e->node是e的head. 例如遍历节点 n 的所有出边

```c
// 遍历节点 n 的所有出边
for (e = agfstout(g,n); e; e = agnxtout(g,e)) {
    // do something
}

// 搜索u->v的边
e = agedge(g,u,v,NULL,FALSE); // 这里搜索的是匿名边, 有且当
e = agedge(g,u,v,"e8",FALSE);
```

删除边的方法与删除节点的方法类似，可以使用函数agdeledge(g, e)来删除边e，并释放其占用的内存

## Traversals

Cgraph提供了用于迭代图对象的函数。例如，我们可以通过以下方式扫描图（有向或无向）的所有边：

> 函数命名都应该算挺标准的了, 应该不难理解和推测, 见 example8.c

```c
Agnode_t *agfstnode(Agraph_t * g);
Agnode_t *agnxtnode(Agraph_t * g, Agnode_t * n);

for (n = agfstnode(g); n; n = agnxtnode(g,n)) {
    for (e = agfstout(g,n); e; e = agnxtout(g,e)) {
        /* do something with e */
    }
}
```

对于有向边，"in"和"out"的意义是显而易见的。对于无向图，Cgraph**根据创建边时两个节点的顺序分配方向(tail head)**

但是如果在遍历过程中代码删除了一个边或节点，则对象将不再有效，无法获取下一个对象

```c
for (e = agfstedge(g,n); e; e = agnxtedge(g,e,n)) {
    // delete node n/edge e -> error
}
```

通常可以通过替换为以下代码处理, 即提前取得下一个节点:

```c
for (e = agfstedge(g,n); e; e = f) {
    f = agnxtedge(g,e,n);
    // delete node n/edge e -> ok
}
```

遍历保证按照它们在根图中创建的顺序访问图的节点或节点的边

## External Attributes

图形对象可以具有相关的 `字符串名称-值` 对。当读取图形文件时，Cgraph的解析器会处理这些细节，因此属性可以在文件的任何位置添加。**在C程序中，必须在使用之前声明值。**

Cgraph假定给定类型（图/子图、节点或边）的**所有对象具有相同的属性**, **即属性内部没有子类型的概念**。属性信息存储在数据字典中。每个图形都有三个（用于图/子图、节点和边），在调用这些字典的创建、搜索和遍历时需要使用预定义的常量 `AGRAPH`, `AGNODE` 和 `AGEDGE`

因此，要为节点创建属性，可以使用：

> 见 example9.c

```c
Agsym_t *sym;
sym = agattr(g,AGNODE,"shape","box"); // 节点的形状变为正方形
```

如果成功，sym指向新创建（或更新）属性的描述符。(因此，即使shape先前已声明并具有其他默认值，它也将通过上述方式设置为box)
通过使用空指针作为值，可以使用相同的函数搜索图形的属性定义。

```c
sym = agattr(g,AGNODE,"shape",0);
if (sym) {
    printf("The default shape is %s.\n",sym->defval);
}
```

如果已经有了一个 graph object 的指针 n, 可以直接利用 agattrsym 去获取属性 

```c
Agnode_t* n;
Agsym_t* sym = agattrsym(n,"shape");
if (sym) {
    printf("The default shape is %s.\n",sym->defval);
}
```

> 如果未定义属性，则两个函数均返回NULL

可以使用 agnxtattr 去遍历所有属性：

```c
sym = 0; /* to get the first one */
while (sym = agnxtattr(g,AGNODE,sym)) {
    printf("%s = %s\n",sym->name,sym->defval);
}
```

假设某个对象已经存在属性，则可以使用其字符串名称或其Agsym_t描述符获取或设置其值。要使用字符串名称，我们有：

```c
Agnode_t* n;
char *str = agget(n,"shape");
agset(n,"shape","hexagon");
```

如果属性经常被引用，则使用其描述符作为索引会更快，如下所示：

```c
Agsym_t *sym = agattr(g,AGNODE,"shape","box");
char *str = agxget(n,sym);
agxset(n,sym,"hexagon");
```

Cgraph提供了两个辅助函数来处理属性。函数agsafeset首先检查属性是否已定义，如果没有定义，则使用默认值def定义它。然后将value用作分配给obj的特定值。

有时有用的是将一个对象的所有值复制到另一个对象中。这可以使用agcopyattr(void *src, void\* tgt)轻松完成。这假定源和目标是相同类型的图形对象，并且src的属性已经为tgt定义。如果src和tgt属于同一根图，则这将自动成立。

> 这里的 API 有点多, 不过大概意思都差不多, 读者可以参考 example9.c 的代码选择合适的方式使用. 关于都有什么属性以及属性都有什么值可选参考 `属性`

## Internal Attributes

只使用字符串属性完全是可能的。然而，通常来说这样做效率太低下了。为了解决这个问题，每个图形对象（图形、节点或边）都可以有一个相关的内部数据记录列表。每个记录的布局是由程序员定义的，除了每个记录都必须有一个 `Agrec_t` 头。这些记录是通过Cgraph分配的。例如：

```c
typedef struct mynode_s {
    Agrec_t h;
    int count;
} mynode_t;
mynode_t *data;
Agnode_t *n;
n = agnode(g, "mynodename", TRUE);
data = (mynode_t *)agbindrec(n, "mynode_t", sizeof(mynode_t), FALSE);
data->count = 1;
```

类似地，aggetrec会搜索记录，如果记录存在则返回指向该记录的指针，否则返回NULL；agdelrec从对象中删除记录。
虽然每个图形对象可能有自己独特的记录集合，但为了方便起见，有一些函数可以通过一次性为所有节点、边或子图分配或删除相同的记录来更新整个图形。这些函数包括：
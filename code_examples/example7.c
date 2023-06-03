

#include <graphviz/cgraph.h>
#include <graphviz/gvc.h>
#include <stdio.h>

int main(int argc, char **argv) {
    Agraph_t *g;
    FILE *fp = fopen("./ast.dot", "r");
    g = agread((FILE *)fp, NULL);

    Agnode_t *node1, *node2;
    Agedge_t *e;

    node1 = agnode(g, "node7", 1);  // "-"
    node2 = agnode(g, "node13", 1);  // "1"

    // e1 = agedge(g, node1, node2, "e1", 1);
    agedge(g, node1, node2, "e1", 1);
    agedge(g, node1, node2, "e2", 1);
    agedge(g, node1, node2, "e3", 1);

    e = agedge(g, node1, node2, NULL, 0);
    printf("find NULL edge name should the last created edge = %s\n",agnameof(e));

    for (e = agfstout(g, node1); e; e = agnxtout(g, e)) {
        printf("edge name = %s\n",agnameof(e));
    }

    GVC_t *gvc;
    gvc = gvContext();
    gvLayout(gvc, g, "dot");
    gvRenderFilename(gvc, g, "png", "e7.png");
    gvFreeLayout(gvc, g);
    agclose(g);
    gvFreeContext(gvc);
    return 0;
}

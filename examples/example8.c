

#include <graphviz/cgraph.h>
#include <graphviz/gvc.h>
#include <stdio.h>

int main(int argc, char **argv) {
    Agraph_t *g;
    FILE *fp = fopen("./ast.dot", "r");
    g = agread((FILE *)fp, NULL);

    Agnode_t *n;
    Agedge_t *e;

    for (n = agfstnode(g); n; n = agnxtnode(g, n)) {
        for (e = agfstout(g, n); e; e = agnxtout(g, e)) {
            char *tail_node_name = agnameof(n);
            char *head_node_name = agnameof(aghead(e));
            printf("edge [%s] -> [%s]\n", tail_node_name, head_node_name);
        }
    }

    GVC_t *gvc;
    gvc = gvContext();
    gvLayout(gvc, g, "dot");
    gvRenderFilename(gvc, g, "png", "e8.png");
    gvFreeLayout(gvc, g);
    agclose(g);
    gvFreeContext(gvc);
    return 0;
}



#include <graphviz/cgraph.h>
#include <graphviz/gvc.h>
#include <stdio.h>

int main(int argc, char **argv) {
    Agraph_t *g;
    FILE *fp = fopen("./ast.dot", "r");
    g = agread((FILE *)fp, NULL);

    Agnode_t *node1, *node2;
    Agnode_t *node_tail, *node_head;
    Agedge_t *e1, *e2;

    node1 = agnode(g, "node8", 1);  // "/"
    node2 = agnode(g, "node9", 1);  // "12"

    e1 = agedge(g, node1, node2, NULL, 0);

    if (e1) {
        node_tail = agtail(e1);
        node_head = aghead(e1);
        printf("find edge node1 -> node2\n");
        printf("node head name = %s\n", agnameof(node_head));
        printf("node tail name = %s\n", agnameof(node_tail));
    } else {
        printf("fail to find edge node1 -> node2\n");
        e1 = agedge(g, node1, node2, "e1", 1);
    }

    e2 = agedge(g, node2, node1, NULL, 0);

    if (e2) {
        node_tail = agtail(e2);
        node_head = aghead(e2);
        printf("find edge node2 -> node1\n");
        printf("node head name = %s\n", agnameof(node_head));
        printf("node tail name = %s\n", agnameof(node_tail));
    } else {
        printf("fail to find edge node2 -> node1\n");
        e2 = agedge(g, node2, node1, "e2", 1);
    }

    if (e1 && e2 &&ageqedge(e1, e2)) {
        printf("e1 e2 are the same edge\n");
    } else {
        printf("e1 e2 are different edges\n");
    }
    GVC_t *gvc;
    gvc = gvContext();
    gvLayout(gvc, g, "dot");
    gvRenderFilename(gvc, g, "png", "e6.png");
    gvFreeLayout(gvc, g);
    agclose(g);
    gvFreeContext(gvc);
    return 0;
}

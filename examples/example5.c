

#include <graphviz/cgraph.h>
#include <graphviz/gvc.h>
#include <stdio.h>

int main(int argc, char **argv) {
    Agraph_t *g;
    FILE *fp = fopen("./ast.dot", "r");
    g = agread((FILE *)fp, NULL);

    

    Agnode_t *node;

    // 应该找得到
    char *node_name = "node8";
    node = agnode(g, node_name, 0);
    if (node == NULL) {
        printf("could not find %s\n", node_name);
    } else {
        int in = agdegree(g, node, 1, 0);
        int out = agdegree(g, node, 0, 1);
        printf("find %s: in = %d | out = %d\n", agnameof(node), in, out);
        agdelnode(g, node);
    }

    // 应该找不到
    node_name = "node24";
    node = agnode(g, node_name, 0);
    if (node == NULL) {
        printf("could not find %s\n", node_name);
    } else {
        int in = agdegree(g, node, 1, 0);
        int out = agdegree(g, node, 0, 1);
        printf("find %s: in = %d | out = %d\n", agnameof(node), in, out);
    }
    GVC_t *gvc;
    gvc = gvContext();
    gvLayout(gvc, g, "dot");
    gvRenderFilename(gvc, g, "png", "e5.png");
    gvFreeLayout(gvc, g);
    agclose(g);
    gvFreeContext(gvc);
    return 0;
}

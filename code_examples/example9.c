

#include <graphviz/cgraph.h>
#include <graphviz/gvc.h>
#include <stdio.h>

int main(int argc, char **argv) {
    Agraph_t *g;
    FILE *fp = fopen("./ast.dot", "r");
    g = agread((FILE *)fp, NULL);

    Agsym_t *sym;
    sym = agattr(g, AGNODE, "shape", 0);
    if (sym) {
        printf("[agattr]: The global default shape is %s.\n", sym->defval);
    }

    char *node_name = "node1";
    Agnode_t *node = agnode(g, node_name, 0);
    if (node) {
        sym = agattrsym(node, "shape");
        if (sym) {
            printf("[agattrsym]: The default shape of %s is %s.\n", node_name, sym->defval);
        }
        char *font_name = agget(node, "fontname");
        printf("[agget]: font name is %s\n", font_name);
        agset(node, "shape", "hexagon");
        printf("[agset]: new shape for %s is hexagon\n", node_name);
    }

    printf("[search all attributes]\n");
    sym = NULL;
    while ((sym = agnxtattr(g, AGNODE, sym))) {
        printf("%s = %s\n", sym->name, sym->defval);
    }

    printf("reset node2 shape\n");
    node = agnode(g, "node2", 0);
    sym = agattr(g, AGNODE, "shape", "box");
    agxset(node, sym, NULL);

    printf("change node3 shape to box\n");
    node = agnode(g, "node3", 0);
    sym = agattr(g, AGNODE, "shape", NULL);
    agxset(node, sym, "box");

    printf("copy node3 shape to node11\n");
    Agnode_t *nodex = agnode(g, "node11", 0);
    agcopyattr(node, nodex);

    GVC_t *gvc;
    gvc = gvContext();
    gvLayout(gvc, g, "dot");
    gvRenderFilename(gvc, g, "png", "e9.png");
    gvFreeLayout(gvc, g);
    agclose(g);
    gvFreeContext(gvc);
    return 0;
}


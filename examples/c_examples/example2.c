

#include <graphviz/cgraph.h>
#include <graphviz/gvc.h>
#include <stdio.h>

int main(int argc, char **argv) {
    Agraph_t *g;
    FILE *fp = fopen("./ast.dot", "r");
    g = agread((FILE*)fp, NULL);
    printf("name = %s\n", agnameof(g));
    printf("direct: %s\n", agisdirected(g) ? "true" : "false");
    printf("strict: %s\n", agisstrict(g) ? "true" : "false");

    agclose(g);
    return 0;
}

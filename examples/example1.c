#include <graphviz/cgraph.h>
#include <graphviz/gvc.h>

int main(int argc, char **argv) {
    Agraph_t *g;
    g = agopen("G", Agdirected, NULL);

    printf("name = %s\n", agnameof(g));
    printf("direct: %s\n", agisdirected(g) ? "true" : "false");
    printf("strict: %s\n", agisstrict(g) ? "true" : "false");

    agclose(g);
    return 0;
}



#include <graphviz/cgraph.h>
#include <graphviz/gvc.h>
#include <stdio.h>

int main(int argc, char **argv) {
    Agraph_t *g;
    char *str = "digraph { A -> B; }";
    g = agmemread(str);
    FILE *fp = fopen("example3.dot", "w");
    agwrite(g, fp);  // 利用 FILE *fp 写文件

    agclose(g);
    return 0;
}

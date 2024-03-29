#include <graphviz/gvc.h>

char buf[1024];

void readFile(char *fname)
{
    FILE *fp = fopen(fname, "r");
    int p = 0;
    char ch;
    while ((ch = fgetc(fp)) != EOF)
    {
        buf[p++] = ch;
    }
    buf[p] = '\0';
}

int main(int argc, char **argv)
{
    GVC_t *gvc;
    Agraph_t *g;
    gvc = gvContext();
    readFile("ast.dot");
    g = agmemread(buf);
    gvLayout(gvc, g, "dot");
    gvRenderFilename(gvc, g, "png", "out.png");
    gvFreeLayout(gvc, g);
    agclose(g);
    return (gvFreeContext(gvc));
}

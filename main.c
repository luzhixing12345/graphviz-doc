/*
 *Copyright (c) 2023 All rights reserved
 *@description: 123
 *@author: Zhixing Lu
 *@date: 2023-01-15
 *@email: luzhixing12345@163.com
 *@Github: luzhixing12345
 */

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

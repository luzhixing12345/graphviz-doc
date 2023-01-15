
#include <stdio.h>
#include<stdlib.h>
#include<string.h>

char buf[1024];

char *readFile(char *fname)
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

int main() {
    
    readFile("ast.dot");
    printf("s = %s\n",buf);
    return 0;
}
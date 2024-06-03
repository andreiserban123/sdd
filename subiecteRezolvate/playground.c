#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    char *sir = malloc(100 * sizeof(char));
    strcpy(sir, "02/06/2003");
    char *token = strtok(sir, "/");
    int zi = atoi(token);
    token = strtok(NULL, "/");
    int luna = atoi(token);
    token = strtok(NULL, "/");
    int an = atoi(token);
    printf("Ziua: %d, luna: %d, anul: %d\n", zi, luna, an);
    free(sir);
    return 0;
}

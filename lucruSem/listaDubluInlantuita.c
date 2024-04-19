#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct produs
{
    int *cod;
    char *denumire;
    float pret;
    float cantitate;
} produs;

typedef struct nodls
{
    produs *inf;
    struct nodls *next, *prev;
} nodls;

nodls *inserare(nodls *cap, nodls **coada, produs *p)
{
    nodls *nou = malloc(sizeof(nodls));
    nou->inf = malloc(sizeof(produs));
    nou->inf->cod = malloc(sizeof(int));
    *(nou->inf->cod) = *(p->cod);
    nou->inf->denumire = malloc(strlen(p->denumire) + 1);
    strcpy(nou->inf->denumire, p->denumire);
    nou->inf->pret = p->pret;
    nou->inf->cantitate = p->cantitate;
    nou->prev = NULL;
    nou->next = NULL;
    if (cap == NULL)
    {
        cap = nou;
        *coada = nou;
    }
    else
    {
        (*coada)->next = nou;
        nou->prev = (*coada);
        *coada = nou;
    }
    return cap;
}

void traversare(nodls *cap)
{
    nodls *tmp = cap;
    while (tmp != NULL)
    {
        printf("\n Cod=%d, Denumire =%s, Pret=%5.2f, Cantitate=%5.2f",
               *(tmp->inf->cod), tmp->inf->denumire, tmp->inf->pret, tmp->inf->cantitate);
        tmp = tmp->next;
    }
    printf("\n");
}

void traversareInversa(nodls *coada)
{
    nodls *tmp = coada;
    while (tmp != NULL)
    {
        printf("\n Cod=%d, Denumire =%s, Pret=%5.2f, Cantitate=%5.2f",
               *(tmp->inf->cod), tmp->inf->denumire, tmp->inf->pret, tmp->inf->cantitate);
        tmp = tmp->prev;
    }
}

void dezalocare(nodls *cap)
{
    while (cap != NULL)
    {
        nodls *tmp = cap;
        cap = cap->next;
        free(tmp->inf->cod);
        free(tmp->inf->denumire);
        free(tmp->inf);

        free(tmp);
    }
}

int main()
{
    FILE *f = fopen("input.txt", "r");
    int n;
    fscanf(f, "%d", &n);
    nodls *cap = NULL, *coada = NULL;
    produs *p;
    char buffer[20];
    for (int i = 0; i < n; i++)
    {
        p = (produs *)malloc(sizeof(produs));
        p->cod = (int *)malloc(sizeof(int));
        fscanf(f, "%d", p->cod);
        fscanf(f, "%s", buffer);
        p->denumire = (char *)malloc((strlen(buffer) + 1) * sizeof(char));
        strcpy(p->denumire, buffer);
        fscanf(f, "%f", &p->pret);
        fscanf(f, "%f", &p->cantitate);
        cap = inserare(cap, &coada, p);
        free(p->denumire);
        free(p->cod);
        free(p);
    }
    traversare(cap);
    traversareInversa(coada);
    dezalocare(cap);
    return 0;
}
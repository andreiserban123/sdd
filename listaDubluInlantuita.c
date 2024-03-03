#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct Cofetarie
{
    char *denumire;
    int *preturi;
    int dim;
} Cofetarie;

typedef struct Nod
{
    struct Nod *prev;
    struct Nod *next;
    Cofetarie info; // te complici daca faci Cofetarie *info;
} Nod;

typedef struct ListaDubla
{
    Nod *primulNod;
    Nod *ultimulNod;
} ListaDubla;

Cofetarie initializareCofetarie(char *denumire, int *preturi, int dim)
{
    Cofetarie c;
    c.denumire = malloc(strlen(denumire) + 1);
    strcpy(c.denumire, denumire);
    c.dim = dim;
    c.preturi = malloc(sizeof(int) * dim);
    for (int i = 0; i < c.dim; i++)
    {
        c.preturi[i] = preturi[i];
    }
    return c;
}

void afisare(Cofetarie c)
{
    printf("Denumire: %s\n", c.denumire);
    printf("Preturi: ");
    for (int i = 0; i < c.dim; i++)
    {
        printf("%d ", c.preturi[i]);
    }
    printf("\n");
}

ListaDubla inserareInceput(ListaDubla lista, Cofetarie c)
{
    Nod *nou = malloc(sizeof(Nod));
    nou->prev = NULL;
    nou->next = lista.primulNod;
    nou->info = c;
    if (lista.primulNod != NULL)
    {
        lista.primulNod->prev = nou;
    }
    else
    {
        lista.ultimulNod = nou;
    }
    lista.primulNod = nou;
    return lista;
}

void afisareLista(ListaDubla lista)
{
    Nod *temp = lista.primulNod;
    while (temp != NULL)
    {
        afisare(temp->info);
        temp = temp->next;
    }
}

int main()
{
    ListaDubla lista;
    lista.primulNod = NULL;
    lista.ultimulNod = NULL;
    Cofetarie c, c1;
    int preturi[] = {1, 2, 3};
    c = initializareCofetarie("Cofetarie1", preturi, 3);
    lista = inserareInceput(lista, c);
    c1 = initializareCofetarie("Cofetarie2", NULL, 0);
    lista = inserareInceput(lista, c1);
    afisareLista(lista);
    return 0;
}
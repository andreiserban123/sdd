#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct Auto
{
    char *marca;
    int nrUsi;
} Auto;

typedef struct Nod
{
    Auto info;
    struct Nod *next;
} Nod;

Auto initializare(char *marca, int nrUsi)
{
    Auto automobil;
    automobil.marca = malloc(strlen(marca) + 1);
    strcpy(automobil.marca, marca);
    automobil.nrUsi = nrUsi;
    return automobil;
}

Nod *inserareInceput(Nod *cap, Auto info)
{
    Nod *nou = malloc(sizeof(Nod));
    nou->info = info;
    nou->next = cap;
    cap = nou;
    return cap;
}

void afisare(Auto a)
{
    printf("Marca: %s\n", a.marca);
    printf("Nr Usi: %d\n", a.nrUsi);
}

void afisareLista(Nod *cap)
{
    while (cap != NULL)
    {
        afisare(cap->info);
        cap = cap->next;
    }
}

void freeAuto(Auto a)
{
    free(a.marca);
}

void inserareNodLaFinal(Nod **cap, Auto a)
{
    Nod *aux = *cap;
    Nod *nou = (Nod *)malloc(sizeof(Nod));
    if (nou == NULL)
        return;
    nou->info = a;
    nou->next = NULL;
    if (*cap == NULL)
    {
        *cap = nou;
    }
    else
    {
        while (aux->next != NULL)
        {
            aux = aux->next;
        }
        // sunt poz pe ultimul nod
        aux->next = nou;
    }
}

void freeLista(Nod *cap)
{
    while (cap != NULL)
    {
        Nod *tmp = cap->next;
        freeAuto(cap->info);
        free(cap);
        cap = tmp;
    }
}

int main()
{
    Nod *cap = NULL;
    inserareNodLaFinal(&cap, initializare("masina1", 4));
    inserareNodLaFinal(&cap, initializare("masina2", 3));
    inserareNodLaFinal(&cap, initializare("masina3", 2));
    inserareNodLaFinal(&cap, initializare("masina4", 10));
    inserareNodLaFinal(&cap, initializare("masina5", 8));
    afisareLista(cap);
    freeLista(cap);
    return 0;
}

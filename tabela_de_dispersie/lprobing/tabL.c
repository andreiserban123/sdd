#define _CRT_SECURE_NO_WARNINGS
#include <malloc.h>
#include <string.h>
#include <stdio.h>

typedef struct masina
{
    int anFabricatie;
    char *marca;
    float pret;
} masina;

typedef struct hashT
{
    masina **vect;
    int nrElem;
} hashT;

int hash(hashT tab, char *cheie)
{
    return cheie[0] % tab.nrElem;
}

hashT inserareHash(hashT tab, masina m)
{
    int poz = hash(tab, m.marca);

    while (tab.vect[poz] != NULL)
    {
        poz = (poz + 1) % tab.nrElem;
    }
    masina *p = malloc(sizeof(masina));
    p->anFabricatie = m.anFabricatie;
    p->marca = malloc(strlen(m.marca) + 1);
    strcpy(p->marca, m.marca);
    p->pret = m.pret;
    tab.vect[poz] = p;
    return tab;
}

int main()
{
    FILE *f = fopen("fisier.txt", "r");
    hashT tab;
    tab.nrElem = 20;
    tab.vect = malloc(sizeof(masina *) * tab.nrElem);
    for (int i = 0; i < tab.nrElem; i++)
    {
        tab.vect[i] = NULL;
    }
    int nrMasini;
    masina m;
    char buffer[200];
    fscanf(f, "%d", &nrMasini);
    for (int i = 0; i < nrMasini; i++)
    {
        fscanf(f, "%d", &m.anFabricatie);
        fscanf(f, "%s", buffer);
        m.marca = malloc(strlen(buffer) + 1);
        strcpy(m.marca, buffer);
        fscanf(f, "%f", &m.pret);

        tab = inserareHash(tab, m);
        free(m.marca);
    }

    for (int i = 0; i < tab.nrElem; i++)
    {
        if (tab.vect[i] != NULL)
        {
            printf("\n POZITIA %d\n", i);
            printf("Masina: %d %s %f\n", tab.vect[i]->anFabricatie, tab.vect[i]->marca, tab.vect[i]->pret);
        }
    }

    for (int i = 0; i < tab.nrElem; i++)
    {
        if (tab.vect[i] != NULL)
        {
            free(tab.vect[i]->marca);
            free(tab.vect[i]);
        }
    }
    free(tab.vect);
    tab.vect = NULL;

    return 0;
}
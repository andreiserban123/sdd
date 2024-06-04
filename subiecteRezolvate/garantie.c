#define _CRT_SECURE_NO_WARNINGS
#include <malloc.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
    char *denumireProdus;
    unsigned int nrLuni;
    unsigned char esteElectrica;
    char *service;
} Garantie;

typedef struct
{
    struct nodLS *next;
    Garantie inf;
} nodLS;

typedef struct
{
    nodLS **vect;
    int nrEL;
} hashT;

int hash(char *key, int nrEl)
{
    return key[0] % nrEl;
}

nodLS *inserareLS(nodLS *cap, Garantie g)
{
    nodLS *nou = malloc(sizeof(nodLS));
    nou->next = NULL;
    nou->inf = g;
    if (cap == NULL)
    {
        return nou;
    }
    nodLS *tmp = cap;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = nou;
    return cap;
}

void insertHashT(hashT t, Garantie g)
{
    int poz = hash(g.service, t.nrEL);
    t.vect[poz] = inserareLS(t.vect[poz], g);
}

void modificaService(char *oldKey, char *newKey, hashT t)
{
    int poz = hash(oldKey, t.nrEL);
    nodLS *tmp, *prev = NULL;
    tmp = t.vect[poz];
    while (tmp)
    {
        if (strcmp(tmp->inf.service, oldKey) == 0)
        {
            if (prev == NULL)
            {
                t.vect[poz] = t.vect[poz]->next;
                Garantie g = tmp->inf;
                g.denumireProdus = malloc(strlen(tmp->inf.denumireProdus) + 1);
                strcpy(g.denumireProdus, tmp->inf.denumireProdus);
                g.service = malloc(strlen(newKey) + 1);
                strcpy(g.service, newKey);
                insertHashT(t, g);

                free(tmp->inf.denumireProdus);
                free(tmp->inf.service);
                free(tmp);
            }
            else
            {
                prev->next = tmp->next;

                Garantie g = tmp->inf;
                g.denumireProdus = malloc(strlen(tmp->inf.denumireProdus) + 1);
                strcpy(g.denumireProdus, tmp->inf.denumireProdus);
                g.service = malloc(strlen(newKey) + 1);
                strcpy(g.service, newKey);
                insertHashT(t, g);

                free(tmp->inf.denumireProdus);
                free(tmp->inf.service);
                free(tmp);
                tmp = prev->next;
            }
        }
        else
        {
            prev = tmp;
            tmp = tmp->next;
        }
    }
}

void afisareLS(nodLS *cap)
{
    while (cap)
    {
        printf("\n  Service:%s, denumire Produs:%s, nr luni:%d", cap->inf.service, cap->inf.denumireProdus, cap->inf.nrLuni);
        cap = cap->next;
    }
}

void afisareHASH(hashT t)
{
    for (int i = 0; i < t.nrEL; i++)
    {
        printf("\nPoz %d", i);
        if (t.vect[i])
        {
            afisareLS(t.vect[i]);
        }
    }
}

int nrReparatii(hashT t, char *key)
{
    int poz = hash(key, t.nrEL);
    nodLS *tmp = t.vect[poz];
    int nr = 0;
    while (tmp)
    {
        if (strcmp(tmp->inf.service, key) == 0)
        {
            nr++;
        }
        tmp = tmp->next;
    }
    return nr;
}

void extindeGarantia(hashT t, int nrLuni)
{
    for (int i = 0; i < t.nrEL; i++)
    {
        if (t.vect[i])
        {

            nodLS *tmp = t.vect[i];
            while (tmp)
            {
                if (tmp->inf.esteElectrica == 1)
                {
                    tmp->inf.nrLuni += nrLuni;
                }
                tmp = tmp->next;
            }
        }
    }
}

typedef struct
{
    struct nodLD *next, *prev;
    Garantie inf;
} nodLD;

typedef struct
{
    nodLD *prim, *ultim;
} LD;

void inserareLD(LD *l, Garantie g)
{
    nodLD *nou = malloc(sizeof(nodLD));
    nou->inf = g;
    nou->inf.denumireProdus = malloc(strlen(g.denumireProdus) + 1);
    strcpy(nou->inf.denumireProdus, g.denumireProdus);
    nou->inf.service = malloc(strlen(g.service) + 1);
    strcpy(nou->inf.service, g.service);
    nou->next = nou->prev = NULL;
    if (l->prim == NULL || l->ultim == NULL)
    {
        l->prim = l->ultim = nou;
    }
    else
    {
        l->ultim->next = nou;
        nou->prev = l->ultim;
        l->ultim = nou;
    }
}

LD salveazaLD(hashT t, int prag)
{
    LD l;
    l.prim = l.ultim = NULL;
    for (int i = 0; i < t.nrEL; i++)
    {

        if (t.vect[i])
        {
            nodLS *tmp = t.vect[i];
            while (tmp)
            {
                if (tmp->inf.nrLuni < prag)
                {
                    inserareLD(&l, tmp->inf);
                }
                tmp = tmp->next;
            }
        }
    }
    return l;
}

void dezalocareHash(hashT *t)
{
    for (int i = 0; i < t->nrEL; i++)
    {
        if (t->vect[i])
        {
            while (t->vect[i])
            {
                free(t->vect[i]->inf.denumireProdus);
                free(t->vect[i]->inf.service);
                t->vect[i] = t->vect[i]->next;
            }
        }
    }
    free(t->vect);
    t->vect = NULL;
    t->nrEL = 0;
}

int main()
{
    FILE *f = fopen("fisier.txt", "r");
    hashT t;
    t.nrEL = 11;
    t.vect = malloc(sizeof(nodLS *) * t.nrEL);
    for (int i = 0; i < t.nrEL; i++)
    {
        t.vect[i] = NULL;
    }
    Garantie g;
    char buffer[50];
    while (fscanf(f, " %[^\n]", buffer) == 1)
    {
        g.denumireProdus = malloc(strlen(buffer) + 1);
        strcpy(g.denumireProdus, buffer);
        fscanf(f, "%u", &g.nrLuni);
        fscanf(f, "%d", &g.esteElectrica);
        fscanf(f, " %[^\n]", buffer);
        g.service = malloc(strlen(buffer) + 1);
        strcpy(g.service, buffer);
        insertHashT(t, g);
    }
    afisareHASH(t);

    printf("\n\nNumar de garatii la service HomeRepair: % d", nrReparatii(t, "HomeRepair"));
    extindeGarantia(t, 6);

    modificaService("HomeRepair", "La dobre", t);
    afisareHASH(t);

    LD lista = salveazaLD(t, 15);

    printf("\nAfis lista:\n\n");

    while (lista.prim)
    {
        printf("\n  Service:%s, denumire Produs:%s, nr luni:%d", lista.prim->inf.service, lista.prim->inf.denumireProdus,
               lista.prim->inf.nrLuni);
        nodLD *tmp = lista.prim;
        lista.prim = lista.prim->next;
        free(tmp->inf.denumireProdus);
        free(tmp->inf.service);
        free(tmp);
    }
    printf("\nAfis hash:\n\n");

    afisareHASH(t);
    dezalocareHash(&t);
    afisareHASH(t);

    return 0;
}
#define _CRT_SECURE_NO_WARNINGS

#include <malloc.h>
#include <stdio.h>
#include <string.h>

typedef struct produs
{
    int cod;
    char *denumire;
    float pret;
} produs;

typedef struct nodLS
{
    produs inf;
    struct nodLS *next;
} nodLS;

typedef struct hashT
{
    struct nodLS **vect;
    int nrElem;
} hashT;

int functieHash(int cheie, hashT *tabela)
{
    return cheie % tabela->nrElem;
}

void inserareHashT(hashT *t, produs p)
{
    nodLS *nou = malloc(sizeof(nodLS));
    nou->next = NULL;
    nou->inf = p;
    int poz = functieHash(p.cod, t);

    if (t->vect[poz] == NULL)
    {
        t->vect[poz] = nou;
        return;
    }
    nodLS *tmp = t->vect[poz];
    while (tmp->next)
    {
        tmp = tmp->next;
    }
    tmp->next = nou;
}

void parcurgereHashT(hashT *t)
{
    if (t->vect)
    {
        for (int i = 0; i < t->nrElem; i++)
        {
            if (t->vect[i])
            {
                printf("\nPOZ %d:", i);
                nodLS *tmp = t->vect[i];
                while (tmp)
                {
                    printf("\nCOD: %d , denumire: %s , pret: %.2f", tmp->inf.cod,
                           tmp->inf.denumire, tmp->inf.pret);
                    tmp = tmp->next;
                }
            }
        }
    }
}

int cautaDupaCod(hashT *t, int cod, produs *val)
{
    int poz = functieHash(cod, t);
    if (t->vect[poz])
    {
        nodLS *tmp = t->vect[poz];
        while (tmp)
        {
            if (tmp->inf.cod == cod)
            {
                *val = tmp->inf;
                (*val).denumire = malloc(strlen(tmp->inf.denumire) + 1);
                strcpy((*val).denumire, tmp->inf.denumire);
                return 1;
            }
            tmp = tmp->next;
        }
    }
    else
    {
        return 0;
    }
}

void dezalocareListaS(nodLS **cap)
{
    while (*cap)
    {
        nodLS *tmp = *cap;
        (*cap) = (*cap)->next;
        free(tmp->inf.denumire);
        free(tmp);
    }
}

void dezalocareHashT(hashT *t)
{
    for (int i = 0; i < t->nrElem; i++)
    {
        if (t->vect[i])
        {
            dezalocareListaS(&t->vect[i]);
        }
    }
    free(t->vect);
    t->vect = NULL;
}

int main()
{
    hashT t;
    FILE *f = fopen("fisier.txt", "r");
    if (f == NULL)
        return -1;
    fscanf(f, "%d", &t.nrElem);
    char buffer[200];
    produs p;
    t.vect = malloc(sizeof(nodLS *) * t.nrElem);
    for (int i = 0; i < t.nrElem; i++)
    {
        t.vect[i] = NULL;
    }
    for (int i = 0; i < t.nrElem; i++)
    {
        fscanf(f, "%d", &p.cod);
        fscanf(f, "%s", buffer);
        p.denumire = malloc(strlen(buffer) + 1);
        strcpy(p.denumire, buffer);
        fscanf(f, "%f", &p.pret);
        inserareHashT(&t, p);
    }

    // cauta dupa codul 123

    int ok = cautaDupaCod(&t, 103, &p);
    if (ok == 1)
    {
        printf("\nNUME PRODUS CAUTAT: %s", p.denumire);
        free(p.denumire);
    }
    else
    {
        printf("\nPRODUSUL NU EXISTA!");
    }

    // dezalocare
    dezalocareHashT(&t);
    return 0;
}
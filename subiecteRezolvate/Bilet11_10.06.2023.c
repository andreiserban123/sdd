#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct Fotografie
{
    unsigned int idFotografie;
    char *oras;
    char *data;
    float rezolutie;

} Fotografie;

typedef struct
{
    Fotografie inf;
    struct nodARB *st, *dr;
} nodARB;

void insertARB(nodARB **rad, Fotografie fot)
{
    if (*rad == NULL)
    {
        nodARB *nou = malloc(sizeof(nodARB));
        nou->inf = fot;
        nou->st = nou->dr = NULL;
        *rad = nou;
    }
    else if ((*rad)->inf.idFotografie > fot.idFotografie)
    {
        insertARB(&(*rad)->st, fot);
    }
    else
        insertARB(&(*rad)->dr, fot);
}

void traversarePreOrdine(nodARB *rad)
{
    if (rad)
    {
        printf("%d %s %s %.2f\n", rad->inf.idFotografie, rad->inf.oras, rad->inf.data, rad->inf.rezolutie);
        traversarePreOrdine(rad->st);
        traversarePreOrdine(rad->dr);
    }
}

void nrFotografii(nodARB *rad, int *nr, char *oras)
{
    if (rad)
    {
        if (strcmp(rad->inf.oras, oras) == 0)
        {
            (*nr)++;
        }
        nrFotografii(rad->st, nr, oras);
        nrFotografii(rad->dr, nr, oras);
    }
}

void traversarePostOrdine(nodARB *rad)
{
    if (rad)
    {
        traversarePostOrdine(rad->st);
        traversarePostOrdine(rad->dr);
        printf("%d %s %s %.2f\n", rad->inf.idFotografie, rad->inf.oras, rad->inf.data, rad->inf.rezolutie);
    }
}

void schimbaData(nodARB *rad, int id, char *data)
{
    if (rad == NULL)
    {
        return;
    }
    if (rad->inf.idFotografie == id)
    {
        free(rad->inf.data);
        rad->inf.data = malloc(strlen(data) + 1);
        strcpy(rad->inf.data, data);
    }
    else if (id < rad->inf.idFotografie)
    {
        schimbaData(rad->st, id, data);
    }
    else
    {
        schimbaData(rad->dr, id, data);
    }
}

nodARB *findMin(nodARB *rad)
{
    while (rad->st)
        rad = rad->st;
    return rad;
}

void getNrPhoto(nodARB *rad, float rez, int *nr)
{
    if (rad)
    {
        if (rad->inf.rezolutie == rez)
        {
            (*nr)++;
        }
        getNrPhoto(rad->st, rez, nr);
        getNrPhoto(rad->dr, rez, nr);
    }
}

void populareVect(nodARB *rad, Fotografie *v, int *i, float rez)
{
    if (rad)
    {
        if (rad->inf.rezolutie == rez)
        {
            v[(*i)] = rad->inf;
            v[(*i)].oras = malloc(strlen(rad->inf.oras) + 1);
            strcpy(v[(*i)].oras, rad->inf.oras);
            v[(*i)].data = malloc(strlen(rad->inf.data) + 1);
            strcpy(v[(*i)].data, rad->inf.data);
            (*i)++;
        }
        populareVect(rad->st, v, i, rez);
        populareVect(rad->dr, v, i, rez);
    }
}

Fotografie *salveazaVec(nodARB *rad, float rez, int *n)
{
    int nr = 0;
    getNrPhoto(rad, rez, &nr);
    Fotografie *v = malloc(sizeof(Fotografie) * nr);
    int i = 0;
    *n = nr;
    populareVect(rad, v, &i, rez);
    return v;
}

nodARB *stergeNodDupaId(nodARB *rad, int id)
{
    if (rad == NULL)
        return rad;
    else if (rad->inf.idFotografie == id)
    {
        if (rad->st == NULL)
        {
            nodARB *tmp = rad->dr;
            free(rad->inf.oras);
            free(rad->inf.data);
            free(rad);
            return tmp;
        }
        else if (rad->dr == NULL)
        {
            nodARB *tmp = rad->st;
            free(rad->inf.oras);
            free(rad->inf.data);
            free(rad);
            return tmp;
        }
        else
        {
            nodARB *tmp = findMin(rad->dr);
            free(rad->inf.data);
            free(rad->inf.oras);
            rad->inf = tmp->inf;
            // deep copy
            rad->inf.oras = malloc(strlen(tmp->inf.oras) + 1);
            strcpy(rad->inf.oras, tmp->inf.oras);
            rad->inf.data = malloc(strlen(tmp->inf.data) + 1);
            strcpy(rad->inf.data, tmp->inf.data);
            rad->dr = stergeNodDupaId(rad->dr, tmp->inf.idFotografie);
        }
    }
    else if (id < rad->inf.idFotografie)
    {
        rad->st = stergeNodDupaId(rad->st, id);
    }
    else
    {
        rad->dr = stergeNodDupaId(rad->dr, id);
    }
    return rad;
}

void afisareInOrdine(nodARB *rad)
{
    if (rad)
    {
        afisareInOrdine(rad->st);
        printf("%d %s %s %.2f\n", rad->inf.idFotografie, rad->inf.oras, rad->inf.data, rad->inf.rezolutie);
        afisareInOrdine(rad->dr);
    }
}

void dezalocareARB(nodARB **rad)
{
    if (*rad)
    {
        dezalocareARB(&(*rad)->st);
        dezalocareARB(&(*rad)->dr);
        free((*rad)->inf.oras);
        free((*rad)->inf.data);
        free(*rad);
    }
    *rad = NULL;
}

int main()
{
    FILE *f = fopen("Bilet11_10.06.2023.txt", "r");
    if (!f)
    {
        printf("Fisierul nu exista");
        return 1;
    }

    nodARB *rad = NULL;
    Fotografie fot;
    char buffer[100];
    while (fscanf(f, "%d", &fot.idFotografie) == 1)
    {
        fscanf(f, " %[^\n]", buffer);
        fot.oras = malloc((strlen(buffer) + 1) * sizeof(char));
        strcpy(fot.oras, buffer);
        fscanf(f, " %[^\n]", buffer);
        fot.data = malloc((strlen(buffer) + 1) * sizeof(char));
        strcpy(fot.data, buffer);
        fscanf(f, "%f", &fot.rezolutie);

        insertARB(&rad, fot);
    }
    fclose(f);
    traversarePreOrdine(rad);

    int nr = 0;
    nrFotografii(rad, &nr, "Paris");
    printf("\nNR de poze in paris: %d\n", nr);
    schimbaData(rad, 3, "02/06/2003");
    traversarePreOrdine(rad);
    rad = stergeNodDupaId(rad, rad->inf.idFotografie);
    printf("\n\n");
    afisareInOrdine(rad);
    Fotografie *v = NULL;
    int n = 0;
    v = salveazaVec(rad, 200, &n);
    printf("\n\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d %s %s %.2f\n", v[i].idFotografie, v[i].oras, v[i].data, v[i].rezolutie);
        free(v[i].oras);
        free(v[i].data);
    }
    free(v);
    v = NULL;
    dezalocareARB(&rad);
    return 0;
}
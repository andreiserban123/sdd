#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct
{
    int cod;
    char *denumire;
    float valoare;
} Proiect;

typedef struct
{
    struct nodARB *st, *dr;
    Proiect info;
} nodARB;

int calculInaltime(nodARB *rad)
{

    if (rad != NULL)
    {

        int st = calculInaltime(rad->st);
        int dr = calculInaltime(rad->dr);

        if (st > dr)
            return st + 1;
        else
            return dr + 1;
    }
    else
    {
        return 0;
    }
}

int difInaltime(nodARB *rad)
{
    if (rad != NULL)
    {
        return calculInaltime(rad->st) - calculInaltime(rad->dr);
    }
    else
    {
        return 0;
    }
}

void rotireLaStanga(nodARB **rad)
{
    nodARB *aux = (*rad)->dr;
    (*rad)->dr = aux->st;
    aux->st = *rad;
    *rad = aux;
}

void rotireLaDreapta(nodARB **rad)
{
    nodARB *aux = (*rad)->st;
    (*rad)->st = aux->dr;
    aux->dr = *rad;
    *rad = aux;
}

nodARB *findMin(nodARB *rad)
{
    while (rad->st)
    {
        rad = rad->st;
    }
    return rad;
}

nodARB *stergeNod(nodARB *rad, int cod)
{

    if (rad == NULL)
        return NULL;

    if (rad->info.cod == cod)
    {

        if (rad->st == NULL)
        {
            nodARB *tmp = rad->dr;
            free(rad->info.denumire);
            free(rad);
            return tmp;
        }
        else if (rad->dr == NULL)
        {
            nodARB *tmp = rad->st;
            free(rad->info.denumire);
            free(rad);
            return tmp;
        }
        else
        {
            nodARB *tmp = findMin(rad->dr);
            free(rad->info.denumire);
            rad->info = tmp->info;
            rad->info.denumire = malloc(strlen(tmp->info.denumire) + 1);
            strcpy(rad->info.denumire, tmp->info.denumire);
            rad->dr = stergeNod(rad->dr, tmp->info.cod);
        }
    }
    else if (cod < rad->info.cod)
    {
        rad->st = stergeNod(rad->st, cod);
    }
    else
    {
        rad->dr = stergeNod(rad->dr, cod);
    }

    return rad;
}

void valoareMedie(nodARB *rad, int *nr, float *sum)
{
    if (rad)
    {
        (*nr)++;
        *sum += rad->info.valoare;
        valoareMedie(rad->st, nr, sum);
        valoareMedie(rad->dr, nr, sum);
    }
}

void insertAVL(nodARB **rad, Proiect p)
{

    if (*rad == NULL)
    {
        nodARB *nou = malloc(sizeof(nodARB));
        nou->info = p;
        nou->dr = nou->st = NULL;
        *rad = nou;
    }
    else if (p.cod < (*rad)->info.cod)
    {
        insertAVL(&(*rad)->st, p);
    }
    else
        insertAVL(&(*rad)->dr, p);

    int dif = difInaltime(*rad);
    if (dif == 2)
    {
        // dezechilibru pe dreapta stanga
        if (difInaltime((*rad)->st) == -1)
            rotireLaStanga(&(*rad)->st);
        rotireLaDreapta(rad);
    }
    if (dif == -2)
    {
        // dezechilibru pe dreapta
        if (difInaltime((*rad)->dr) == 1)
            rotireLaDreapta(&(*rad)->dr);
        rotireLaStanga(rad);
    }
}

void afisarePreOrdine(nodARB *rad)
{
    if (rad)
    {
        printf("\nCOD:%d , denumire:%s , pret:%.2f", rad->info.cod, rad->info.denumire, rad->info.valoare);
        afisarePreOrdine(rad->st);
        afisarePreOrdine(rad->dr);
    }
}

void proiecteCuValpesteMediumVAL(nodARB *rad, float medium)
{
    if (rad)
    {
        if (rad->info.valoare > medium)
        {
            printf("\nCOD:%d , denumire:%s , pret:%.2f", rad->info.cod, rad->info.denumire, rad->info.valoare);
        }
        proiecteCuValpesteMediumVAL(rad->st, medium);
        proiecteCuValpesteMediumVAL(rad->dr, medium);
    }
}

void conversieVector(nodARB *rad, Proiect **v, int *i)
{
    if (rad)
    {
        conversieVector(rad->st, v, i);
        int k = *i;
        (*v)[k] = rad->info;
        (*v)[k].denumire = malloc(strlen(rad->info.denumire) + 1);
        strcpy((*v)[k].denumire, rad->info.denumire);
        k++;
        *i = k;
        conversieVector(rad->dr, v, i);
    }
}

typedef struct
{
    struct nodLD *next, *prev;
    Proiect info;
} nodLD;

typedef struct
{
    nodLD *prim;
} LD;

void sortLD(LD *lista)
{
    nodLD *tmp = lista->prim;
    int nr = 0;
    if (tmp != NULL)
    {
        do
        {
            nr++;
            tmp = tmp->next;

        } while (tmp != lista->prim);
    }

    Proiect *v = malloc(sizeof(Proiect) * nr);

    int i = 0;
    tmp = lista->prim;
    if (tmp != NULL)
    {
        do
        {
            v[i] = tmp->info;
            v[i].denumire = malloc(strlen(tmp->info.denumire) + 1);
            strcpy(v[i].denumire, tmp->info.denumire);
            i++;
            free(tmp->info.denumire);
            tmp = tmp->next;

        } while (tmp != lista->prim);
    }
    for (int i = 0; i < nr - 1; i++)
    {
        for (int j = i + 1; j < nr; j++)
        {
            if (v[i].valoare > v[j].valoare)
            {
                Proiect aux = v[i];
                v[i] = v[j];
                v[j] = aux;
            }
        }
    }
    tmp = lista->prim;
    i = 0;
    nodLD *prev = NULL;
    if (tmp != NULL)
    {
        do
        {
            tmp->info = v[i];
            if (prev)
            {
                prev->next = tmp;
                tmp->prev = prev;
            }
            prev = tmp;
            i++;
            tmp = tmp->next;

        } while (tmp != lista->prim);
    }
    prev->next = lista->prim;
    lista->prim->prev = prev;
    free(v);
}

void savingCirculare(nodARB *rad, LD *lista)
{
    if (rad)
    {
        nodLD *nou = malloc(sizeof(nodLD));
        nou->info = rad->info;
        nou->info.denumire = malloc(strlen(rad->info.denumire) + 1);
        strcpy(nou->info.denumire, rad->info.denumire);

        if (lista->prim == NULL)
        {
            nou->next = nou;
            nou->prev = nou;
            lista->prim = nou;
        }
        else
        {
            nodLD *ultim = lista->prim->prev;
            nou->next = lista->prim;
            (lista->prim)->prev = nou;
            ultim->next = nou;
            nou->prev = ultim;
        }

        savingCirculare(rad->st, lista);
        savingCirculare(rad->dr, lista);
    }
}

void dezARB(nodARB **rad)
{
    if (*rad)
    {
        dezARB(&(*rad)->st);
        dezARB(&(*rad)->dr);
        free((*rad)->info.denumire);
        free(*rad);
    }
    *rad = NULL;
}

void dezLD(LD *lista)
{

    if (lista->prim)
    {
        nodLD *tmp = lista->prim;

        do
        {
            nodLD *aux = tmp;
            tmp = tmp->next;
            free(aux->info.denumire);
            free(aux);

        } while (tmp != lista->prim);
    }

    lista->prim = NULL;
}

int main()
{
    FILE *f = fopen("fisier.txt", "r");
    if (f == NULL)
        return -1;

    char buffer[200];
    nodARB *rad = NULL;
    Proiect p;

    while (fscanf(f, "%d", &p.cod) == 1)
    {
        fscanf(f, " %[^\n]", buffer);
        p.denumire = malloc(strlen(buffer) + 1);
        strcpy(p.denumire, buffer);
        fscanf(f, "%f", &p.valoare);
        insertAVL(&rad, p);
    }
    afisarePreOrdine(rad);
    float sum = 0;
    int nr = 0;
    valoareMedie(rad, &nr, &sum);
    float mediu = sum / (float)nr;
    printf("\nVal medium %.2f", mediu);

    proiecteCuValpesteMediumVAL(rad, mediu);

    rad = stergeNod(rad, 4);
    nr--;
    printf("\n\nDupa stergerea nodului cu cod 4");
    afisarePreOrdine(rad);
    Proiect *v = malloc(sizeof(Proiect) * nr);
    int n = 0;
    conversieVector(rad, &v, &n);
    printf("\n\nVector:");
    for (int i = 0; i < n; i++)
    {
        printf("\nCOD:%d , denumire:%s , pret:%.2f", v[i].cod, v[i].denumire, v[i].valoare);
        free(v[i].denumire);
    }
    free(v);
    v = NULL;
    LD lista;
    lista.prim = NULL;
    savingCirculare(rad, &lista);

    nodLD *tmp = lista.prim;

    printf("\n");

    if (tmp != NULL)
    {

        do
        {
            printf("\nCOD:%d , denumire:%s , pret:%.2f", tmp->info.cod, tmp->info.denumire, tmp->info.valoare);
            tmp = tmp->next;
        } while (tmp != lista.prim);
    }

    sortLD(&lista);

    tmp = lista.prim;

    printf("\n");

    if (tmp != NULL)
    {

        do
        {
            printf("\nCOD:%d , denumire:%s , pret:%.2f", tmp->info.cod, tmp->info.denumire, tmp->info.valoare);
            tmp = tmp->next;
        } while (tmp != lista.prim);
    }

    printf("\n\n INVERS:");
    tmp = lista.prim;

    if (tmp != NULL)
    {

        do
        {
            printf("\nCOD:%d , denumire:%s , pret:%.2f", tmp->info.cod, tmp->info.denumire, tmp->info.valoare);
            tmp = tmp->prev;
        } while (tmp != lista.prim);
    }

    dezARB(&rad);
    dezLD(&lista);

    return 0;
}
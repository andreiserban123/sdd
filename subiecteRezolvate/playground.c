#define _CRT_SECURE_NO_WARNINGS
#include <malloc.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
    unsigned int id;
    char *denumireHotel;
    char *numeClient;
    float sumaDePlata;
} Rezervare;

typedef struct
{
    struct nodARB *st, *dr;
    Rezervare inf;
} nodARB;

void insertARB(nodARB **rad, Rezervare r)
{
    if (*rad == NULL)
    {
        nodARB *nou = malloc(sizeof(nodARB));
        nou->inf = r;
        nou->st = nou->dr = NULL;
        *rad = nou;
    }
    else if (r.id < (*rad)->inf.id)
    {
        insertARB(&(*rad)->st, r);
    }
    else
    {
        insertARB(&(*rad)->dr, r);
    }

    echilibrare(rad);
}

void parcurgerePreOrdine(nodARB *rad)
{
    if (rad)
    {
        printf("\nID:%u, numeCli:%s", rad->inf.id, rad->inf.numeClient);
        parcurgerePreOrdine(rad->st);
        parcurgerePreOrdine(rad->dr);
    }
}
void parcurgerePostOrdine(nodARB *rad)
{
    if (rad)
    {
        parcurgerePostOrdine(rad->st);
        parcurgerePostOrdine(rad->dr);
        printf("\nID:%u, numeCli:%s", rad->inf.id, rad->inf.numeClient);
    }
}

void sumaTotala(nodARB *rad, char *client, float *valoare)
{
    if (rad)
    {
        if (strcmp(rad->inf.numeClient, client) == 0)
        {
            (*valoare) += rad->inf.sumaDePlata;
        }

        sumaTotala(rad->st, client, valoare);
        sumaTotala(rad->dr, client, valoare);
    }
}
int cheiaMin(nodARB *rad)
{
    while (rad->st)
        rad = rad->st;
    return rad->inf.id;
}

nodARB *findMIN(nodARB *rad)
{
    while (rad->st)
        rad = rad->st;
    return rad;
}

nodARB *stergeNodDupaCheie(nodARB *rad, int id)
{
    if (rad == NULL)
    {
        return rad;
    }
    if (rad->inf.id == id)
    {
        if (rad->st == NULL)
        {
            nodARB *tmp = rad->dr;
            free(rad->inf.denumireHotel);
            free(rad->inf.numeClient);
            free(rad);
            return tmp;
        }
        else if (rad->dr == NULL)
        {
            nodARB *tmp = rad->st;
            free(rad->inf.denumireHotel);
            free(rad->inf.numeClient);
            free(rad);
            return tmp;
        }
        else
        {
            nodARB *tmp = findMIN(rad->dr);
            free(rad->inf.denumireHotel);
            free(rad->inf.numeClient);
            rad = tmp;
            rad->inf.denumireHotel = malloc(strlen(tmp->inf.denumireHotel) + 1);
            strcpy(rad->inf.denumireHotel, tmp->inf.denumireHotel);
            rad->inf.numeClient = malloc(strlen(tmp->inf.numeClient) + 1);
            strcpy(rad->inf.numeClient, tmp->inf.numeClient);

            rad->dr = stergeNodDupaCheie(rad->dr, tmp->inf.id);
        }
    }
    else if (rad->inf.id > id)
    {
        rad->st = stergeNodDupaCheie(rad->st, id);
    }
    else
        rad->dr = stergeNodDupaCheie(rad->dr, id);
    return rad;
}

void stergeIdCelMaiMic(nodARB **rad)
{
    int id = cheiaMin(*rad);
    *rad = stergeNodDupaCheie(*rad, id);
    echilibrare(rad);
}

int inaltime(nodARB *rad)
{
    if (rad)
    {
        int st = inaltime(rad->st);
        int dr = inaltime(rad->dr);
        if (st > dr)
        {
            return st + 1;
        }
        else
        {
            return dr + 1;
        }
    }
    else
    {
        return 0;
    }
}

void populareVector(nodARB *rad, int *v, int nivel)
{
    if (rad)
    {
        v[nivel]++;
        populareVector(rad->st, v, nivel + 1);
        populareVector(rad->dr, v, nivel + 1);
    }
}

int difInaltimi(nodARB *rad)
{
    if (rad)
    {
        return inaltime(rad->st) - inaltime(rad->dr);
    }
    else
    {
        return 0;
    }
}

void rotireLaDreapta(nodARB **rad)
{
    nodARB *aux = (*rad)->st;
    (*rad)->st = aux->dr;
    aux->dr = *rad;
    *rad = aux;
}

void rotireLaStanga(nodARB **rad)
{
    nodARB *aux = (*rad)->dr;
    (*rad)->dr = aux->st;
    aux->st = *rad;
    *rad = aux;
}

void echilibrare(nodARB **rad)
{
    int dif = difInaltimi(*rad);
    if (dif == -2)
    {
        // dezechilibrare pe dreapta
        if (difInaltimi((*rad)->dr) == 1)
            rotireLaDreapta(&(*rad)->dr);
        rotireLaStanga(rad);
    }
    if (dif == 2)
    {
        // dezechilibru pe partea stanga
        if (difInaltimi((*rad)->st) == -1)
            rotireLaStanga(&(*rad)->st);
        rotireLaDreapta(rad);
    }
}

int *vectorCuNrNoduriNivel(nodARB *rad, int *n)
{
    int l = inaltime(rad);
    int *vector = calloc(l, sizeof(int));
    *n = l;
    populareVector(rad, vector, 0);
    return vector;
}

Rezervare cautaDupaId(nodARB *rad, int id, int *ok)
{
    if (rad == NULL)
    {
        *ok = 0;
        Rezervare r;
        r.id = 0;
        return r;
    }
    else if (rad->inf.id == id)
    {
        *ok = 1;
        Rezervare r = rad->inf;
        r.denumireHotel = malloc(strlen(rad->inf.denumireHotel) + 1);
        strcpy(r.denumireHotel, rad->inf.denumireHotel);
        r.numeClient = malloc(strlen(rad->inf.numeClient) + 1);
        strcpy(r.numeClient, rad->inf.numeClient);
        return r;
    }
    else if (id < rad->inf.id)
    {
        cautaDupaId(rad->st, id, ok);
    }
    else
    {
        cautaDupaId(rad->dr, id, ok);
    }
}

int main()
{
    FILE *f = fopen("test.txt", "r");
    Rezervare r;
    char buffer[50];
    nodARB *rad = NULL;
    while (fscanf(f, "%u", &r.id) == 1)
    {
        fscanf(f, " %[^\n]", buffer);
        r.denumireHotel = malloc(strlen(buffer) + 1);
        strcpy(r.denumireHotel, buffer);
        fscanf(f, " %[^\n]", buffer);
        r.numeClient = malloc(strlen(buffer) + 1);
        strcpy(r.numeClient, buffer);
        fscanf(f, "%f", &r.sumaDePlata);
        insertARB(&rad, r);
    }
    parcurgerePreOrdine(rad);
    float val = 0;
    sumaTotala(rad, "Visan Dorel", &val);
    stergeIdCelMaiMic(&rad);
    // rad = stergeNodDupaCheie(rad, 2);

    int ok = 0;
    r = cautaDupaId(rad, 99, &ok);
    if (ok == 0)
    {
        printf("\nNu exista rezervare cu acest id");
    }
    else
    {
        printf("\n\nID:%d, numecliet:%s, suma:%.2f", r.id, r.numeClient, r.sumaDePlata);
        free(r.denumireHotel);
        free(r.numeClient);
    }

    echilibrare(&rad);
    printf("\n\nDUPA STERGERE!");
    parcurgerePreOrdine(rad);
    int lungime;
    int *numarNoduri = vectorCuNrNoduriNivel(rad, &lungime);
    for (int i = 0; i < lungime; i++)
    {
        printf("\nNivel %d: %d noduri", i, numarNoduri[i]);
    }
    free(numarNoduri);
    return 0;
}
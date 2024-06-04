#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct
{
    char *email;
    char *denumire;
    int nrFotografii;
    float spatiuOcupat;
} Album;

typedef struct
{

    struct nodARB *st, *dr;
    Album inf;
} nodARB;

typedef struct
{
    Album *vect;
    int nrEl;
} heap;

int inaltime(nodARB *rad)
{
    if (rad)
    {

        int st = inaltime(rad->st);
        int dr = inaltime(rad->dr);

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
    if (rad)
    {
        return inaltime(rad->st) - inaltime(rad->dr);
    }
    else
    {
        return 0;
    }
}

int findMinim(nodARB *rad)
{
    while (rad->st)
        rad = rad->st;
    return rad->inf.nrFotografii;
}

nodARB *findMin(nodARB *rad)
{
    while (rad->st)
        rad = rad->st;
    return rad;
}

nodARB *stergeDupaId(nodARB *rad, int id)
{
    if (rad == NULL)
    {
        return rad;
    }
    if (rad->inf.nrFotografii == id)
    {
        if (rad->st == NULL)
        {
            nodARB *tmp = rad->dr;
            free(rad->inf.email);
            free(rad->inf.denumire);
            free(rad);
            return tmp;
        }
        else if (rad->dr == NULL)
        {
            nodARB *tmp = rad->st;
            free(rad->inf.email);
            free(rad->inf.denumire);
            free(rad);
            return tmp;
        }
        else
        {
            nodARB *tmp = findMin(rad->dr);
            free(rad->inf.denumire);
            free(rad->inf.email);
            rad->inf.email = malloc(strlen(tmp->inf.email) + 1);
            strcpy(rad->inf.email, tmp->inf.email);
            rad->inf.denumire = malloc(strlen(tmp->inf.denumire) + 1);
            strcpy(rad->inf.denumire, tmp->inf.denumire);

            rad->st = stergeDupaId(rad->dr, tmp->inf.nrFotografii);
        }
    }
    else if (id < rad->inf.nrFotografii)
        rad->st = stergeDupaId(rad->st, id);
    else
        rad->dr = stergeDupaId(rad->dr, id);
    return rad;
}

void rotireLaDreapta(nodARB **rad)
{
    nodARB *aux = (*rad)->st;
    (*rad)->st = aux->dr;
    aux->dr = (*rad);
    *rad = aux;
}
void rotireLaStanga(nodARB **rad)
{
    nodARB *aux = (*rad)->dr;
    (*rad)->dr = aux->st;
    aux->st = (*rad);
    *rad = aux;
}

void echilibreaza(nodARB **rad)
{
    int dif = difInaltime(*rad);
    if (dif == -2)
    {
        if (difInaltime((*rad)->dr) == 1)
            rotireLaDreapta(&(*rad)->dr);
        rotireLaStanga(rad);
    }
    else if (dif == 2)
    {
        if (difInaltime((*rad)->st) == -1)
            rotireLaStanga(&(*rad)->st);
        rotireLaDreapta(rad);
    }
}

void stergeMin(nodARB **rad)
{
    int id = findMinim(*rad);
    *rad = stergeDupaId(*rad, id);
    echilibreaza(rad);
}

void insertARB(nodARB **rad, Album a)
{
    if (*rad == NULL)
    {
        nodARB *nou = malloc(sizeof(nodARB));
        nou->st = nou->dr = NULL;
        nou->inf = a;
        *rad = nou;
    }
    else if (a.nrFotografii < (*rad)->inf.nrFotografii)
    {
        insertARB(&(*rad)->st, a);
    }
    else
    {
        insertARB(&(*rad)->dr, a);
    }

    echilibreaza(rad);
}

void traversarePreOrdine(nodARB *rad)
{
    if (rad)
    {
        printf("\nNR. fotografii:%d, email:%s, denumire:%s, spatiu stocat %.2f ", rad->inf.nrFotografii,
               rad->inf.email, rad->inf.denumire, rad->inf.spatiuOcupat);
        traversarePreOrdine(rad->st);
        traversarePreOrdine(rad->dr);
    }
}

void traversarePostOrdine(nodARB *rad)
{
    if (rad)
    {
        traversarePostOrdine(rad->st);
        traversarePostOrdine(rad->dr);
        printf("\nNR. fotografii:%d, email:%s, denumire:%s, spatiu stocat %.2f ", rad->inf.nrFotografii,
               rad->inf.email, rad->inf.denumire, rad->inf.spatiuOcupat);
    }
}

void nrAlbumeDupaUtil(nodARB *rad, char *email, int *nr)
{
    if (rad)
    {
        if (strcmp(rad->inf.email, email) == 0)
        {
            (*nr)++;
        }
        nrAlbumeDupaUtil(rad->st, email, nr);
        nrAlbumeDupaUtil(rad->dr, email, nr);
    }
}

void populeazaHeap(nodARB *rad, char *email, heap h, int *i)
{
    if (rad)
    {
        if (strcmp(rad->inf.email, email) == 0)
        {
            h.vect[*i] = rad->inf;
            h.vect[*i].denumire = malloc(strlen(rad->inf.denumire) + 1);
            strcpy(h.vect[*i].denumire, rad->inf.denumire);

            h.vect[*i].email = malloc(strlen(rad->inf.email) + 1);
            strcpy(h.vect[*i].email, rad->inf.email);
            (*i)++;
        }
        populeazaHeap(rad->st, email, h, i);
        populeazaHeap(rad->dr, email, h, i);
    }
}

void filtreaza(int index, heap h)
{
    int pozRad = index;
    int st = pozRad * 2 + 1;
    int dr = pozRad * 2 + 2;

    if (st < h.nrEl && h.vect[pozRad].spatiuOcupat < h.vect[st].spatiuOcupat)
        pozRad = st;
    if (dr < h.nrEl && h.vect[pozRad].spatiuOcupat < h.vect[dr].spatiuOcupat)
        pozRad = dr;

    if (index != pozRad)
    {
        Album aux = h.vect[index];
        h.vect[index] = h.vect[pozRad];
        h.vect[pozRad] = aux;
        filtreaza(pozRad, h);
    }
}

heap extrage(heap h, Album *a)
{
    *a = h.vect[0];
    h.vect[0] = h.vect[h.nrEl - 1];
    h.nrEl--;
    Album *v = malloc(sizeof(Album) * h.nrEl);
    for (int i = 0; i < h.nrEl; i++)
    {
        v[i] = h.vect[i];
    }
    free(h.vect);
    h.vect = v;
    if (h.nrEl == 0)
    {
        free(h.vect);
        h.vect = NULL;
    }
    return h;
}

void dezalocARB(nodARB **rad)
{
    if (*rad)
    {
        dezalocARB(&(*rad)->st);
        dezalocARB(&(*rad)->dr);
        free((*rad)->inf.email);
        free((*rad)->inf.denumire);
        free(*rad);
        *rad = NULL;
    }
}

heap salveazaHeap(nodARB *rad, char *email)
{
    heap h;
    int nr = 0;
    nrAlbumeDupaUtil(rad, email, &nr);
    h.vect = malloc(sizeof(Album) * nr);
    h.nrEl = nr;
    nr = 0;
    populeazaHeap(rad, email, h, &nr);
    for (int i = (h.nrEl - 1) / 2; i >= 0; i--)
        filtreaza(i, h);
    return h;
}

int main()
{
    FILE *f = fopen("bilet14.txt", "r");
    if (!f)
        return 1;
    Album a;
    char buffer[200];
    nodARB *rad = NULL;
    while (fscanf(f, " %[^\n]", buffer) == 1)
    {
        a.email = malloc(strlen(buffer) + 1);
        strcpy(a.email, buffer);
        fscanf(f, " %[^\n]", buffer);
        a.denumire = malloc(strlen(buffer) + 1);
        strcpy(a.denumire, buffer);
        fscanf(f, "%d", &a.nrFotografii);
        fscanf(f, "%f", &a.spatiuOcupat);
        insertARB(&rad, a);
    }
    traversarePreOrdine(rad);
    stergeMin(&rad);
    printf("\n\n");
    traversarePreOrdine(rad);
    printf("\n");
    traversarePostOrdine(rad);
    heap h;
    printf("\n\n");
    h = salveazaHeap(rad, "john.doe@example.com");
    for (int i = 0; i < h.nrEl; i++)
    {
        printf("\nNR. fotografii:%d, email:%s, denumire:%s, spatiu stocat %.2f ", h.vect[i].nrFotografii,
               h.vect[i].email, h.vect[i].denumire, h.vect[i].spatiuOcupat);
    }

    printf("\nExtrageri repetate:\n");
    int nr = h.nrEl;
    for (int i = 0; i < nr; i++)
    {
        h = extrage(h, &a);
        printf("\nNR. fotografii:%d, email:%s, denumire:%s, spatiu stocat %.2f ", a.nrFotografii,
               a.email, a.denumire, a.spatiuOcupat);
        free(a.email);
        free(a.denumire);
    }
    dezalocARB(&rad);
    return 0;
}
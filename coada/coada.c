#define _CRT_SECURE_NO_WARNINGS

#include <malloc.h>
#include <string.h>
#include <stdio.h>

typedef struct
{
    int zi, luna, an;
} dataReceptionare;

typedef struct
{
    dataReceptionare data;
    int prioritate;
    char *aplicatie;
    float durata;
} notificare;

typedef struct
{
    notificare inf;
    struct nodCoada *next;
} nodCoada;

nodCoada *put(nodCoada *prim, nodCoada **ultim, notificare n)
{
    nodCoada *nou = malloc(sizeof(nodCoada));
    nou->inf = n;
    nou->next = NULL;
    if (prim == NULL || (*ultim) == NULL)
    {
        *ultim = nou;
        return nou;
    }
    (*ultim)->next = nou;
    *ultim = nou;
    return prim;
}

int get(nodCoada **prim, notificare *val)
{

    if (*prim != NULL)
    {
        *val = (*prim)->inf;
        (*val).aplicatie = malloc(strlen((*prim)->inf.aplicatie) + 1);
        strcpy((*val).aplicatie, (*prim)->inf.aplicatie);
        nodCoada *tmp = *prim;
        (*prim) = (*prim)->next;
        free(tmp->inf.aplicatie);
        free(tmp);
        return 1;
    }
    else
    {
        return 0;
    }
}

int main()
{
    FILE *f = fopen("fisier.txt", "r");
    nodCoada *prim = NULL, *ultim = NULL;
    char buffer[20];
    notificare notif;

    int nrEl;
    fscanf(f, "%d", &nrEl);
    for (int i = 0; i < nrEl; i++)
    {
        fscanf(f, "%d", &notif.data.zi);
        fscanf(f, "%d", &notif.data.luna);
        fscanf(f, "%d", &notif.data.an);

        fscanf(f, "%d", &notif.prioritate);
        fscanf(f, "%s", buffer);
        notif.aplicatie = malloc(strlen(buffer) + 1);
        strcpy(notif.aplicatie, buffer);
        fscanf(f, "%f", &notif.durata);
        prim = put(prim, &ultim, notif);
    }
    fclose(f);

    while (get(&prim, &notif) == 1)
    {
        printf("\nData: %d/%d/%d, Prioritate: %d, Aplicatie: %s, Durata: %5.2f",
               notif.data.zi, notif.data.luna,
               notif.data.an, notif.prioritate,
               notif.aplicatie, notif.durata);
        free(notif.aplicatie);
    }
    ultim = NULL;

    return 0;
}
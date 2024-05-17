#define _CRT_SECURE_NO_WARNINGS

#include <malloc.h>
#include <string.h>
#include <stdio.h>

typedef struct dataReceptionare
{
    int zi, luna, an;
} dataReceptionare;

typedef struct notificare
{
    dataReceptionare data;
    int prioritate;
    char *aplicatie;
    float durata;
} notificare;

typedef struct nodStiva
{
    notificare inf;
    struct nodStiva *next;
} nodStiva;

nodStiva *push(nodStiva *varf, notificare notif)
{
    nodStiva *nou = malloc(sizeof(nodStiva));
    nou->inf = notif;
    nou->next = NULL;
    if (varf == NULL)
    {
        return nou;
    }
    nou->next = varf;
    varf = nou;
    return varf;
}

int pop(nodStiva **varf, notificare *val)
{
    if (*varf)
    {
        *val = (*varf)->inf;
        (*val).aplicatie = malloc(strlen((*varf)->inf.aplicatie) + 1);
        strcpy((*val).aplicatie, (*varf)->inf.aplicatie);
        nodStiva *tmp = (*varf);
        (*varf) = (*varf)->next;
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
    char buffer[20];
    notificare notif;
    nodStiva *varf = NULL;
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
        varf = push(varf, notif);
    }
    fclose(f);
    while (pop(&varf, &notif) == 1)
    {
        printf("\nData: %d/%d/%d, Prioritate: %d, Aplicatie: %s, Durata: %5.2f",
               notif.data.zi, notif.data.luna,
               notif.data.an, notif.prioritate,
               notif.aplicatie, notif.durata);
        free(notif.aplicatie);
    }
    return 0;
}
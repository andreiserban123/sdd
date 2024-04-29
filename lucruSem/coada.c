#include <stdio.h>
#include <malloc.h>
#include <string.h>
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

typedef struct nodCoada
{
    struct nodCoada *next;
    notificare inf;
} nodCoada;

typedef struct Coada
{
    nodCoada *prim;
    nodCoada *ultim;
} Coada;

void put(Coada *c, notificare inf)
{
    nodCoada *nou = malloc(sizeof(nodCoada));
    nou->next = NULL;
    nou->inf = inf;
    nou->inf.aplicatie = malloc(strlen(inf.aplicatie) + 1);
    strcpy(nou->inf.aplicatie, inf.aplicatie);
    if (c->prim == NULL || c->ultim == NULL)
    {
        c->prim = c->ultim = nou;
        return;
    }
    c->ultim->next = nou;
    c->ultim = nou;
}

int get(Coada *c, notificare *val)
{
    if (c->prim == NULL || c->ultim == NULL)
    {
        return -1;
    }
    nodCoada *tmp = c->prim;
    *val = c->prim->inf;
    val->aplicatie = malloc(strlen(c->prim->inf.aplicatie) + 1);
    strcpy(val->aplicatie, c->prim->inf.aplicatie);
    c->prim = c->prim->next;
    free(tmp->inf.aplicatie);
    free(tmp);
    return 0;
}

int main()
{
    Coada coada;
    coada.prim = coada.ultim = NULL;
    char buffer[20];
    notificare notif;
    FILE *f = fopen("stiva_coada.txt", "r");
    if (f == NULL)
    {
        printf("Fis nu s-a deschis!");
        return -1;
    }
    while (fscanf(f, "%d", &notif.data.zi) == 1)
    {
        fscanf(f, "%d", &notif.data.luna);
        fscanf(f, "%d", &notif.data.an);
        fscanf(f, "%d", &notif.prioritate);
        fscanf(f, "%s", buffer);
        notif.aplicatie = (char *)malloc((strlen(buffer) + 1) * sizeof(char));
        strcpy(notif.aplicatie, buffer);
        fscanf(f, "%f", &notif.durata);
        put(&coada, notif);
    }
    while (get(&coada, &notif) == 0)
    {
        printf("\nData: %d/%d/%d, Prioritate: %d, Aplicatie: %s, Durata: %5.2f",
               notif.data.zi, notif.data.luna,
               notif.data.an, notif.prioritate,
               notif.aplicatie, notif.durata);
        free(notif.aplicatie);
    }
    return 0;
}

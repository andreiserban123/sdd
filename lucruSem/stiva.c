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

typedef struct nodStiva
{
    notificare inf;
    struct nodstiva *next;
} nodStiva;

void push(nodStiva **varf, notificare inf)
{
    nodStiva *nou = malloc(sizeof(nodStiva));
    nou->inf = inf;
    nou->next = (*varf);
    *varf = nou;
}

int pop(nodStiva **varf, notificare *val)
{
    if (*varf == NULL)
    {
        return -1;
    }
    nodStiva *tmp = *varf;
    *val = (*varf)->inf;
    val->aplicatie = malloc(strlen((*varf)->inf.aplicatie) + 1);
    strcpy(val->aplicatie, (*varf)->inf.aplicatie);
    *varf = (*varf)->next;
    free(tmp->inf.aplicatie);
    free(tmp);
    return 0;
}

int main(int argc, char const *argv[])
{
    nodStiva *varf = NULL;
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
        push(&varf, notif);
    }
    while (pop(&varf, &notif) != -1)
    {
        printf("\nData: %d/%d/%d, Prioritate: %d, Aplicatie: %s, Durata: %5.2f",
               notif.data.zi, notif.data.luna,
               notif.data.an, notif.prioritate,
               notif.aplicatie, notif.durata);
        free(notif.aplicatie);
    }
    printf("\nValoare stiva %s", varf);
    fclose(f);
    return 0;
}

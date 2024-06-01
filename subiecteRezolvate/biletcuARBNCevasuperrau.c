#define _CRT_SECURE_NO_WARNINGS
#include <malloc.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
    char *emailUtilizator;
    char *denumireAlbum;
    int numarFotografii;
    float spatiuOcupat;
} Album;

typedef struct nod
{
    Album inf;
    struct nod *st, *dr;
} nod;

void insertABC(nod **rad, Album a)
{
    if (*rad == NULL)
    {
        nod *nou = (nod *)malloc(sizeof(nod));
        nou->inf = a;
        nou->st = nou->dr = NULL;
        *rad = nou;
    }
    else
    {
        if ((*rad)->inf.numarFotografii > a.numarFotografii)
        {
            insertABC(&(*rad)->st, a);
        }
        else
        {
            insertABC(&(*rad)->dr, a);
        }
    }
}

void preOrdine(nod *rad)
{
    if (rad)
    {
        printf("\n Email: %s, DenumireAlbum: %s, Numar Poze: %d, Memorie: %.2f",
               rad->inf.emailUtilizator, rad->inf.denumireAlbum, rad->inf.numarFotografii,
               rad->inf.spatiuOcupat);
        preOrdine(rad->st);
        preOrdine(rad->dr);
    }
}

int nrMin(nod *rad)
{
    while (rad->st)
    {
        rad = rad->st;
    }
    return rad->inf.numarFotografii;
}

nod *findMin(nod *rad)
{
    while (rad->st)
    {
        rad = rad->st;
    }
    return rad;
}

nod *stergeNod(nod *rad, int key)
{
    if (rad == NULL)
    {
        return rad;
    }

    if (rad->inf.numarFotografii == key)
    {
        nod *tmp;
        if (rad->st == NULL)
        {
            tmp = rad->dr;
            free(rad->inf.denumireAlbum);
            free(rad->inf.emailUtilizator);
            free(rad);
            return tmp;
        }
        else if (rad->dr == NULL)
        {
            tmp = rad->st;
            free(rad->inf.denumireAlbum);
            free(rad->inf.emailUtilizator);
            free(rad);
            return tmp;
        }
        else
        {
            tmp = findMin(rad->dr); // cel mai mic din dreapta

            free(rad->inf.denumireAlbum);
            free(rad->inf.emailUtilizator);

            rad->inf = tmp->inf;
            rad->inf.emailUtilizator = malloc(strlen(tmp->inf.emailUtilizator) + 1);
            strcpy(rad->inf.emailUtilizator, tmp->inf.emailUtilizator);
            rad->inf.denumireAlbum = malloc(strlen(tmp->inf.denumireAlbum) + 1);
            strcpy(rad->inf.denumireAlbum, tmp->inf.denumireAlbum);
            rad->dr = stergeNod(rad->dr, tmp->inf.numarFotografii);
        }
    }
    else if (key < rad->inf.numarFotografii)
    {
        rad->st = stergeNod(rad->st, key);
    }
    else if (key > rad->inf.numarFotografii)
    {
        rad->dr = stergeNod(rad->dr, key);
    }

    return rad;
}

void dezalocareABC(nod *rad)
{
    if (rad)
    {
        dezalocareABC(rad->st);
        dezalocareABC(rad->dr);
        free(rad->inf.emailUtilizator);
        free(rad->inf.denumireAlbum);
        free(rad);
    }
}
void reverseInOrdine(nod *rad)
{
    if (rad)
    {

        reverseInOrdine(rad->dr);
        reverseInOrdine(rad->st);
        printf("\nEmail: %s, DenumireAlbum: %s, Numar Poze: %d, Memorie: %.2f",
               rad->inf.emailUtilizator, rad->inf.denumireAlbum, rad->inf.numarFotografii,
               rad->inf.spatiuOcupat);
    }
}

int main()
{
    nod *rad = NULL;
    FILE *f = fopen("fisier.txt", "r");
    if (!f)
    {
        printf("Eroare deschidere fisier!\n");
        return 1;
    }
    char buffer[50];
    Album a;
    while (fscanf(f, " %[^\n]", buffer) == 1)
    {
        a.emailUtilizator = malloc(strlen(buffer) + 1);
        strcpy(a.emailUtilizator, buffer);
        fscanf(f, " %[^\n]", buffer);
        a.denumireAlbum = malloc(strlen(buffer) + 1);
        strcpy(a.denumireAlbum, buffer);
        fscanf(f, "%d", &a.numarFotografii);
        fscanf(f, "%f", &a.spatiuOcupat);
        insertABC(&rad, a);
    }
    fclose(f);

    printf("\nAfisare arbore in preordine inainte de stergere:\n");
    preOrdine(rad);

    int min = nrMin(rad);
    printf("\nMinimul de poze este: %d\n", min);

    rad = stergeNod(rad, 30);

    printf("\nPreordine:");
    preOrdine(rad);
    printf("\nInversiune:");
    reverseInOrdine(rad);
    dezalocareABC(rad);

    return 0;
}

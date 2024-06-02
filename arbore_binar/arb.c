#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct
{
    int nrMatricol;
    char *nume;
    float medie;
} student;

typedef struct nodArb
{
    student inf;
    struct nodArb *st, *dr;
} nodArb;

void inserare(nodArb **rad, student s)
{
    if (*rad == NULL)
    {
        nodArb *nou = malloc(sizeof(nodArb));
        nou->inf = s;
        nou->dr = nou->st = NULL;
        *rad = nou;
    }
    else
    {
        if (s.nrMatricol < (*rad)->inf.nrMatricol)
            inserare(&(*rad)->st, s);
        else
            inserare(&(*rad)->dr, s);
    }
}
// nodArb *stergeNod(nodArb *rad, int key)
// {
//     if (rad == NULL)
//     {
//         return rad;
//     }

//     if (rad->inf.numarFotografii == key)
//     {
//         nodArb *tmp;
//         if (rad->st == NULL)
//         {
//             tmp = rad->dr;
//             free(rad->inf.denumireAlbum);
//             free(rad->inf.emailUtilizator);
//             free(rad);
//             return tmp;
//         }
//         else if (rad->dr == NULL)
//         {
//             tmp = rad->st;
//             free(rad->inf.denumireAlbum);
//             free(rad->inf.emailUtilizator);
//             free(rad);
//             return tmp;
//         }
//         else
//         {
//             tmp = findMin(rad->dr); // cel mai mic din dreapta

//             free(rad->inf.denumireAlbum);
//             free(rad->inf.emailUtilizator);

//             rad->inf = tmp->inf;
//             rad->inf.emailUtilizator = malloc(strlen(tmp->inf.emailUtilizator) + 1);
//             strcpy(rad->inf.emailUtilizator, tmp->inf.emailUtilizator);
//             rad->inf.denumireAlbum = malloc(strlen(tmp->inf.denumireAlbum) + 1);
//             strcpy(rad->inf.denumireAlbum, tmp->inf.denumireAlbum);
//             rad->dr = stergeNod(rad->dr, tmp->inf.numarFotografii);
//         }
//     }
//     else if (key < rad->inf.numarFotografii)
//     {
//         rad->st = stergeNod(rad->st, key);
//     }
//     else if (key > rad->inf.numarFotografii)
//     {
//         rad->dr = stergeNod(rad->dr, key);
//     }

//     return rad;
// }
void afisareInOrdine(nodArb *rad)
{
    if (rad != NULL)
    {
        afisareInOrdine(rad->st);
        printf("\nNR MAT:%d, nume:%s, medie:%.2f", rad->inf.nrMatricol, rad->inf.nume,
               rad->inf.medie);
        afisareInOrdine(rad->dr);
    }
}

int cautareDupaNrMat(int nrMat, nodArb *rad)
{
    if (rad)
    {
        if (rad->inf.nrMatricol == nrMat)
        {
            return 1;
        }
        else
        {
            if (nrMat < rad->inf.nrMatricol)
            {
                cautareDupaNrMat(nrMat, rad->st);
            }
            else
                cautareDupaNrMat(nrMat, rad->dr);
        }
    }
    else
    {
        return 0;
    }
}

void dezalocare(nodArb **rad)
{
    if (*rad)
    {
        dezalocare(&(*rad)->st);
        dezalocare(&(*rad)->dr);
        free((*rad)->inf.nume);
        free(*rad);
    }
    *rad = NULL;
}

int calculInaltime(nodArb *rad)
{
    if (rad)
    {
        int inaltimeS = calculInaltime(rad->st);
        int inaltimeD = calculInaltime(rad->dr);
        if (inaltimeD > inaltimeS)
            return inaltimeD + 1;
        else
            return inaltimeS + 1;
    }
    else
    {
        return 0;
    }
}

int main()
{

    FILE *f = fopen("fisier.txt", "r");
    nodArb *radacina = NULL;
    int nr;
    student s;
    char buffer[20];
    fscanf(f, "%d", &nr);
    for (int i = 0; i < nr; i++)
    {
        fscanf(f, "%d", &s.nrMatricol);
        fscanf(f, "%s", buffer);
        s.nume = malloc(strlen(buffer) + 1);
        strcpy(s.nume, buffer);

        fscanf(f, "%f", &s.medie);
        inserare(&radacina, s);
    }

    afisareInOrdine(radacina);

    printf("\nCautare nrMat 17: %d", cautareDupaNrMat(17, radacina));
    printf("\nInaltime: %d", calculInaltime(radacina));

    dezalocare(&radacina);
    return 0;
}
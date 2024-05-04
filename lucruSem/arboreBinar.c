#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <malloc.h>

typedef struct Cofetarie
{
    int id;
    char *nume;
} Cofetarie;

typedef struct nod
{
    struct nod *stanga;
    struct nod *dreapta;
    Cofetarie inf;
} nod;

void inserare(nod **radacina, Cofetarie inf)
{
    if ((*radacina) == NULL)
    {
        nod *nou = malloc(sizeof(nod));
        nou->inf = inf;
        nou->stanga = NULL;
        nou->dreapta = NULL;
        *radacina = nou;
    }
    else
    {
        if (inf.id > (*radacina)->inf.id)
        {
            inserare(&(*radacina)->dreapta, inf);
        }
        else
        {
            inserare(&(*radacina)->stanga, inf);
        }
    }
}

void afisareInOrdine(nod *radacina)
{
    if (radacina != NULL)
    {
        afisareInOrdine(radacina->stanga);
        printf("\nID: %d, denumire:%s", radacina->inf.id, radacina->inf.nume);
        afisareInOrdine(radacina->dreapta);
    }
}
void afisarePreOrdine(nod *radacina)
{
    if (radacina)
    {
        printf("\nID: %d, denumire:%s", radacina->inf.id, radacina->inf.nume);
        afisarePreOrdine(radacina->stanga);
        afisarePreOrdine(radacina->dreapta);
    }
}

void afisarePostOrdine(nod *radacina)
{
    if (radacina)
    {
        afisarePostOrdine(radacina->stanga);
        afisarePostOrdine(radacina->dreapta);
        printf("\nID: %d, denumire:%s", radacina->inf.id, radacina->inf.nume);
    }
}

void cautareDupaId(Cofetarie *c, nod *radacina, int idCautat)
{
    if (radacina)
    {
        if (radacina->inf.id == idCautat)
            *c = radacina->inf;
        else
        {
            if (idCautat < radacina->inf.id)
            {
                cautareDupaId(c, radacina->stanga, idCautat);
            }
            else
            {
                cautareDupaId(c, radacina->dreapta, idCautat);
            }
        }
    }
    else
    {
        Cofetarie x;
        x.id = -1;
        x.nume = malloc(1);
        strcpy(x.nume, "");
        *c = x;
    }
}

void dezalocareArbore(nod **radacina)
{
    if (*radacina)
    {
        dezalocareArbore(&(*radacina)->stanga);
        dezalocareArbore(&(*radacina)->dreapta);
        free((*radacina)->inf.nume);
        free((*radacina));
    }
    *radacina = NULL;
}

int calculInaltime(nod *radacina)
{
    if (radacina)
    {
        int inaltimeS = calculInaltime(radacina->stanga);
        int inaltimeD = calculInaltime(radacina->dreapta);

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
    FILE *f = fopen("arb.txt", "r");
    Cofetarie c;
    nod *radacina = NULL;
    while (fscanf(f, "%d", &c.id) == 1)
    {
        char buffer[200];
        fscanf(f, "%s", buffer);
        c.nume = malloc(strlen(buffer) + 1);
        strcpy(c.nume, buffer);
        inserare(&radacina, c);
    }
    afisarePreOrdine(radacina);
    cautareDupaId(&c, radacina, 1000);
    printf("\nID CAUTAT: %d, nume:%s", c.id, c.nume);
    if (c.id == -1)
    {
        free(c.nume);
    }

    int inaltime = calculInaltime(radacina);
    printf("\nInaltimea Arborelului:%d", inaltime);

    dezalocareArbore(&radacina);
    return 0;
}

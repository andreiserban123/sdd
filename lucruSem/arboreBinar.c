#include <stdio.h>
#include <string.h>
#include <malloc.h>

typedef struct Cofetarie
{
    int id;
    char *nume;
} Cofetarie;

typedef struct Nod
{
    Cofetarie inf;
    struct Nod *stanga;
    struct Nod *dreapta;
} Nod;

void inserareArbore(Cofetarie c, Nod **radacina)
{
    if (*radacina == NULL)
    {
        Nod *nou = malloc(sizeof(Nod));
        nou->inf = c;
        nou->dreapta = nou->stanga = NULL;
        *radacina = nou;
    }
    else
    {
        if (c.id > (*radacina)->inf.id)
        {
            inserareArbore(c, &(*radacina)->dreapta);
        }
        else
        {
            inserareArbore(c, &(*radacina)->stanga);
        }
    }
}

void afisareInOrdine(Nod *radacina)
{
    if (radacina != NULL)
    {
        afisareInOrdine(radacina->stanga);
        printf("\nID: %d, denumire:%s", radacina->inf.id, radacina->inf.nume);
        afisareInOrdine(radacina->dreapta);
    }
}
void afisarePreOrdine(Nod *radacina)
{
    if (radacina)
    {
        printf("\nID: %d, denumire:%s", radacina->inf.id, radacina->inf.nume);
        afisarePreOrdine(radacina->stanga);
        afisarePreOrdine(radacina->dreapta);
    }
}

void afisarePostOrdine(Nod *radacina)
{
    if (radacina)
    {
        afisarePostOrdine(radacina->stanga);
        afisarePostOrdine(radacina->dreapta);
        printf("\nID: %d, denumire:%s", radacina->inf.id, radacina->inf.nume);
    }
}

void dezalocareArbore(Nod **radacina)
{
    if (*radacina)
    {
        dezalocareArbore(&(*radacina)->stanga);
        dezalocareArbore(&(*radacina)->dreapta);
        free((*radacina)->inf.nume);
        free(*radacina);
    }
    *radacina = NULL;
}

int main(int argc, char const *argv[])
{
    FILE *f = fopen("arb.txt", "r");
    Nod *radacina = NULL;
    Cofetarie c;
    while (fscanf(f, "%d", &c.id) == 1)
    {
        char buffer[200];
        fscanf(f, "%s", buffer);
        c.nume = malloc(strlen(buffer) + 1);
        strcpy(c.nume, buffer);
        inserareArbore(c, &radacina);
    }
    // afisareInOrdine(radacina);
    afisarePostOrdine(radacina);
    dezalocareArbore(&radacina);
    return 0;
}

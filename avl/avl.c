#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct Cofetarie Cofetarie;
typedef struct Nod Nod;

struct Cofetarie
{
    int id;
    char *nume;
};

struct Nod
{
    Cofetarie info;
    Nod *stanga;
    Nod *dreapta;
};

Cofetarie initializareCofetarie(int id, char *nume)
{
    Cofetarie c;
    c.id = id;
    c.nume = (char *)malloc(sizeof(char) * (strlen(nume) + 1));
    strcpy(c.nume, nume);
    return c;
}

void afisareCofetarie(Cofetarie c)
{
    printf("\n Cofetaria %s are id-ul %d", c.nume, c.id);
}

void inserareArbore(Cofetarie c, Nod **radacina)
{
    if (*radacina == NULL)
    {
        Nod *nou = (Nod *)malloc(sizeof(Nod));
        nou->info = c;
        nou->dreapta = NULL;
        nou->stanga = NULL;
        *radacina = nou;
    }
    else
    {
        if (c.id > (*radacina)->info.id)
            inserareArbore(c, &(*radacina)->dreapta);
        else
            inserareArbore(c, &(*radacina)->stanga);
    }
}

void afisareInOrdine(Nod *radacina)
{
    // afisare In ordine : Stanga,Radacina,Dreapta
    if (radacina != NULL)
    {
        afisareInOrdine(radacina->stanga);
        afisareCofetarie(radacina->info);
        afisareInOrdine(radacina->dreapta);
    }
}
void afisarePreOrdine(Nod *radacina)
{
    // Afisare preordine : Radacina,Stanga,Dreapta
    if (radacina != NULL)
    {
        afisareCofetarie(radacina->info);
        afisarePreOrdine(radacina->stanga);
        afisarePreOrdine(radacina->dreapta);
    }
}

void dezalocareArbore(Nod **radacina)
{
    if (*radacina)
    {
        dezalocareArbore(&(*radacina)->stanga);
        dezalocareArbore(&(*radacina)->dreapta);
        free((*radacina)->info.nume);
        free(*radacina);
    }
    *radacina = NULL;
}

void cautareDupaId(Cofetarie *c, Nod *radacina, int idCautat)
{

    if (radacina)
    {
        if (radacina->info.id == idCautat)
            *c = initializareCofetarie(idCautat, radacina->info.nume);
        else
        {
            if (idCautat < radacina->info.id)
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
        *c = initializareCofetarie(-1, "");
}

int calculInaltime(Nod *radacina)
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

void rotireLaStanga(Nod **radacina)
{
    Nod *aux = (*radacina)->dreapta;
    (*radacina)->dreapta = aux->stanga;
    aux->stanga = *radacina;
    *radacina = aux;
}

void rotireLaDreapta(Nod **radacina)
{
    Nod *aux = (*radacina)->stanga;
    (*radacina)->stanga = aux->dreapta;
    aux->dreapta = *radacina;
    *radacina = aux;
}

int calculDezechilibru(Nod *radacina)
{
    if (radacina)
    {
        return calculInaltime(radacina->stanga) - calculInaltime(radacina->dreapta);
    }
    else
    {
        return 0;
    }
}

void inserareInAVL(Cofetarie c, Nod **radacina)
{
    if (*radacina == NULL)
    {
        Nod *nou = (Nod *)malloc(sizeof(Nod));
        nou->info = c;
        nou->dreapta = NULL;
        nou->stanga = NULL;
        *radacina = nou;
    }
    else
    {
        if (c.id > (*radacina)->info.id)
            inserareArbore(c, &(*radacina)->dreapta);
        else
            inserareArbore(c, &(*radacina)->stanga);
    }

    int dezechilibru = calculDezechilibru(*radacina);
    if (dezechilibru == -2)
    {
        // dezechilibru pe partea dreapta
        if (calculDezechilibru((*radacina)->dreapta == 1))
            rotireLaDreapta(&(*radacina)->dreapta);
        rotireLaStanga(radacina);
    }
    if (dezechilibru == 2)
    {
        // dezechilibru pe partea stanga
        if (calculDezechilibru((*radacina)->stanga == -1))
            rotireLaStanga(&(*radacina)->stanga);
        rotireLaDreapta(radacina);
    }
}

void main()
{
    Nod *radacina = NULL;
    inserareInAVL(initializareCofetarie(5, "Ana"), &radacina);
    inserareInAVL(initializareCofetarie(4, "Amalia"), &radacina);
    inserareInAVL(initializareCofetarie(7, "Andreea"), &radacina);
    inserareInAVL(initializareCofetarie(6, "Ina"), &radacina);
    inserareInAVL(initializareCofetarie(9, "Raluca"), &radacina);
    inserareInAVL(initializareCofetarie(8, "Anna"), &radacina);

    afisareInOrdine(radacina);
    afisarePreOrdine(radacina);
}
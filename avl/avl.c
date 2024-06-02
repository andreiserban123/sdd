#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include <string.h>

typedef struct Carte Carte;
typedef struct nod nod;

struct Carte
{
    int id;
    char *nume;
    int nrPagini;
};

struct nod
{
    Carte info;
    nod *st;
    nod *dr;
};

Carte initializare(int id, char *nume, int nrPagini)
{
    Carte carte;
    carte.id = id;
    carte.nrPagini = nrPagini;
    carte.nume = (char *)malloc(sizeof(char) * (strlen(nume) + 1));
    strcpy(carte.nume, nume);
    return carte;
}

void afisareCarte(Carte carte)
{
    printf("\nCartea %s are %d pagini si id-ul %d", carte.nume, carte.nrPagini, carte.id);
}

void parcurgerePreOrdine(nod *radacina)
{
    if (radacina)
    {
        afisareCarte(radacina->info);
        parcurgerePreOrdine(radacina->st);
        parcurgerePreOrdine(radacina->dr);
    }
}
int calculInaltime(nod *rad)
{
    if (rad != NULL)
    {
        int st = calculInaltime(rad->st);
        int dr = calculInaltime(rad->dr);

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

int diferentaInaltime(nod *rad)
{
    if (rad != NULL)
    {
        return calculInaltime(rad->st) - calculInaltime(rad->dr);
    }
    else
    {
        return 0;
    }
}

void rotireLaDreapta(nod **rad)
{

    nod *aux = (*rad)->st;
    (*rad)->st = aux->dr;
    aux->dr = *rad;
    *rad = aux;
}
void rotireLaStanga(nod **rad)
{
    nod *aux = (*rad)->dr;
    (*rad)->dr = aux->st;
    aux->st = *rad;
    *rad = aux;
}

nod *findMin(nod *rad)
{
    while (rad->st)
    {
        rad = rad->st;
    }
    return rad;
}

nod *stergeDupaId(nod *rad, int id)
{
    if (rad == NULL)
        return rad;

    if ((rad)->info.id == id)
    {

        if ((rad)->st == NULL)
        {

            nod *tmp = rad->dr;
            free(rad->info.nume);
            free(rad);
            return tmp;
        }
        else if ((rad)->dr == NULL)
        {
            nod *tmp = rad->st;
            free(rad->info.nume);
            free(rad);
            return tmp;
        }
        else
        {
            nod *aux = findMin(rad->dr);
            free(rad->info.nume);

            rad->info = aux->info;
            rad->info.nume = malloc(strlen(aux->info.nume) + 1);
            strcpy(rad->info.nume, aux->info.nume);

            rad->dr = stergeDupaId(rad->dr, aux->info.id);
        }
    }
    else if (id < rad->info.id)
    {
        rad->st = stergeDupaId(rad->st, id);
    }
    else
    {
        rad->dr = stergeDupaId(rad->dr, id);
    }
    return rad;
}

void insertABC(nod **rad, Carte c)
{
    if (*rad == NULL)
    {
        nod *nou = malloc(sizeof(nod));
        nou->info = c;
        nou->st = nou->dr = NULL;
        *rad = nou;
    }
    else if (c.id < (*rad)->info.id)
    {
        insertABC(&(*rad)->st, c);
    }
    else
    {
        insertABC(&(*rad)->dr, c);
    }
}

void inserareInArboreAVL(nod **rad, Carte c)
{
    if (*rad == NULL)
    {
        nod *nou = malloc(sizeof(nod));
        nou->info = c;
        nou->st = nou->dr = NULL;
        *rad = nou;
    }
    else if (c.id < (*rad)->info.id)
    {
        inserareInArboreAVL(&(*rad)->st, c);
    }
    else
    {
        inserareInArboreAVL(&(*rad)->dr, c);
    }

    int dif = diferentaInaltime(*rad);
    if (dif == 2)
    {
        // dezechilbru pe st
        if (diferentaInaltime((*rad)->st) == -1)
        {
            rotireLaStanga(&(*rad)->st);
        }
        rotireLaDreapta(rad);
    }
    if (dif == -2)
    {
        // dezechilibru pe dr
        if (diferentaInaltime((*rad)->dr) == 1)
        {
            rotireLaDreapta(&(*rad)->dr);
        }
        rotireLaStanga(rad);
    }
}

void transformare(nod *rad, nod **radAVL)
{
    if (rad)
    {
        transformare(rad->st, radAVL);
        Carte c = rad->info;
        c.nume = malloc(strlen(rad->info.nume) + 1);
        strcpy(c.nume, rad->info.nume);
        inserareInArboreAVL(radAVL, c);
        transformare(rad->dr, radAVL);
    }
}

void dezalocareARB(nod **rad)
{
    if (*rad)
    {
        dezalocareARB(&(*rad)->st);
        dezalocareARB(&(*rad)->dr);

        free((*rad)->info.nume);
        free((*rad));
    }
    *rad = NULL;
}

void main()
{
    nod *radacina = NULL;
    insertABC(&radacina, initializare(1, "Morometii", 260));
    insertABC(&radacina, initializare(2, "Baltagul", 100));
    insertABC(&radacina, initializare(3, "Amintiri din Copilarie", 380));
    insertABC(&radacina, initializare(4, "Ion", 120));
    insertABC(&radacina, initializare(5, "Enigma Otiliei", 300));

    nod *avl = NULL;
    transformare(radacina, &avl);
    avl = stergeDupaId(avl, 2);
    dezalocareARB(&radacina);
    parcurgerePreOrdine(avl);
    dezalocareARB(&avl);
}
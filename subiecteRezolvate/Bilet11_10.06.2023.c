#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct Fotografie
{
    unsigned int idFotografie;
    char *oras;
    char *data;
    float rezolutie;

} Fotografie;

typedef struct nod
{
    Fotografie inf;
    struct nod *st, *dr;

} nod;

void inserare(nod **rad, Fotografie p)
{
    if (*rad == NULL)
    {
        nod *nou = malloc(sizeof(nod));
        nou->inf = p;
        nou->st = nou->dr = NULL;
        *rad = nou;
    }
    else
    {
        if (p.idFotografie < (*rad)->inf.idFotografie)
            inserare(&(*rad)->st, p);
        else
            inserare(&(*rad)->dr, p);
    }
}

void afisareInOrdine(nod *rad)
{
    if (rad)
    {
        afisareInOrdine(rad->st);
        printf("\n id:%u, oras:%s, data:%s, rezolutie:%.2f", rad->inf.idFotografie,
               rad->inf.oras, rad->inf.data, rad->inf.rezolutie);
        afisareInOrdine(rad->dr);
    }
}

void dezalocare(nod **rad)
{
    if (*rad)
    {
        dezalocare(&(*rad)->st);
        dezalocare(&(*rad)->dr);
        free((*rad)->inf.oras);
        free((*rad)->inf.data);
        free(*rad);
    }
    *rad = NULL;
}

void cautaDupaOras(nod *rad, char *nume, int *nr)
{
    if (rad)
    {
        if (strcmp(rad->inf.oras, nume) == 0)
        {
            *nr = *nr + 1;
        }
        cautaDupaOras(rad->st, nume, nr);
        cautaDupaOras(rad->dr, nume, nr);
    }
}

void cautaDupaRez(nod *rad, float rez, int *nr)
{
    if (rad)
    {
        if (rad->inf.rezolutie == rez)
        {
            *nr = *nr + 1;
        }
        cautaDupaRez(rad->st, rez, nr);
        cautaDupaRez(rad->dr, rez, nr);
    }
}

void schimbaData(nod *rad, unsigned int key, char *nouaData)
{
    if (rad)
    {
        if (rad->inf.idFotografie == key)
        {
            free(rad->inf.data);
            rad->inf.data = malloc(strlen(nouaData) + 1);
            strcpy(rad->inf.data, nouaData);
        }
        else
        {
            if (key < rad->inf.idFotografie)
                schimbaData(rad->st, key, nouaData);
            else
                schimbaData(rad->dr, key, nouaData);
        }
    }
}

void stergeRadacina(nod **rad)
{
    if (*rad == NULL)
        return;

    nod *temp;
    // Case 1: No child
    if ((*rad)->st == NULL && (*rad)->dr == NULL)
    {
        free((*rad)->inf.oras);
        free((*rad)->inf.data);
        free(*rad);
        *rad = NULL;
    }
    // Case 2: One child (right child only)
    else if ((*rad)->st == NULL)
    {
        temp = *rad;
        *rad = (*rad)->dr;
        free(temp->inf.oras);
        free(temp->inf.data);
        free(temp);
    }
    // Case 3: One child (left child only)
    else if ((*rad)->dr == NULL)
    {
        temp = *rad;
        *rad = (*rad)->st;
        free(temp->inf.oras);
        free(temp->inf.data);
        free(temp);
    }
    // Case 4: Two children
    else
    {
        nod *parent = NULL;
        nod *successor = (*rad)->dr;

        // Find the inorder successor (smallest in the right subtree)
        while (successor->st != NULL)
        {
            parent = successor;
            successor = successor->st;
        }

        // Replace root's data with successor's data
        free((*rad)->inf.oras);
        free((*rad)->inf.data);
        (*rad)->inf = successor->inf;

        // Fix the successor's parent's child
        if (parent != NULL)
        {
            if (successor->dr != NULL)
                parent->st = successor->dr;
            else
                parent->st = NULL;
        }
        else
        {
            // Special case: the direct right child of the root was the inorder successor
            (*rad)->dr = successor->dr;
        }
        free(successor);
    }
}

void creeareVector(nod *rad, Fotografie *v, float rez, int *i)
{
    if (rad)
    {
        if (rad->inf.rezolutie == rez)
        {
            Fotografie f;
            f = rad->inf;
            f.oras = malloc(strlen(rad->inf.oras) + 1);
            strcpy(f.oras, rad->inf.oras);
            f.data = malloc(strlen(rad->inf.data) + 1);
            strcpy(f.data, rad->inf.data);
            v[(*i)++] = f;
        }
        creeareVector(rad->st, v, rez, i);
        creeareVector(rad->dr, v, rez, i);
    }
}

void main()
{
    int nrPoz;
    nod *rad = NULL;
    FILE *f = fopen("fisier.txt", "r");
    fscanf(f, "%d", &nrPoz);
    char buffer[200];
    Fotografie p;
    for (int i = 0; i < nrPoz; i++)
    {
        fscanf(f, "%u", &p.idFotografie);
        fscanf(f, "%s", buffer);
        p.oras = malloc(strlen(buffer) + 1);
        strcpy(p.oras, buffer);
        fscanf(f, "%s", buffer);
        p.data = malloc(strlen(buffer) + 1);
        strcpy(p.data, buffer);
        fscanf(f, "%f", &p.rezolutie);
        inserare(&rad, p);
    }

    int nr = 0;
    cautaDupaOras(rad, "Cluj", &nr);
    if (nr == 0)
    {
        printf("\nOrasul nu exista in lista de poze!");
    }
    else
    {
        printf("\nExista %d poze in orasul respectiv!", nr);
    }

    schimbaData(rad, 25, "02/06/2003");

    printf("\nBefore deleting the root node:\n");
    afisareInOrdine(rad);

    // Delete the root node
    stergeRadacina(&rad);

    printf("\nAfter deleting the root node:\n");
    afisareInOrdine(rad);

    nr = 0;
    float rez = 400.0;
    cautaDupaRez(rad, rez, &nr);

    if (nr == 0)
    {
        printf("\nNU exista poze cu rezolutia specificata!");
    }
    else
    {
        Fotografie *v = malloc(sizeof(Fotografie) * nr);
        int i = 0;
        creeareVector(rad, v, 400.0, &i);
        printf("\n\nVector:");
        for (int i = 0; i < nr; i++)
        {
            printf("\n id:%u, oras:%s, data:%s, rezolutie:%.2f", v[i].idFotografie,
                   v[i].oras, v[i].data, v[i].rezolutie);
        }
    }

    // Deallocate memory
    dezalocare(&rad);
    afisareInOrdine(rad);

    fclose(f);
}

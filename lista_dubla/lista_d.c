#define _CRT_SECURE_NO_WARNINGS
#include <malloc.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
    int nrMatricol;
    char *nume;
    float medie;
} student;

typedef struct
{
    student inf;
    struct nodLS *next, *prev;
} nodLS;

nodLS *inserare_lista(nodLS *cap, nodLS **coada, student s)
{
    nodLS *nou = malloc(sizeof(nodLS));
    nou->next = nou->prev = NULL;
    nou->inf = s;
    if (cap == NULL || (*coada) == NULL)
    {
        *coada = nou;
        return nou;
    }
    (*coada)->next = nou;
    nou->prev = (*coada);
    *coada = nou;
    return cap;
}

void traversareLista(nodLS *cap)
{
    if (cap != NULL)
    {
        while (cap != NULL)
        {
            printf("\nNR MAT: %d, NUME: %s, MEDIE: %.2f", cap->inf.nrMatricol, cap->inf.nume,
                   cap->inf.medie);
            cap = cap->next;
        }
    }
    else
    {
        printf("\nLISTA ESTE GOALA!");
    }
}

void traversareInversa(nodLS *coada)
{
    if (coada != NULL)
    {
        while (coada != NULL)
        {
            printf("\nNR MAT: %d, NUME: %s, MEDIE: %.2f", coada->inf.nrMatricol, coada->inf.nume,
                   coada->inf.medie);
            coada = coada->prev;
        }
    }
    else
    {
        printf("\nLISTA ESTE GOALA!");
    }
}

student *salvareVector(nodLS *cap, float medie, int *nr)
{
    *nr = 0;
    nodLS *tmp = cap;
    while (tmp)
    {
        if (tmp->inf.medie > medie)
        {
            (*nr)++;
        }
        tmp = tmp->next;
    }
    student *v = malloc(sizeof(student) * (*nr));
    tmp = cap;
    int i = 0;
    while (tmp)
    {
        if (tmp->inf.medie > medie)
        {
            v[i] = tmp->inf;
            v[i].nume = malloc(strlen(tmp->inf.nume) + 1);
            strcpy(v[i].nume, tmp->inf.nume);
            i++;
        }
        tmp = tmp->next;
    }
    return v;
}

void dezalocareLista(nodLS **cap)
{
    while (*cap != NULL)
    {
        nodLS *tmp = *cap;
        *cap = (*cap)->next;
        free(tmp->inf.nume);
        free(tmp);
    }
}

int main()
{
    int nrStud;
    nodLS *cap = NULL, *coada = NULL;
    student s;
    char buffer[20];
    FILE *f = fopen("fisier.txt", "r");

    if (f == NULL)
        return -1;

    fscanf(f, "%d", &nrStud);

    for (int i = 0; i < nrStud; i++)
    {
        fscanf(f, "%d", &s.nrMatricol);
        fscanf(f, "%s", buffer);
        s.nume = malloc(strlen(buffer) + 1);
        strcpy(s.nume, buffer);
        fscanf(f, "%f", &s.medie);
        cap = inserare_lista(cap, &coada, s);
    }
    fclose(f);

    traversareLista(cap);
    printf("\n*******************************\n");
    traversareInversa(coada);

    // salvarea listei intr-un vector de studenti

    printf("\n*******************************\n");

    int nr;
    student *v = salvareVector(cap, 9.5f, &nr);

    for (int i = 0; i < nr; i++)
    {
        printf("\nNR MAT: %d, NUME: %s, MEDIE: %.2f", v[i].nrMatricol, v[i].nume,
               v[i].medie);
        free(v[i].nume);
    }
    free(v);
    v = NULL;
    printf("\n*******************************\n");

    //// dezalocare LISTA
    dezalocareLista(&cap);
    coada = NULL;
    traversareLista(cap);
    traversareInversa(coada);
    return 0;
}

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
    struct nodLS *next;
} nodLS;

nodLS *inserare_lista(nodLS *cap, student s)
{
    nodLS *nou = malloc(sizeof(nodLS));
    nou->inf = s;
    nou->next = NULL;

    if (cap == NULL)
        return nou;
    nodLS *tmp = cap;
    while (tmp->next != NULL)
    {
        tmp = tmp->next;
    }
    tmp->next = nou;
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

int main()
{
    int nrStud;
    nodLS *cap = NULL;
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
        cap = inserare_lista(cap, s);
    }
    fclose(f);

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

    traversareLista(cap);

    // dezalocare LISTA
    dezalocareLista(&cap);
    traversareLista(cap);
    return 0;
}

#include <malloc.h>
#include <stdio.h>
#include <string.h>

typedef struct student
{
    int nrMatricol;
    char *nume;
    float medie;
} student;

typedef struct NodLS
{
    struct NodLS *prev;
    struct NodLS *next;
    student inf;
} NodLS;

NodLS *insertNod(NodLS *cap, student s)
{
    NodLS *nou = malloc(sizeof(NodLS));
    nou->next = NULL;
    nou->prev = NULL;
    nou->inf.nrMatricol = s.nrMatricol;
    nou->inf.nume = malloc(strlen(s.nume) + 1);
    strcpy(nou->inf.nume, s.nume);
    nou->inf.medie = s.medie;
    if (cap == NULL)
    {
        return nou;
    }
    else
    {
        NodLS *tmp = cap;
        while (tmp->next != NULL)
        {
            tmp = tmp->next;
        }
        tmp->next = nou;
        nou->prev = tmp;
        return cap;
    }
}

void traversare(NodLS *cap)
{
    NodLS *tmp = cap;
    while (tmp)
    {
        printf("\nNr. Matricol:%d,  NUME: %s,  MEDIE:%5.2f", tmp->inf.nrMatricol, tmp->inf.nume, tmp->inf.medie);
        tmp = tmp->next;
    }
}

void traversareInversa(NodLS *cap)
{
    NodLS *tmp = cap;
    if (cap == NULL)
    {
        return;
    }
    while (tmp->next != NULL)
    {
        tmp = tmp->next;
    }
    while (tmp != NULL)
    {
        printf("\nNr. Matricol:%d,  NUME: %s,  MEDIE:%5.2f", tmp->inf.nrMatricol, tmp->inf.nume, tmp->inf.medie);
        tmp = tmp->prev;
    }
}

void dezalocareList(NodLS **cap)
{
    NodLS *tmp = *cap;
    while (*cap != NULL)
    {
        *cap = (*cap)->next;
        free(tmp->inf.nume);
        free(tmp);
        tmp = *cap;
    }
}

void salvareVector(NodLS *cap, student *v, int *nrElem)
{
    *nrElem = 0;
    while (cap)
    {
        if (cap->inf.medie > 9.5f)
        {
            student s = cap->inf;
            s.nume = malloc(strlen(cap->inf.nume) + 1);
            strcpy(s.nume, cap->inf.nume);
            v[(*nrElem)++] = s;
        }
        cap = cap->next;
    }
}

int main()
{
    FILE *f = fopen("fisier.txt", "r");
    if (f == NULL)
    {
        printf("Fisier nu exista/nu s-a putut deschide!");
        return 1;
    }
    NodLS *cap = NULL;
    int nr;
    fscanf(f, "%d", &nr);
    for (int i = 0; i < nr; i++)
    {
        student s;
        fscanf(f, "%d", &s.nrMatricol);
        char buffer[200];
        fscanf(f, " %[^\n]", buffer);
        s.nume = malloc(strlen(buffer) + 1);
        strcpy(s.nume, buffer);
        fscanf(f, "%f", &s.medie);
        cap = insertNod(cap, s);
    }
    fclose(f);
    printf("TRAVERSARE :");
    traversare(cap);
    printf("\nTRAVERSARE INVERSA :");
    traversareInversa(cap);

    // pune studentii cu nota peste 9.5 intr-un vector
    student *vect = malloc(sizeof(student) * nr);
    int nrElem;
    salvareVector(cap, vect, &nrElem);
    printf("\nVECTOR CU STUD CU MED PESTE 9.5");
    for (int i = 0; i < nrElem; i++)
    {
        printf("\nNr. Matricol:%d,  NUME: %s,  MEDIE:%5.2f", vect[i].nrMatricol, vect[i].nume, vect[i].medie);
    }

    for (int i = 0; i < nrElem; i++)
    {
        free(vect[i].nume);
    }
    free(vect);

    dezalocareList(&cap);
    traversare(cap);
    return 0;
}
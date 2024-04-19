#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct student
{
    int nrMatricol;
    char *nume;
    float medie;
} student;

typedef struct nodls
{
    student info;
    struct nodls *next, *prev;
} nodls;

nodls *inserare(nodls *cap, nodls **coada, student s)
{
    nodls *nou = malloc(sizeof(nodls));
    nou->info.nume = malloc(strlen(s.nume) + 1);
    strcpy(nou->info.nume, s.nume);
    nou->info.medie = s.medie;
    nou->info.nrMatricol = s.nrMatricol;
    nou->next = NULL;
    nou->prev = NULL;
    if (cap == NULL)
    {
        cap = nou;
        *coada = nou;
    }
    else
    {
    }
    return cap;
}

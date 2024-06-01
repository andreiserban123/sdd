#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

#define terminat 1
#define neterminat 0
typedef struct
{
    int id;
    char *numeProiect;
    int status;
} Proiect;

typedef struct
{
    Proiect inf;
    struct nod *next;

} nod;

typedef struct
{
    nod **vect;
    int nrEl;
} hashT;

typedef struct
{
    struct nodABC *st, *dr;
    Proiect inf;
} nodABC;

int hash(char *cheie, hashT tab)
{
    return cheie[0] % tab.nrEl;
}

nod *insertLS(nod *cap, Proiect p)
{
    nod *nou = malloc(sizeof(nod));
    nou->inf = p;
    nou->next = NULL;
    if (cap == NULL)
        return nou;
    nod *tmp = cap;
    while (tmp->next)
    {
        tmp = tmp->next;
    }
    tmp->next = nou;
    return cap;
}

void insertHashT(hashT *tab, Proiect p)
{
    int poz = hash(p.numeProiect, *tab);
    tab->vect[poz] = insertLS(tab->vect[poz], p);
}

void afisHashT(hashT tab)
{
    for (int i = 0; i < tab.nrEl; i++)
    {
        if (tab.vect[i])
        {
            printf("\nPoz %d:\n", i);
            nod *cap = tab.vect[i];
            while (cap)
            {
                printf("\nID:%d, Nume:%s, STATUS:%d", cap->inf.id, cap->inf.numeProiect,
                       cap->inf.status);
                cap = cap->next;
            }
        }
    }
}

int eliminare(hashT *tab, char *key)
{
    int poz = hash(key, *tab);
    nod *cap = tab->vect[poz];
    nod *tmp = cap;
    nod *prev = cap;
    int ok = 0;
    while (tmp)
    {
        if (strcmp(tmp->inf.numeProiect, key) == 0)
        {
            ok = 1;
            if (tmp == cap)
            {
                cap = cap->next;
                free(tmp->inf.numeProiect);
                free(tmp);
                tmp = cap;
            }
            else
            {
                prev->next = tmp->next;
                free(tmp->inf.numeProiect);
                free(tmp);
                tmp = prev->next;
            }
        }
        else
        {
            prev = tmp;
            tmp = tmp->next;
        }
    }
    tab->vect[poz] = cap;
    return ok;
}

void modif(hashT *tab, char *oldKey, char *newKey)
{
    Proiect p;
    int poz = hash(oldKey, *tab);

    nod *tmp = tab->vect[poz];
    while (tmp)
    {
        if (strcmp(tmp->inf.numeProiect, oldKey) == 0)
        {
            p = tmp->inf;
        }
        tmp = tmp->next;
    }
    eliminare(tab, oldKey);
    p.numeProiect = malloc(strlen(newKey) + 1);
    strcpy(p.numeProiect, newKey);
    insertHashT(tab, p);
}

void insertABC(nodABC **rad, Proiect p)
{
    if (*rad == NULL)
    {
        nodABC *nou = malloc(sizeof(nodABC));
        nou->st = nou->dr = NULL;
        nou->inf = p;
        nou->inf.numeProiect = (char *)malloc(sizeof(char) * (strlen(p.numeProiect) + 1));
        strcpy(nou->inf.numeProiect, p.numeProiect);
        *rad = nou;
    }
    else
    {
        if ((*rad)->inf.id > p.id)
            insertABC(&(*rad)->st, p);
        else
            insertABC(&(*rad)->dr, p);
    }
}

void afisareInOrdine(nodABC *rad)
{
    if (rad)
    {
        afisareInOrdine(rad->st);
        printf("\nID:%d, nume:%s , status:%d", rad->inf.id,
               rad->inf.numeProiect, rad->inf.status);
        afisareInOrdine(rad->dr);
    }
}

void proiecteTerminate(nodABC *rad, int *count)
{
    if (rad)
    {
        if (rad->inf.status == 1)
        {
            (*count)++;
        }
        proiecteTerminate(rad->st, count);
        proiecteTerminate(rad->dr, count);
    }
}

void detProiecteUltimulNivel(nodABC *rad)
{
    if (rad)
    {
        if (rad->dr == NULL && rad->st == NULL)
        {
            printf("\nID:%d, nume:%s , status:%d", rad->inf.id,
                   rad->inf.numeProiect, rad->inf.status);
        }
        detProiecteUltimulNivel(rad->st);
        detProiecteUltimulNivel(rad->dr);
    }
}

void dezalocareHashT(hashT *tab)
{
    for (int i = 0; i < tab->nrEl; i++)
    {
        if (tab->vect[i])
        {
            nod *cap = tab->vect[i];
            while (cap)
            {
                nod *tmp = cap;
                cap = cap->next;
                free(tmp->inf.numeProiect);
                free(tmp);
            }
            tab->vect[i] = cap;
        }
    }
    free(tab->vect);
    tab->vect = NULL;
    tab->nrEl = 0;
}

void dezalocareABC(nodABC **rad)
{
    if (*rad)
    {
        dezalocareABC(&(*rad)->st);
        dezalocareABC(&(*rad)->dr);
        free((*rad)->inf.numeProiect);
        free(*rad);
    }
    *rad = NULL;
}

int main()
{
    FILE *f = fopen("fisier.txt", "r");
    hashT tab;
    tab.nrEl = 20;
    tab.vect = malloc(sizeof(nod *) * tab.nrEl);
    for (int i = 0; i < tab.nrEl; i++)
        tab.vect[i] = NULL;
    Proiect p;
    char buffer[50];
    while (fscanf(f, "%d", &p.id) == 1)
    {
        fscanf(f, " %[^\n]", buffer);
        p.numeProiect = malloc(strlen(buffer) + 1);
        strcpy(p.numeProiect, buffer);
        fscanf(f, "%d", &p.status);
        insertHashT(&tab, p);
    }
    // afisHashT(tab);
    int ok = eliminare(&tab, "microsoft");
    if (ok)
    {
        printf("\n A fost eliminat!\n");
        afisHashT(tab);
    }
    else
    {
        printf("\n Nu exista acest proiect!");
    }

    modif(&tab, "discord", "deeznuts");
    printf("\n\n");
    afisHashT(tab);

    nodABC *rad = NULL;

    for (int i = 0; i < tab.nrEl; i++)
    {
        if (tab.vect[i])
        {
            nod *cap = tab.vect[i];
            while (cap)
            {
                insertABC(&rad, cap->inf);
                cap = cap->next;
            }
        }
    }

    printf("\n\n\n Afisare ABC:");
    afisareInOrdine(rad);

    // schimb putin cerinta
    // determinarea valorii totale de proiecte terminate din arborele binar

    int count = 0;
    proiecteTerminate(rad, &count);
    printf("\n\nPROIECTE TERMINATE: %d", count);

    printf("\n\nPROIECTE ultimul nivel");
    detProiecteUltimulNivel(rad);

    dezalocareHashT(&tab);
    dezalocareABC(&rad);

    return 0;
}
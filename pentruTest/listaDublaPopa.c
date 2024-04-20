#include <stdio.h>
#include <string.h>
#include <malloc.h>

typedef struct cont_bancar
{
    char iban[25];
    char moneda[4];
    char *titular;
    float sold;
} cont_bancar;

typedef struct nod
{
    struct nod *next;
    struct nod *prev;
    cont_bancar inf;
} nod;

typedef struct IBAN
{
    char iban[25];
} IBAN;

nod *inserare(nod *cap, cont_bancar b)
{
    nod *nou = malloc(sizeof(nod));
    nou->inf = b;
    nou->next = NULL;
    nou->prev = NULL;
    if (cap == NULL)
    {
        return nou;
    }
    nod *tmp = cap;
    while (tmp->next != NULL)
    {
        tmp = tmp->next;
    }
    tmp->next = nou;
    nou->prev = tmp;
    return cap;
}

void dezalocareLista(nod **cap)
{
    while (*cap)
    {
        nod *tmp = *cap;
        *cap = (*cap)->next;
        free(tmp->inf.titular);
        free(tmp);
    }
}

void parcurgere(nod *cap)
{
    while (cap)
    {
        printf("\n%s\n%s\n%s\n%5.2f", cap->inf.iban, cap->inf.titular,
               cap->inf.moneda, cap->inf.sold);
        cap = cap->next;
    }
}

// salvare conturi bancare cu aceeasi moneda
// conturile bancare sunt stocate in lista dubla
// functia intoarce vectorul: elemente de tip IBAN

IBAN *salvareConturi(nod **cap, int nr, char *currency, int *n)
{
    *n = 0;
    IBAN *v = malloc(sizeof(IBAN) * nr);
    nod *tmp = *cap;
    nod *prev = NULL;

    while (tmp)
    {
        nod *next = tmp->next;
        if (strcmp(tmp->inf.moneda, currency) == 0)
        {
            strcpy(v[(*n)++].iban, tmp->inf.iban);
            if (tmp == *cap)
            {
                *cap = next;
                if (*cap)
                    (*cap)->prev = NULL;
            }
            else
            {
                if (prev)
                    prev->next = next;
                if (next)
                    next->prev = prev;
            }
            free(tmp->inf.titular);
            free(tmp);
        }
        else
        {
            prev = tmp;
        }
        tmp = next;
    }
    return v;
}

int main()
{
    int nr;
    nod *cap = NULL;
    FILE *f = fopen("Conturi.txt", "r");
    if (f == NULL)
    {
        printf("Fisierul nu s-a deschis");
        return 1;
    }
    fscanf(f, "%d", &nr);
    for (int i = 0; i < nr; i++)
    {
        cont_bancar c;
        fscanf(f, "%s", c.iban);
        char buffer[200];
        fscanf(f, " %[^\n]", buffer);
        c.titular = malloc(strlen(buffer) + 1);
        strcpy(c.titular, buffer);
        fscanf(f, "%s", c.moneda);
        fscanf(f, "%f", &c.sold);
        cap = inserare(cap, c);
    }
    fclose(f);
    parcurgere(cap);
    int nrElemVector;
    IBAN *v = salvareConturi(&cap, nr, "RON", &nrElemVector);
    printf("\nAFISARE VECTOR:");
    for (int i = 0; i < nrElemVector; i++)
    {
        printf("\n%s ", v[i].iban);
    }
    free(v);
    printf("\n  ------ AFISARE LISTA ------ \n");
    parcurgere(cap);
    dezalocareLista(&cap);
    return 0;
}
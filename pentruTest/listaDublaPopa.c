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
    nod *tmp = *cap;
    IBAN *v = malloc(sizeof(IBAN) * nr);

    while (tmp != NULL)
    {
        if (strcmp(tmp->inf.moneda, currency) == 0)
        {
            strcpy(v[(*n)++].iban, tmp->inf.iban);
            if (tmp->prev == NULL)
            {
                (*cap) = (*cap)->next;
                if ((*cap))
                    (*cap)->prev = NULL;
                free(tmp->inf.titular);
                free(tmp);
                tmp = *cap;
            }
            else if (tmp->next == NULL)
            {
                nod *p = tmp->prev;
                p->next = NULL;
                free(tmp->inf.titular);
                free(tmp);
                tmp = p;
            }
            else
            {
                nod *prev = tmp->prev;
                nod *next = tmp->next;
                free(tmp->inf.titular);
                free(tmp);
                prev->next = next;
                next->prev = prev;
                tmp = next;
            }
        }
        else
        {
            tmp = tmp->next;
        }
    }
    return v;
}

void adaugare(nod **cap, float sold)
{
    nod *tmp = *cap;

    while (tmp != NULL)
    {
        if (tmp->inf.sold >= sold)
        {
            nod *nou = malloc(sizeof(nod));
            nou->inf.titular = malloc(strlen(tmp->inf.titular) + 1);
            strcpy(nou->inf.titular, tmp->inf.titular);
            strcpy(nou->inf.iban, "INVALID");
            strcpy(nou->inf.moneda, "N/A"); // Assuming currency is not relevant for the new node
            nou->inf.sold = 0;              // Assuming 0 as the default sold for the new node
            nou->next = NULL;
            nou->prev = NULL;
            // cap
            if (tmp->prev == NULL)
            {
                nod *temp = tmp;
                tmp = tmp->next;
                free(temp->inf.titular);
                free(temp);
                *cap = nou;
                (*cap)->next = tmp;
                if (tmp)
                    tmp->prev = (*cap);
            }
            // coada
            else if (tmp->next == NULL)
            {
                nod *p = NULL;
                p = tmp->prev;
                free(tmp->inf.titular);
                free(tmp);
                p->next = nou;
                nou->prev = p;
                tmp = NULL;
            }
            else
            {
                nod *prev = tmp->prev;
                nod *next = tmp->next;
                free(tmp->inf.titular);
                free(tmp);
                prev->next = nou;
                nou->prev = prev;
                nou->next = next;
                next->prev = nou;
                tmp = nou;
            }
        }
        else
        {
            tmp = tmp->next;
        }
    }
}

// interschimbare noduri
void sortare(nod **cap)
{
    int sortat = 0;
    do
    {
        sortat = 0;
        nod *tmp = *cap;
        while (tmp->next != NULL)
        {
            if (tmp->inf.sold > tmp->next->inf.sold)
            {
                sortat = 1;
                nod *p = tmp->next;
                tmp->next = p->next;
                p->prev = tmp->prev;

                if (tmp->prev != NULL)
                {
                    tmp->prev->next = p;
                }
                else
                {
                    *cap = p;
                }

                if (p->next != NULL)
                {
                    p->next->prev = tmp;
                }

                p->next = tmp;
                tmp->prev = p;

                // Adjust 'tmp' to point to the node before 'p' to continue sorting from the correct position
                tmp = p;
            }
            tmp = tmp->next;
        }
    } while (sortat);
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

    // int nrElemVector;
    // IBAN* v = salvareConturi(&cap, nr, "RON", &nrElemVector);
    // printf("\nAFISARE VECTOR:");
    // for (int i = 0; i < nrElemVector; i++)
    //{
    //	printf("\n%s ", v[i].iban);
    // }
    // free(v);
    // printf("\n  ------ AFISARE LISTA ------ \n");
    sortare(&cap);
    parcurgere(cap);
    dezalocareLista(&cap);
    return 0;
}
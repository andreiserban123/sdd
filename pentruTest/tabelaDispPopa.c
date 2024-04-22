#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct cont_bancar
{
    char iban[25];
    char *titular;
    char moneda[4];
    float sold;
} cont_bancar;

typedef struct nod
{
    struct nod *next;
    cont_bancar inf;
} nod;

typedef struct hashT
{
    int nrElem;
    nod **v;

} hashT;

nod *inserare(nod *cap, cont_bancar c)
{

    nod *nou = malloc(sizeof(nod));
    nou->inf = c;
    nou->next = NULL;

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
    return cap;
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

void dezalocare(nod **cap)
{
    while (*cap)
    {
        nod *tmp = *cap;
        *cap = (*cap)->next;
        free(tmp->inf.titular);
        free(tmp);
    }
}
int functieHash(int cod, hashT tabela)
{
    return cod % tabela.nrElem;
}

void parcurgereT(hashT tabela)
{
    for (int i = 0; i < tabela.nrElem; i++)
    {
        if (tabela.v[i])
        {
            printf("\nPOZ %d", i);
            parcurgere(tabela.v[i]);
        }
    }
}

void dezalocareT(hashT table)
{
    for (int i = 0; i < table.nrElem; i++)
    {
        if (table.v[i])
        {
            dezalocare(&table.v[i]);
        }
    }
}

void stergere_conturi_bancare(hashT *tab, const char *titular)
{
    for (int i = 0; i < tab->nrElem; i++)
    {
        nod *current = tab->v[i];
        nod *previous = NULL;

        while (current != NULL)
        {
            if (strcmp(titular, current->inf.titular) == 0)
            {
                if (previous == NULL)
                {
                    tab->v[i] = current->next;
                }
                else
                {

                    previous->next = current->next;
                }
                nod *nextNode = current->next;
                free(current->inf.titular);
                free(current);
                current = nextNode;
            }
            else
            {
                previous = current;
                current = current->next;
            }
        }
    }
}

int main()
{
    int nr;
    FILE *f = fopen("Conturi.txt", "r");
    if (f == NULL)
    {
        printf("Fisierul nu s-a deschis");
        return 1;
    }
    hashT tabela;
    tabela.nrElem = 10;
    tabela.v = malloc(sizeof(nod *) * tabela.nrElem);

    for (int i = 0; i < tabela.nrElem; i++)
    {
        tabela.v[i] = NULL;
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
        int cod = c.moneda[0] + c.moneda[1] + c.moneda[2];
        int poz = functieHash(cod, tabela);
        tabela.v[poz] = inserare(tabela.v[poz], c);
    }

    stergere_conturi_bancare(&tabela, "Popescu Iulian");

    parcurgereT(tabela);
    dezalocareT(tabela);
    free(tabela.v);

    fclose(f);
}
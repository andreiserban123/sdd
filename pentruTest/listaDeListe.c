#include <stdio.h>
#include <string.h>
#include <malloc.h>

typedef struct cont
{
    char iban[25];
    char *titular;
    char moneda[4];
    float sold;
} cont;

typedef struct nod
{
    struct nod *next;
    cont inf;
} nod;

typedef struct hastT
{
    nod **vect;
    int nrElem;
} hashT;

typedef struct nodP
{
    struct nodP *next;
    nod *inf;
} nodP;

int functieHash(int cod, hashT tabela)
{
    return cod % tabela.nrElem;
}

nod *inserareLS(nod *cap, cont b)
{
    nod *nou = malloc(sizeof(nod));
    nou->inf = b;
    nou->inf.titular = malloc(strlen(b.titular) + 1);
    strcpy(nou->inf.titular, b.titular);
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

nodP *inserareLP(nodP *cap, nod *capLS)
{

    nodP *nou = malloc(sizeof(nodP));
    nou->inf = capLS;
    nou->next = NULL;
    if (cap == NULL)
    {
        return nou;
    }

    nodP *tmp = cap;
    while (tmp->next)
    {
        tmp = tmp->next;
    }
    tmp->next = nou;
    return cap;
}

void traversareLS(nod *cap)
{
    while (cap)
    {
        printf("\n%s\n%s\n%s\n%5.2f", cap->inf.iban, cap->inf.titular,
               cap->inf.moneda, cap->inf.sold);
        cap = cap->next;
    }
}

void dezalocareLS(nod **cap)
{
    while (*cap)
    {
        nod *tmp = *cap;
        *cap = (*cap)->next;
        free(tmp->inf.titular);
        free(tmp);
    }
}

void inserareTabela(hashT tabela, cont c)
{
    int cod = c.moneda[0] + c.moneda[1] + c.moneda[2];
    int poz = functieHash(cod, tabela);
    tabela.vect[poz] = inserareLS(tabela.vect[poz], c);
}

void parcurgereTabela(hashT tabela)
{
    for (int i = 0; i < tabela.nrElem; i++)
    {
        if (tabela.vect[i])
        {
            printf("\nPozitia %d", i);
            traversareLS(tabela.vect[i]);
        }
    }
}

void dezalocareTabela(hashT tabela)
{
    for (int i = 0; i < tabela.nrElem; i++)
    {
        if (tabela.vect[i])
        {
            dezalocareLS(&tabela.vect[i]);
        }
    }
}

int main()
{
    FILE *f = fopen("Conturi.txt", "r");
    if (f == NULL)
        return 1;
    int nr;
    hashT tabela;
    tabela.nrElem = 23;
    tabela.vect = malloc(sizeof(nod *) * tabela.nrElem);
    for (int i = 0; i < tabela.nrElem; i++)
    {
        tabela.vect[i] = NULL;
    }

    fscanf(f, "%d", &nr);
    for (int i = 0; i < nr; i++)
    {
        cont c;
        fscanf(f, "%s", c.iban);
        char buffer[200];
        fscanf(f, " %[^\n]", buffer);
        c.titular = malloc(strlen(buffer) + 1);
        strcpy(c.titular, buffer);
        fscanf(f, "%s", c.moneda);
        fscanf(f, "%f", &c.sold);
        inserareTabela(tabela, c);
        free(c.titular);
    }
    // parcurgereTabela(tabela);
    nodP *cap = NULL;
    for (int i = 0; i < tabela.nrElem; i++)
    {
        if (tabela.vect[i])
        {
            cap = inserareLP(cap, tabela.vect[i]);
        }
    }
    nodP *tmp = cap;
    while (tmp)
    {
        printf("\nLISTA:");
        traversareLS(tmp->inf);
        tmp = tmp->next;
    }
    dezalocareTabela(tabela);
    free(tabela.vect);
    fclose(f);
    return 0;
}
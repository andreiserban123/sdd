#include <stdio.h>
#include <string.h>
#include <malloc.h>

typedef struct produs
{
    int cod;
    char *denumire;
    float pret;
} produs;

typedef struct
{
    produs inf;
    struct nodLS *next;
} nodLS;

typedef struct
{
    struct nodLS **vect;
    int nrElem;
} hashT;

int functiaHash(int cheie, hashT tabela)
{
    return cheie % tabela.nrElem;
}

nodLS *inserare(nodLS *cap, produs p)
{
    nodLS *nou = malloc(sizeof(nodLS));
    nou->inf.cod = p.cod;
    nou->inf.pret = p.pret;
    nou->inf.denumire = malloc(strlen(p.denumire) + 1);
    strcpy(nou->inf.denumire, p.denumire);
    nou->next = NULL;
    if (cap == NULL)
    {
        return nou;
    }
    nodLS *tmp = cap;
    while (tmp->next != NULL)
    {
        tmp = tmp->next;
    }
    tmp->next = nou;
    return cap;
}

void afisareLista(nodLS *cap)
{
    while (cap)
    {
        printf("\nCod: %d, Denumire: %s, Pret: %5.2f", cap->inf.cod, cap->inf.denumire,
               cap->inf.pret);
        cap = cap->next;
    }
}
void dezalocareLista(nodLS **cap)
{
    while (*cap)
    {
        nodLS *tmp = *cap;
        *cap = (*cap)->next;
        free(tmp->inf.denumire);
        free(tmp);
    }
    *cap = NULL;
}

void parcurgereTabela(hashT tabela)
{
    for (int i = 0; i < tabela.nrElem; i++)
    {
        if (tabela.vect[i])
        {
            printf("\nPOZITIA: %d", i);
            afisareLista(tabela.vect[i]);
        }
    }
}

void dezalocareTabela(hashT tabela)
{
    for (int i = 0; i < tabela.nrElem; i++)
    {
        if (tabela.vect[i])
            dezalocareLista(&tabela.vect[i]);
    }
}

int main()
{
    hashT tabela;
    tabela.nrElem = 23;
    tabela.vect = malloc(tabela.nrElem * sizeof(nodLS *));
    for (int i = 0; i < tabela.nrElem; i++)
    {
        tabela.vect[i] = NULL;
    }
    FILE *f = fopen("fisier.txt", "r");
    int nr;
    fscanf(f, "%d", &nr);
    for (int i = 0; i < nr; i++)
    {
        produs p;
        fscanf(f, "%d", &p.cod);
        char buffer[200];
        fscanf(f, " %[^\n]", buffer);
        p.denumire = malloc(strlen(buffer) + 1);
        strcpy(p.denumire, buffer);
        fscanf(f, "%f", &p.pret);
        int poz = functiaHash(p.cod, tabela);
        tabela.vect[poz] = inserare(tabela.vect[poz], p);
        free(p.denumire);
    }
    fclose(f);
    parcurgereTabela(tabela);
    dezalocareTabela(tabela);
    free(tabela.vect);
    return 0;
}

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

typedef struct TITULAR
{
    char *titular;
} TITULAR;

typedef struct hashT
{
    nod **vect;
    int nrElem;
} hashT;

nod *adaugareNod(nod *cap, cont b)
{
    nod *nou = malloc(sizeof(nod));
    nou->inf = b;
    nou->next = NULL;
    if (cap == NULL)
        return nou;
    nod *tmp = cap;
    while (tmp->next != NULL)
    {
        tmp = tmp->next;
    }
    tmp->next = nou;
}

void parcurgereLista(nod *cap)
{
    while (cap)
    {
        printf("\n%s\n%s\n%s\n%.2f", cap->inf.iban, cap->inf.titular,
               cap->inf.moneda, cap->inf.sold);
        cap = cap->next;
    }
}

int funcHash(cont b, hashT tabela)
{
    int cod = b.moneda[0] + b.moneda[1] + b.moneda[2];
    return cod % tabela.nrElem;
}

void adaugareTabela(hashT tabela, cont c)
{
    int poz = funcHash(c, tabela);
    tabela.vect[poz] = adaugareNod(tabela.vect[poz], c);
}
void parcurgereTabela(hashT tb)
{
    for (int i = 0; i < tb.nrElem; i++)
    {
        if (tb.vect[i])
        {
            printf("\nPoz %d", i);
            parcurgereLista(tb.vect[i]);
        }
    }
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

void dezalocreTabela(hashT tabela)
{
    for (int i = 0; i < tabela.nrElem; i++)
    {
        if (tabela.vect[i] != NULL)
        {
            dezalocareLista(&tabela.vect[i]);
        }
    }
    free(tabela.vect);
}

void extrage(hashT tab, float *titular)
{
    for (int i = 0; i < tab.nrElem; i++)
    {
        if (tab.vect[i] != NULL)
        {
            nod *tmp = tab.vect[i];
            nod *p = NULL;
            while (tmp != NULL)
            {
                if (strcmp(titular, tmp->inf.titular) == 0)
                {
                    p = tmp;
                    tmp = tmp->next;
                    if (p == tab.vect[i])
                    {
                        tab.vect[i] = tmp;
                        free(p->inf.titular);
                        free(p);
                    }
                    else
                    {
                        p->next = tmp;
                        free(p->inf.titular);
                        free(p);
                    }
                }
                else
                {
                    tmp = tmp->next;
                }
            }
        }
    }
}

int main()
{
    FILE *f = fopen("Conturi.txt", "r");
    if (f == NULL)
    {
        printf("NU SE POATE DESCHIDE FIS!");
        return 1;
    }

    hashT tabela;
    tabela.nrElem = 10;
    tabela.vect = malloc(sizeof(nod *) * tabela.nrElem);
    for (int i = 0; i < tabela.nrElem; i++)
    {
        tabela.vect[i] = NULL;
    }
    int nr;
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
        adaugareTabela(tabela, c);
    }
    parcurgereTabela(tabela);

    // sa se extrage numele titularilor CONTURILOR CARE AU UN SOLD MAI MIC DECAT CEL DAT
    extrage(tabela, "Ionescu Georgica");

    // DUPA EXTRAGERE
    printf("\n DUPA EXTRAGERE\n");
    parcurgereTabela(tabela);
    dezalocreTabela(tabela);
    return 0;
}
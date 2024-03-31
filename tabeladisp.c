#include <stdio.h>
#include <malloc.h>
#include <string.h>
typedef struct
{
    int cod;
    char *denumire;
    float pret;
} produs;

typedef struct nodLS
{
    produs inf;
    struct nodLS *next;
} nodLS;

typedef struct
{
    struct nodLS **vect;
    int nrElem;
} hashT;

int functieHash(int cheie, hashT tabela)
{
    return cheie % tabela.nrElem;
}

void inserareHash(hashT tabela, produs p)
{
    int poz = functieHash(p.cod, tabela);
    nodLS *nou = malloc(sizeof(nodLS));
    nou->next = NULL;
    nou->inf.cod = p.cod;
    nou->inf.denumire = malloc(strlen(p.denumire) + 1);
    strcpy(nou->inf.denumire, p.denumire);
    nou->inf.pret = p.pret;
    if (tabela.vect[poz] == NULL)
    {
        tabela.vect[poz] = nou;
    }
    else
    {

        nou->next = tabela.vect[poz];
        tabela.vect[poz] = nou;
    }
}
void dezalocareLS(nodLS *capLS)
{
    nodLS *temp = capLS;
    while (temp)
    {
        nodLS *temp2 = temp->next;
        free(temp->inf.denumire);
        free(temp);
        temp = temp2;
    }
}
void traversareHash(hashT tabela)
{
    for (int i = 0; i < tabela.nrElem; i++)
    {
        if (tabela.vect[i] != NULL)
        {
            printf("Pe poz %d sunt:\n", i);
            nodLS *cap = tabela.vect[i];
            while (cap != NULL)
            {
                printf("    Codul:%d, Denumire:%s, Pret:%.2f\n", cap->inf.cod, cap->inf.denumire, cap->inf.pret);
                cap = cap->next;
            }
        }
    }
}

void dezalocareHashT(hashT tabela)
{
    for (int i = 0; i < tabela.nrElem; i++)
    {
        if (tabela.vect[i] != NULL)
        {
            dezalocareLS(tabela.vect[i]);
        }
    }
    free(tabela.vect);
}

int main(int argc, char const *argv[])
{
    hashT tabela;
    tabela.nrElem = 23;
    tabela.vect = malloc(tabela.nrElem * sizeof(nodLS *));
    for (int i = 0; i < tabela.nrElem; i++)
    {
        tabela.vect[i] = NULL;
    }
    int nrProd;
    char buffer[20];
    produs p;
    FILE *f = fopen("produse.txt", "r");
    if (f == NULL)
    {
        free(tabela.vect);
        printf("CANNOT OPEN THE FILE!");
        return 1;
    }
    fscanf(f, "%d", &nrProd);
    for (int i = 0; i < nrProd; i++)
    {
        fscanf(f, "%d", &p.cod);
        fscanf(f, "%s", buffer);
        p.denumire = malloc(strlen(buffer) + 1);
        strcpy(p.denumire, buffer);
        fscanf(f, "%f", &p.pret);
        inserareHash(tabela, p);
        free(p.denumire);
        p.denumire = NULL;
    }
    traversareHash(tabela);
    dezalocareHashT(tabela);
    return 0;
}

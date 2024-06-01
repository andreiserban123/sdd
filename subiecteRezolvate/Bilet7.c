#define _CRT_SECURE_NO_WARNINGS
#include <malloc.h>
#include <string.h>
#include <stdio.h>

typedef struct
{
    unsigned int nrVoucher;
    char *numeBeneficiar;
    char *dataExp;
    float valoare;
} Voucher;

typedef struct
{
    struct nod *next, *prev;
    Voucher inf;
} nod;

nod *inserare(nod *prim, nod **ultim, Voucher v)
{
    nod *nou = malloc(sizeof(nod));
    nou->inf = v;
    nou->next = nou->prev = NULL;
    if (prim == NULL || *ultim == NULL)
    {
        *ultim = nou;
        return nou;
    }

    (*ultim)->next = nou;
    nou->prev = *ultim;
    *ultim = nou;
    return prim;
}

void afisareLD(nod *prim)
{
    while (prim)
    {
        printf("\nNr. Voucher: %u, Data Expirare: %s", prim->inf.nrVoucher, prim->inf.dataExp);
        prim = prim->next;
    }
}

void afisareInvers(nod *prim)
{
    while (prim)
    {
        printf("\nNr. Voucher: %u, Data Expirare: %s", prim->inf.nrVoucher, prim->inf.dataExp);
        prim = prim->prev;
    }
}

float getVal(nod *prim, char *luna)
{
    float c = 0;
    while (prim)
    {
        if (prim->inf.dataExp[3] == luna[0] && prim->inf.dataExp[4] == luna[1])
        {
            c += prim->inf.valoare;
        }
        prim = prim->next;
    }
    if (c == 0)
        return -1;
    return c;
}

Voucher *salveazaVector(nod *prim, int nr, int *n, int prag)
{
    Voucher *v = malloc(sizeof(Voucher) * nr);
    int k = 0;
    while (prim)
    {
        if (prim->inf.valoare > prag)
        {
            v[k] = prim->inf;
            v[k].numeBeneficiar = malloc(strlen(prim->inf.numeBeneficiar) + 1);
            strcpy(v[k].numeBeneficiar, prim->inf.numeBeneficiar);
            k++;
        }
        prim = prim->next;
    }
    *n = k;

    for (int i = 0; i < k - 1; i++)
    {
        for (int j = i + 1; j < k; j++)
        {
            if (v[i].valoare < v[j].valoare)
            {
                Voucher aux = v[i];
                v[i] = v[j];
                v[j] = aux;
            }
        }
    }

    return v;
}

int nr_beneficiari(nod **prim, nod **ultim, int n)
{
    int nr = 0;
    nod *l1 = *prim;
    char **titulari = malloc(sizeof(char *) * n);
    for (int i = 0; i < n; i++)
    {
        titulari[i] = NULL;
    }
    while (l1)
    {
        nod *l2 = l1->next;
        while (l2)
        {
            if (strcmp(l1->inf.numeBeneficiar, l2->inf.numeBeneficiar) == 0)
            {
                int ok = 0;
                for (int i = 0; i < n; i++)
                {
                    if (titulari[i] != NULL && strcmp(l1->inf.numeBeneficiar, titulari[i]) == 0)
                    {
                        ok = 1;
                        i = n;
                    }
                }
                if (ok == 0)
                {
                    titulari[nr] = malloc(strlen(l1->inf.numeBeneficiar) + 1);
                    strcpy(titulari[nr], l1->inf.numeBeneficiar);
                    nr++;
                }
                break;
            }
            l2 = l2->next;
        }
        l1 = l1->next;
    }
    for (int i = 0; i < n; i++)
    {
        if (titulari[i])
        {
            free(titulari[i]);
        }
    }
    free(titulari);
    return nr;
}

void dezalocareLD(nod **prim, nod **ultim)
{
    while (*prim)
    {
        nod *aux = *prim;
        *prim = (*prim)->next;
        free(aux->inf.numeBeneficiar);
        free(aux);
    }
    *ultim = NULL;
}

int main()
{
    FILE *f = fopen("fisier.txt", "r");
    char buffer[20];
    Voucher v;
    nod *prim, *ultim;
    prim = ultim = NULL;
    int nr = 0;
    while (fscanf(f, "%u", &v.nrVoucher) == 1)
    {
        fscanf(f, " %[^\n]", buffer);
        v.numeBeneficiar = malloc(strlen(buffer) + 1);
        strcpy(v.numeBeneficiar, buffer);
        fscanf(f, " %[^\n]", buffer);
        v.dataExp = malloc(strlen(buffer) + 1);
        strcpy(v.dataExp, buffer);
        fscanf(f, "%f", &v.valoare);
        prim = inserare(prim, &ultim, v);
        nr++;
    }
    fclose(f);
    afisareLD(prim);
    printf("\nINVERS:");
    afisareInvers(ultim);

    float val = getVal(prim, "07");
    if (val != -1)
        printf("\n\nValoare comenzi in luna 7: %.2f", val);

    printf("\nNumar de titulari dublati:%d", nr_beneficiari(&prim, &ultim, nr));

    int n;
    Voucher *vect = salveazaVector(prim, nr, &n, 50);
    printf("\n\n");
    for (int i = 0; i < n; i++)
    {
        printf("\nNr. Voucher: %u, Valoare %.2f", vect[i].nrVoucher, vect[i].valoare);
        free(vect[i].numeBeneficiar);
    }
    free(vect);

    dezalocareLD(&prim, &ultim);
    afisareLD(prim);
    afisareInvers(ultim);
    return 0;
}
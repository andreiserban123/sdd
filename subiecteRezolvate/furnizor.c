#define _CRT_SECURE_NO_WARNINGS
#include <malloc.h>
#include <string.h>
#include <stdio.h>

typedef struct
{
    int codClient;
    char *denumireFurnizor;
    float sumaPlata;
    char *dataScadenta;
} Furnizor;

typedef struct nod
{
    Furnizor inf;
    struct nod *next;
} nod;

typedef struct nodLP
{
    struct nodLP *next;
    nod *capLS;
} nodLP;

typedef struct
{
    nod **vect;
    int nrEl;
} hashT;

int hash(hashT *tab, char *key)
{
    return key[0] % tab->nrEl;
}

void afisLS(nod *cap)
{
    while (cap)
    {
        printf("\nCOD:%d , Furnizor:%s , suma:%.2f, data:%s", cap->inf.codClient,
               cap->inf.denumireFurnizor, cap->inf.sumaPlata, cap->inf.dataScadenta);
        cap = cap->next;
    }
}

nod *insertLS(nod *cap, Furnizor f)
{
    nod *nou = (nod *)malloc(sizeof(nod));
    nou->inf = f;
    nou->inf.denumireFurnizor = (char *)malloc(strlen(f.denumireFurnizor) + 1);
    strcpy(nou->inf.denumireFurnizor, f.denumireFurnizor);
    nou->inf.dataScadenta = (char *)malloc(strlen(f.dataScadenta) + 1);
    strcpy(nou->inf.dataScadenta, f.dataScadenta);
    nou->next = NULL;
    if (cap == NULL)
    {
        return nou;
    }
    nod *tmp = cap;
    while (tmp->next)
    {
        tmp = tmp->next;
    }
    tmp->next = nou;
    return cap;
}

void insertHash(hashT *tab, Furnizor f)
{
    int poz = hash(tab, f.denumireFurnizor);
    tab->vect[poz] = insertLS(tab->vect[poz], f);
}

void afisHash(hashT tab)
{
    for (int i = 0; i < tab.nrEl; i++)
    {
        if (tab.vect[i])
        {
            printf("\nPoz %d:", i);
            nod *cap = tab.vect[i];
            while (cap)
            {
                printf("\nCOD: %d, Furnizor:%s, Suma:%.2f, dataScadenta:%s", cap->inf.codClient,
                       cap->inf.denumireFurnizor, cap->inf.sumaPlata, cap->inf.dataScadenta);
                cap = cap->next;
            }
        }
    }
}

int nrFacturi(hashT tab, char *dat)
{
    int counter = 0;
    for (int i = 0; i < tab.nrEl; i++)
    {
        if (tab.vect[i])
        {
            nod *cap = tab.vect[i];
            while (cap)
            {
                if (strcmp(cap->inf.dataScadenta, dat) == 0)
                {
                    counter++;
                }
                cap = cap->next;
            }
        }
    }
    return counter;
}

nodLP *inserareLP(hashT tab, nodLP *cap, int prag)
{
    for (int i = 0; i < tab.nrEl; i++)
    {
        if (tab.vect[i])
        {
            nod *tmp = tab.vect[i];
            while (tmp)
            {
                if (tmp->inf.sumaPlata > prag)
                {
                    if (cap == NULL)
                    {
                        nodLP *nou = (nodLP *)malloc(sizeof(nodLP));
                        nou->next = NULL;
                        nou->capLS = NULL;
                        nou->capLS = insertLS(nou->capLS, tmp->inf);
                        cap = nou;
                    }
                    else
                    {
                        nodLP *tmp2 = cap;
                        int ok = 0;
                        while (tmp2)
                        {
                            if (strcmp(tmp2->capLS->inf.denumireFurnizor, tmp->inf.denumireFurnizor) == 0)
                            {
                                ok = 1;
                                tmp2->capLS = insertLS(tmp2->capLS, tmp->inf);
                                break;
                            }
                            tmp2 = tmp2->next;
                        }
                        if (ok == 0)
                        {
                            nodLP *nou = (nodLP *)malloc(sizeof(nodLP));
                            nou->next = NULL;
                            nou->capLS = NULL;
                            nou->capLS = insertLS(nou->capLS, tmp->inf);

                            nodLP *tmp2 = cap;
                            while (tmp2->next)
                            {
                                tmp2 = tmp2->next;
                            }
                            tmp2->next = nou;
                        }
                    }
                }
                tmp = tmp->next;
            }
        }
    }
    return cap;
}

nodLP *stergeMax(nodLP *capLP)
{
    nodLP *tmpLP = capLP;
    float max = -1;

    while (tmpLP)
    {
        nod *cap = tmpLP->capLS;
        while (cap)
        {
            if (cap->inf.sumaPlata > max)
            {
                max = cap->inf.sumaPlata;
            }
            cap = cap->next;
        }
        tmpLP = tmpLP->next;
    }

    tmpLP = capLP;
    nodLP *prevLP = NULL;

    while (tmpLP)
    {
        nod *tmpLS = tmpLP->capLS;
        nod *prevLS = NULL;

        while (tmpLS)
        {
            if (tmpLS->inf.sumaPlata == max)
            {
                if (prevLS == NULL)
                {
                    tmpLP->capLS = tmpLS->next;
                    free(tmpLS->inf.denumireFurnizor);
                    free(tmpLS->inf.dataScadenta);
                    free(tmpLS);
                    tmpLS = tmpLP->capLS;
                }
                else
                {
                    prevLS->next = tmpLS->next;
                    free(tmpLS->inf.denumireFurnizor);
                    free(tmpLS->inf.dataScadenta);
                    free(tmpLS);
                    tmpLS = prevLS->next;
                }
            }
            else
            {
                prevLS = tmpLS;
                tmpLS = tmpLS->next;
            }
        }

        if (tmpLP->capLS == NULL)
        {
            if (prevLP == NULL)
            {
                capLP = tmpLP->next;
                free(tmpLP);
                tmpLP = capLP;
            }
            else
            {
                prevLP->next = tmpLP->next;
                free(tmpLP);
                tmpLP = prevLP->next;
            }
        }
        else
        {
            prevLP = tmpLP;
            tmpLP = tmpLP->next;
        }
    }

    return capLP;
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
                free(tmp->inf.denumireFurnizor);
                free(tmp->inf.dataScadenta);
                free(tmp);
            }
        }
    }
    free(tab->vect);
    tab->vect = NULL;
    tab->nrEl = 0;
}

void dezalocareLP(nodLP *cap)
{
    while (cap)
    {
        nodLP *tmpLP = cap;
        nod *capLS = cap->capLS;
        while (capLS)
        {
            nod *tmpLS = capLS;
            capLS = capLS->next;
            free(tmpLS->inf.denumireFurnizor);
            free(tmpLS->inf.dataScadenta);
            free(tmpLS);
        }
        cap = cap->next;
        free(tmpLP);
    }
}

int main()
{
    FILE *f = fopen("fisier.txt", "r");
    if (!f)
    {
        printf("Error opening file\n");
        return 1;
    }

    hashT tab;
    tab.nrEl = 20;
    tab.vect = (nod **)malloc(sizeof(nod *) * tab.nrEl);
    for (int i = 0; i < tab.nrEl; i++)
    {
        tab.vect[i] = NULL;
    }

    char buffer[200];
    Furnizor fur;
    while (fscanf(f, "%d", &fur.codClient) == 1)
    {
        fscanf(f, " %[^\n]", buffer);
        fur.denumireFurnizor = (char *)malloc(strlen(buffer) + 1);
        strcpy(fur.denumireFurnizor, buffer);
        fscanf(f, "%f", &fur.sumaPlata);
        fscanf(f, " %[^\n]", buffer);
        fur.dataScadenta = (char *)malloc(strlen(buffer) + 1);
        strcpy(fur.dataScadenta, buffer);
        insertHash(&tab, fur);
        free(fur.dataScadenta);
        free(fur.denumireFurnizor);
    }
    fclose(f);
    afisHash(tab);

    int counter = nrFacturi(tab, "06/06/2026");
    if (counter == 0)
    {
        printf("\nNU exista data scadenta in factura");
    }
    else
    {
        printf("\nExista %d facuturi cu data precizata", counter);
    }

    nodLP *cap = NULL;
    cap = inserareLP(tab, cap, 2000);

    nodLP *tmp = cap;
    while (tmp)
    {
        printf("\nFurnizor %s:", tmp->capLS->inf.denumireFurnizor);
        afisLS(tmp->capLS);
        tmp = tmp->next;
    }
    cap = stergeMax(cap);

    printf("\n\n DUPA STERGERE!");
    tmp = cap;
    while (tmp)
    {
        printf("\nFurnizor %s:", tmp->capLS->inf.denumireFurnizor);
        afisLS(tmp->capLS);
        tmp = tmp->next;
    }

    dezalocareHashT(&tab);
    dezalocareLP(cap);

    return 0;
}

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct vagon
{
    int numarVangon;
    char *firmaTransport;
    int nrBileteVandute;
    int capacitate;
} vagon;

typedef struct nod
{
    vagon inf;
    struct nod *next, *prev;
} nod;

typedef struct heap
{
    vagon *vect;
    int nrElem;
} heap;

nod *inserare(nod *prim, nod **ultim, vagon v)
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

void afisCap(nod *prim)
{
    while (prim)
    {
        printf("\nNr vagon: %d , firma: %s, bilete:%d , capacitate:%d",
               prim->inf.numarVangon, prim->inf.firmaTransport,
               prim->inf.nrBileteVandute, prim->inf.capacitate);
        prim = prim->next;
    }
}
void afisCoada(nod *prim)
{
    while (prim)
    {
        printf("\nNr vagon: %d , firma: %s, bilete:%d , capacitate:%d",
               prim->inf.numarVangon, prim->inf.firmaTransport,
               prim->inf.nrBileteVandute, prim->inf.capacitate);
        prim = prim->prev;
    }
}

void dezalocare(nod **prim, nod **ultim)
{
    while (*prim)
    {
        nod *tmp = (*prim);
        *prim = (*prim)->next;
        free(tmp->inf.firmaTransport);
        free(tmp);
    }

    *ultim = NULL;
}

nod *stergeVag(nod *prim, nod **ultim)
{
    int min = 0;
    if (prim)
    {
        min = prim->inf.nrBileteVandute;
    }
    nod *tmp = prim;
    while (tmp)
    {
        if (tmp->inf.nrBileteVandute < min)
        {
            min = tmp->inf.nrBileteVandute;
        }
        tmp = tmp->next;
    }
    tmp = prim;
    while (tmp)
    {
        if (tmp->inf.nrBileteVandute == min)
        {
            if (tmp == prim)
            {
                prim = prim->next;
                if (prim)
                    prim->prev = NULL;
                free(tmp->inf.firmaTransport);
                free(tmp);
                tmp = prim;
            }
            else if (tmp == *ultim)
            {
                *ultim = (*ultim)->prev;
                if (*ultim)
                    (*ultim)->next = NULL;
                free(tmp->inf.firmaTransport);
                free(tmp);
                tmp = *ultim;
            }
            else
            {
                nod *prev = tmp->prev;
                nod *next = tmp->next;
                prev->next = tmp->next;
                next->prev = prev;
                free(tmp->inf.firmaTransport);
                free(tmp);
                tmp = next;
            }
        }
        else
        {
            tmp = tmp->next;
        }
    }
    return prim;
}

int getNumberofElements(nod *prim)
{
    int nr = 0;
    while (prim)
    {
        nr++;
        prim = prim->next;
    }
    return nr;
}

void filtrare(heap h, int index)
{
    int indexRad = index;
    int indexSt = 2 * index + 1;
    int indexDr = 2 * index + 2;
    if (indexSt < h.nrElem)
    {
        float st = (h.vect[indexSt].nrBileteVandute) / (float)h.vect[indexSt].capacitate;
        float rad = h.vect[indexRad].nrBileteVandute / (float)h.vect[indexRad].capacitate;
        if (st > rad)
        {
            indexRad = indexSt;
        }
    }
    if (indexDr < h.nrElem)
    {
        float dr = h.vect[indexDr].nrBileteVandute / (float)h.vect[indexDr].capacitate;
        float rad = h.vect[indexRad].nrBileteVandute / (float)h.vect[indexRad].capacitate;
        if (dr > rad)
        {
            indexRad = indexDr;
        }
    }
    if (indexRad != index)
    {
        vagon tmp = h.vect[index];
        h.vect[index] = h.vect[indexRad];
        h.vect[indexRad] = tmp;
        filtrare(h, indexRad);
    }
}

int main()
{
    nod *prim = NULL, *ultim = NULL;
    FILE *f = fopen("fisier.txt", "r");
    if (!f)
    {
        printf("Error opening file\n");
        return 1;
    }
    int nrVag;
    fscanf(f, "%d", &nrVag);
    char buffer[200];
    vagon v;
    for (int i = 0; i < nrVag; i++)
    {
        fscanf(f, "%d", &v.numarVangon);
        fscanf(f, "%s", buffer);
        v.firmaTransport = malloc(strlen(buffer) + 1);
        strcpy(v.firmaTransport, buffer);
        fscanf(f, "%d", &v.nrBileteVandute);
        fscanf(f, "%d", &v.capacitate);
        prim = inserare(prim, &ultim, v);
    }
    fclose(f);

    afisCap(prim);
    printf("\n\nDe la coada!");
    afisCoada(ultim);
    printf("\n\n");

    prim = stergeVag(prim, &ultim);
    afisCap(prim);

    int nrElem = getNumberofElements(prim);
    heap h;
    h.nrElem = nrElem;
    h.vect = malloc(sizeof(vagon) * nrElem);

    nod *tmp = prim;
    int i = 0;
    while (tmp)
    {
        h.vect[i] = tmp->inf;
        h.vect[i].firmaTransport = malloc(strlen(tmp->inf.firmaTransport) + 1);
        strcpy(h.vect[i].firmaTransport, tmp->inf.firmaTransport);
        tmp = tmp->next;
        i++;
    }
    for (int i = (h.nrElem - 1) / 2; i >= 0; i--)
    {
        filtrare(h, i);
    }

    printf("\n\nHeap:\n");
    for (int i = 0; i < h.nrElem; i++)
    {
        printf("\nNr vagon: %d , firma: %s, bilete:%d , capacitate:%d",
               h.vect[i].numarVangon, h.vect[i].firmaTransport,
               h.vect[i].nrBileteVandute, h.vect[i].capacitate);
    }

    // Free the allocated memory for the heap
    for (int i = 0; i < h.nrElem; i++)
    {
        free(h.vect[i].firmaTransport);
    }
    free(h.vect);

    dezalocare(&prim, &ultim);

    return 0;
}

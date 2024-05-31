#define _CRT_SECURE_NO_WARNINGS
#include <malloc.h>
#include <string.h>
#include <stdio.h>

typedef struct
{
    int cod;
    char *firmaTransport;
    int numarProduseComandate;
    float sumaPlata;
} comanda;

typedef struct
{
    comanda inf;
    struct nod *next;
} nod;

typedef struct
{
    comanda *vect;
    int nrElem;
} heap;

void filtrare(int index, heap h)
{
    int pozRad = index;
    int st = index * 2 + 1;
    int dr = index * 2 + 2;

    if (st < h.nrElem && h.vect[pozRad].sumaPlata < h.vect[st].sumaPlata)
        pozRad = st;

    if (dr < h.nrElem && h.vect[pozRad].sumaPlata < h.vect[dr].sumaPlata)
        pozRad = dr;

    if (pozRad != index)
    {
        comanda aux = h.vect[index];
        h.vect[index] = h.vect[pozRad];
        h.vect[pozRad] = aux;
    }
}

nod *put(nod *prim, nod **ultim, comanda c)
{
    nod *nou = malloc(sizeof(nod));
    nou->inf = c;
    if (prim == NULL || (*ultim == NULL))
    {
        *ultim = nou;
        return nou;
    }

    (*ultim)->next = nou;
    nou->next = NULL;
    *ultim = nou;
    return prim;
}

int get(nod **prim, nod **ultim, comanda *c)
{
    if (*prim != NULL)
    {
        *c = (*prim)->inf;

        (*c).firmaTransport = malloc(strlen((*prim)->inf.firmaTransport) + 1);
        strcpy((*c).firmaTransport, (*prim)->inf.firmaTransport);
        nod *tmp = *prim;
        (*prim) = (*prim)->next;
        free(tmp->inf.firmaTransport);
        free(tmp);
        return 1;
    }
    else
    {
        *ultim = NULL;
        return 0;
    }
}

void afisare(nod *prim)
{
    while (prim)
    {
        printf("Cod: %d\nFirma transport: %s\nNumar produse comandate: % d\nSuma plata : % 5.2f\n\n", prim->inf.cod, prim->inf.firmaTransport, prim->inf.numarProduseComandate, prim->inf.sumaPlata);
        prim = prim->next;
    }
}

comanda *salveazaVector(nod **prim, nod **ultim, int *nr, int elem, char *nume)
{
    int i = 0;
    comanda c;
    comanda *v = malloc(sizeof(comanda) * elem);

    while (get(prim, ultim, &c))
    {
        if (strcmp(c.firmaTransport, nume) == 0)
        {
            v[i++] = c;
        }
    }
    *nr = i;
    return v;
}

heap salveazaCoada(comanda **v, int n)
{
    heap h;
    h.vect = malloc(sizeof(comanda) * n);

    for (int i = 0; i < n; i++)
    {
        h.vect[i] = (*v)[i];
    }
    free(*v);
    *v = NULL;
    h.nrElem = n;
    for (int i = (n - 1) / 2; i >= 0; i--)
        filtrare(i, h);
    return h;
}

heap extragere(heap h, comanda *c)
{
    comanda *vectNOU = malloc(sizeof(comanda) * (h.nrElem - 1));

    comanda aux = h.vect[0];
    h.vect[0] = h.vect[h.nrElem - 1];
    h.vect[h.nrElem - 1] = aux;
    *c = h.vect[h.nrElem - 1];
    h.nrElem--;

    for (int i = 0; i < h.nrElem; i++)
    {
        vectNOU[i] = h.vect[i];
    }
    h.vect = vectNOU;

    for (int i = (h.nrElem - 1) / 2; i >= 0; i--)
    {
        filtrare(i, h);
    }
    return h;
}

int main()
{
    FILE *f = fopen("fisier.txt", "r");
    char buffer[50];
    nod *prim = NULL, *ultim = NULL;
    comanda c;
    while (fscanf(f, "%d", &c.cod) == 1)
    {
        fscanf(f, " %[^\n]", buffer);
        c.firmaTransport = malloc(strlen(buffer) + 1);
        strcpy(c.firmaTransport, buffer);
        fscanf(f, "%d", &c.numarProduseComandate);
        fscanf(f, "%f", &c.sumaPlata);
        prim = put(prim, &ultim, c);
    }
    fclose(f);
    while (get(&prim, &ultim, &c))
    {
        printf("Cod: %d\nFirma transport: %s\nNumar produse comandate: %d\nSuma plata: %5.2f\n\n", c.cod, c.firmaTransport, c.numarProduseComandate, c.sumaPlata);
        free(c.firmaTransport);
    }

    f = fopen("fisier.txt", "r");
    int nr = 0;
    while (fscanf(f, "%d", &c.cod) == 1)
    {
        fscanf(f, " %[^\n]", buffer);
        c.firmaTransport = malloc(strlen(buffer) + 1);
        strcpy(c.firmaTransport, buffer);
        fscanf(f, "%d", &c.numarProduseComandate);
        fscanf(f, "%f", &c.sumaPlata);
        prim = put(prim, &ultim, c);
        nr++;
    }
    fclose(f);
    printf("\n\n");

    int n;
    comanda *v = salveazaVector(&prim, &ultim, &n, nr, "Rinocer Trans");
    for (int i = 0; i < n; i++)
    {
        printf("Cod: %d\nFirma transport: %s\nNumar produse comandate: %d\nSuma plata: %5.2f\n\n", v[i].cod, v[i].firmaTransport, v[i].numarProduseComandate, v[i].sumaPlata);
    }

    heap h = salveazaCoada(&v, n);

    printf("\n\nAfisare HEAP:\n");
    for (int i = 0; i < h.nrElem; i++)
    {
        printf("Cod: %d\nFirma transport: %s\nNumar produse comandate: %d\nSuma plata: %5.2f\n\n", h.vect[i].cod, h.vect[i].firmaTransport, h.vect[i].numarProduseComandate, h.vect[i].sumaPlata);
    }

    printf("\n\nExtragere:\n");
    comanda c1;
    h = extragere(h, &c1);

    printf("Cod: %d\nFirma transport: %s\nNumar produse comandate: %d\nSuma plata: %5.2f\n\n", c1.cod, c1.firmaTransport, c1.numarProduseComandate, c1.sumaPlata);

    h = extragere(h, &c1);
    printf("Cod: %d\nFirma transport: %s\nNumar produse comandate: %d\nSuma plata: %5.2f\n\n", c1.cod, c1.firmaTransport, c1.numarProduseComandate, c1.sumaPlata);

    return 0;
}
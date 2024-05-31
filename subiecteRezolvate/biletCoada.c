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
        printf("Cod: %d\nFirma transport: %s\nNumar produse comandate: %d\nSuma plata: %5.2f\n\n", prim->inf.cod, prim->inf.firmaTransport, prim->inf.numarProduseComandate, prim->inf.sumaPlata);
        prim = prim->next;
    }
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

    comanda *v = malloc(sizeof(comanda) * nr);
    int i = 0;
    while (get(&prim, &ultim, &c))
    {
        // if (strcmp())
        v[i++] = c;
    }
    int n = i;
    for (int i = 0; i < n; i++)
    {
        printf("Cod: %d\nFirma transport: %s\nNumar produse comandate: %d\nSuma plata: %5.2f\n\n", v[i].cod, v[i].firmaTransport, v[i].numarProduseComandate, v[i].sumaPlata);
        free(v[i].firmaTransport);
    }
    free(v);
    return 0;
}
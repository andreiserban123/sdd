
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct
{
    char *tip_camera;
    int etaj;
    int numar_camera;
    float pret_noapte;
} Camera;

typedef struct
{
    struct nodABC *st, *dr;
    Camera inf;
} nodABC;

typedef struct
{
    struct nodLD *next, *prev;
    Camera inf;
} nodLD;

void insertABC(nodABC **rad, Camera c)
{
    if (*rad == NULL)
    {
        nodABC *nou = malloc(sizeof(nodABC));
        nou->inf = c;
        nou->st = nou->dr = NULL;
        *rad = nou;
    }
    else if (c.numar_camera < (*rad)->inf.numar_camera)
    {
        insertABC(&(*rad)->st, c);
    }
    else
    {
        insertABC(&(*rad)->dr, c);
    }
}

void nr_camere_per_etaj(nodABC *rad, int *n, int **v)
{
    if (rad)
    {
        ((*v)[rad->inf.etaj])++;
        if (*n < rad->inf.etaj)
            *n = rad->inf.etaj;
        nr_camere_per_etaj(rad->st, n, v);
        nr_camere_per_etaj(rad->dr, n, v);
    }
}

nodLD *insertLD(nodLD *cap, nodLD **ultim, Camera c)
{
    nodLD *nou = malloc(sizeof(nodLD));
    nou->inf = c;
    nou->inf.tip_camera = malloc(strlen(c.tip_camera) + 1);
    strcpy(nou->inf.tip_camera, c.tip_camera);

    nou->next = NULL;
    nou->prev = NULL;
    if (cap == NULL || *ultim == NULL)
    {
        *ultim = nou;
        return nou;
    }

    (*ultim)->next = nou;
    nou->prev = *ultim;
    *ultim = nou;
    return cap;
}

void conversie(nodABC *rad, nodLD **prim, nodLD **ultim, int etaj)
{
    if (rad)
    {
        if (rad->inf.etaj == etaj)
        {
            *prim = insertLD(*prim, ultim, rad->inf);
        }
        conversie(rad->st, prim, ultim, etaj);
        conversie(rad->dr, prim, ultim, etaj);
    }
}

void insertDupaCamera(nodLD **cap, nodLD **ultim, int nrCamera, Camera c)
{
    nodLD *tmp = *cap;
    while (tmp)
    {
        if (tmp->inf.numar_camera == nrCamera)
        {
            nodLD *nou = malloc(sizeof(nodLD));
            nou->inf = c;
            nodLD *next = tmp->next;
            nou->next = NULL;
            tmp->next = nou;
            nou->prev = tmp;
            if (next)
            {
                nou->next = next;
                next->prev = nou;
            }
            else
            {
                *ultim = nou;
                tmp = nou;
            }
            return;
        }
        tmp = tmp->next;
    }
}

void dezalocareARB(nodABC **rad)
{
    if (*rad)
    {
        dezalocareARB(&(*rad)->st);
        dezalocareARB(&(*rad)->dr);
        free((*rad)->inf.tip_camera);
        free(*rad);
    }
    *rad = NULL;
}

void dezalocareLISTA(nodLD **prim, nodLD **ultim)
{
    nodLD *tmp;
    while (*prim)
    {
        tmp = *prim;
        *prim = (*prim)->next;
        free(tmp->inf.tip_camera);
        free(tmp);
    }
    *ultim = NULL;
}

int main()
{
    FILE *f = fopen("fisier.txt", "r");
    if (!f)
    {
        printf("Eroare deschidere fisier!\n");
        return 1;
    }
    char buffer[100];
    nodABC *rad = NULL;
    Camera c;
    int nr = 0;
    while (fscanf(f, "%s %d %d %f", buffer, &c.etaj, &c.numar_camera, &c.pret_noapte) == 4)
    {
        c.tip_camera = malloc(strlen(buffer) + 1);
        strcpy(c.tip_camera, buffer);
        nr++;
        insertABC(&rad, c);
    }
    fclose(f);

    int *etaje = malloc(sizeof(int) * nr);
    for (int i = 0; i < nr; i++)
    {
        etaje[i] = 0;
    }
    nr = 0;
    nr_camere_per_etaj(rad, &nr, &etaje);
    for (int i = 1; i <= nr; i++)
    {
        printf("\nLA ETAJUL %d avem %d camere", i, etaje[i]);
    }
    free(etaje);
    etaje = NULL;

    // salveaza intr-o lista dubla camerele de la etaj 1

    nodLD *prim, *ultim;
    prim = ultim = NULL;
    conversie(rad, &prim, &ultim, 1);

    nodLD *tmp = prim;
    while (tmp)
    {
        printf("\n%s %d %d %5.2f", tmp->inf.tip_camera, tmp->inf.etaj, tmp->inf.numar_camera, tmp->inf.pret_noapte);
        tmp = tmp->next;
    }

    Camera camera1;
    camera1.tip_camera = malloc(strlen("VIP") + 1);
    strcpy(camera1.tip_camera, "VIP");
    camera1.etaj = 1;
    camera1.numar_camera = 112;
    camera1.pret_noapte = 150.0;
    insertDupaCamera(&prim, &ultim, 101, camera1);
    printf("\n\nDUPA INSERARE:");
    tmp = prim;
    while (tmp)
    {
        printf("\n%s %d %d %5.2f", tmp->inf.tip_camera, tmp->inf.etaj, tmp->inf.numar_camera, tmp->inf.pret_noapte);
        tmp = tmp->next;
    }

    printf("\n\n");
    tmp = ultim;
    while (tmp)
    {
        printf("\n%s %d %d %5.2f", tmp->inf.tip_camera, tmp->inf.etaj, tmp->inf.numar_camera, tmp->inf.pret_noapte);
        tmp = tmp->prev;
    }

    dezalocareARB(&rad);
    dezalocareLISTA(&prim, &ultim);
    return 0;
}
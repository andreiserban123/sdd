#define _CRT_SECURE_NO_WARNINGS
#include <malloc.h>
#include <string.h>
#include <stdio.h>

typedef struct
{
    char *dataConsultatie;
    char *numeMedic;
    char *specialitate;
    char *diagnostic;
    int pret;
} consulatie;

typedef struct
{
    struct nodLS *next;
    consulatie inf;
} nodLS;

typedef struct
{
    struct nodLD *next;
    nodLS *capLS;
} nodLD;

nodLS *inserareLS(nodLS *cap, consulatie c)
{
    nodLS *nou = malloc(sizeof(nodLS));
    nou->inf = c;
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
nodLD *inserareLD(nodLD *capLD, consulatie c)
{
    if (capLD == NULL)
    {
        nodLD *nou = malloc(sizeof(nodLD));
        nou->next = NULL;
        nou->capLS = NULL;
        nou->capLS = inserareLS(nou->capLS, c);
        return nou;
    }
    nodLD *tmp = capLD;

    while (tmp)
    {
        if (strcmp(tmp->capLS->inf.specialitate, c.specialitate) == 0)
        {
            tmp->capLS = inserareLS(tmp->capLS, c);
            return capLD;
        }
        tmp = tmp->next;
    }
    tmp = capLD;

    while (tmp->next)
        tmp = tmp->next;

    nodLD *nou = malloc(sizeof(nodLD));
    nou->next = NULL;
    nou->capLS = NULL;
    nou->capLS = inserareLS(nou->capLS, c);
    tmp->next = nou;

    return capLD;
}

void afisLS(nodLS *cap)
{
    while (cap)
    {
        printf("\n Data:%s, numeMedic:%s, Specialitate:%s, Diagnostic:%s pret:%d", cap->inf.dataConsultatie,
               cap->inf.numeMedic, cap->inf.specialitate, cap->inf.diagnostic, cap->inf.pret);
        cap = cap->next;
    }
}

int valoareSpecializare(nodLD *capLD, char *specializare)
{
    int sum = 0;

    while (capLD)
    {
        if (strcmp(capLD->capLS->inf.specialitate, specializare) == 0)
        {
            nodLS *cap = capLD->capLS;
            while (cap)
            {
                sum += cap->inf.pret;
                cap = cap->next;
            }
            return sum;
        }
        capLD = capLD->next;
    }
    return -1;
}

void modificaConsultatie(nodLD *capLD, char *data, int pret)
{
    while (capLD)
    {
        nodLS *cap = capLD->capLS;
        while (cap)
        {
            if (strcmp(cap->inf.dataConsultatie, data) == 0)
            {
                cap->inf.pret = pret;
                return;
            }
            cap = cap->next;
        }
        capLD = capLD->next;
    }
}

nodLD *stergeDupaDiagnostic(nodLD *capLD, char *diag)
{
    nodLD *tmpLD = capLD;
    nodLD *prevLD = capLD;
    while (tmpLD)
    {
        nodLS *cap = tmpLD->capLS;
        nodLS *tmp = cap;
        nodLS *prev = tmp;
        while (tmp)
        {
            if (strcmp(tmp->inf.diagnostic, diag) == 0)
            {
                if (tmp == cap)
                {
                    cap = cap->next;
                    free(tmp->inf.dataConsultatie);
                    free(tmp->inf.numeMedic);
                    free(tmp->inf.specialitate);
                    free(tmp->inf.diagnostic);
                    free(tmp);
                    tmpLD->capLS = cap;
                    tmp = cap;
                }
                else
                {
                    prev->next = tmp->next;
                    free(tmp->inf.dataConsultatie);
                    free(tmp->inf.numeMedic);
                    free(tmp->inf.specialitate);
                    free(tmp->inf.diagnostic);
                    free(tmp);
                    tmp = prev->next;
                }
            }
            else
            {
                prev = tmp;
                tmp = tmp->next;
            }
        }

        if (tmpLD->capLS == NULL)
        {
            if (tmpLD == capLD)
            {
                capLD = capLD->next;
            }
            else
            {
                prevLD->next = tmpLD->next;
            }
            free(tmpLD);
            tmpLD = prevLD->next;
        }
        else
        {
            prevLD = tmpLD;
            tmpLD = tmpLD->next;
        }
    }
    return capLD;
}

consulatie *createVect(nodLD *capLD, int prag, int nr, int *n)
{
    consulatie *v = malloc(sizeof(consulatie) * nr);

    int k = 0;
    while (capLD)
    {
        nodLS *cap = capLD->capLS;
        while (cap)
        {
            if (cap->inf.pret > prag)
            {
                v[k].dataConsultatie = malloc(strlen(cap->inf.dataConsultatie) + 1);
                strcpy(v[k].dataConsultatie, cap->inf.dataConsultatie);
                v[k].numeMedic = malloc(strlen(cap->inf.numeMedic) + 1);
                strcpy(v[k].numeMedic, cap->inf.numeMedic);
                v[k].specialitate = malloc(strlen(cap->inf.specialitate) + 1);
                strcpy(v[k].specialitate, cap->inf.specialitate);
                v[k].diagnostic = malloc(strlen(cap->inf.diagnostic) + 1);
                strcpy(v[k].diagnostic, cap->inf.diagnostic);
                v[k].pret = cap->inf.pret;
                k++;
            }
            cap = cap->next;
        }
        capLD = capLD->next;
    }
    *n = k;
    return v;
}

nodLS *dezLS(nodLS *cap)
{
    while (cap)
    {
        nodLS *tmp = cap->next;
        free(cap->inf.dataConsultatie);
        free(cap->inf.diagnostic);
        free(cap->inf.numeMedic);
        free(cap->inf.specialitate);
        free(cap);
        cap = tmp;
    }
    return cap;
}

int main()
{
    FILE *f = fopen("fisier.txt", "r");
    consulatie c;
    nodLD *capLD = NULL;
    char *buffer[50];
    int nrCons;
    fscanf(f, "%d", &nrCons);
    for (int i = 0; i < nrCons; i++)
    {

        fscanf(f, "%s", buffer);
        c.dataConsultatie = (char *)malloc(strlen(buffer) + 1);
        strcpy(c.dataConsultatie, buffer);
        fscanf(f, "%s", buffer);
        c.numeMedic = (char *)malloc(strlen(buffer) + 1);
        strcpy(c.numeMedic, buffer);
        fscanf(f, "%s", buffer);
        c.specialitate = (char *)malloc(strlen(buffer) + 1);
        strcpy(c.specialitate, buffer);
        fscanf(f, "%s", buffer);
        c.diagnostic = malloc(strlen(buffer) + 1);
        strcpy(c.diagnostic, buffer);
        fscanf(f, "%d", &c.pret);
        capLD = inserareLD(capLD, c);
    }

    nodLD *tmpLD = capLD;
    while (tmpLD)
    {
        printf("\n Consultatii de la specializarea: %s", tmpLD->capLS->inf.specialitate);
        afisLS(tmpLD->capLS);
        tmpLD = tmpLD->next;
    }

    int pret = valoareSpecializare(capLD, "f");

    if (pret == -1)
    {
        printf("\n Specializarea nu exista!");
    }
    else
    {
        printf("\n Pretul este: %d", pret);
    }

    modificaConsultatie(capLD, "02/06/2003", 9999);

    printf("\n\n");
    tmpLD = capLD;
    while (tmpLD)
    {
        printf("\n Consultatii de la specializarea: %s", tmpLD->capLS->inf.specialitate);
        afisLS(tmpLD->capLS);
        tmpLD = tmpLD->next;
    }
    capLD = stergeDupaDiagnostic(capLD, "Sanatos");

    printf("\n\n");
    tmpLD = capLD;
    while (tmpLD)
    {
        printf("\n Consultatii de la specializarea: %s", tmpLD->capLS->inf.specialitate);
        afisLS(tmpLD->capLS);
        tmpLD = tmpLD->next;
    }
    int n;
    consulatie *v = createVect(capLD, 400, nrCons, &n);

    printf("\n\nVector:");
    for (int i = 0; i < n; i++)
    {
        printf("\n Data:%s, numeMedic:%s, Specialitate:%s, Diagnostic:%s pret:%d", v[i].dataConsultatie,
               v[i].numeMedic, v[i].specialitate, v[i].diagnostic, v[i].pret);
        free(v[i].dataConsultatie);
        free(v[i].numeMedic);
        free(v[i].specialitate);
        free(v[i].diagnostic);
    }

    nodLD *tmp = capLD;
    while (tmp)
    {
        nodLD *aux = tmp->next;
        tmp->capLS = dezLS(tmp->capLS);
        free(tmp);
        tmp = aux;
    }

    return 0;
}
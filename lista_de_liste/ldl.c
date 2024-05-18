#include <malloc.h>
#include <stdio.h>
#include <string.h>

typedef struct produs
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

typedef struct nodLP
{
    nodLS *inf;
    struct nodLP *next;

} nodLP;

nodLS *inserareLS(nodLS *cap, produs p)
{
    nodLS *nou = (nodLS *)malloc(sizeof(nodLS));
    nou->inf = p;
    nou->next = NULL;
    if (cap == NULL)
    {
        cap = nou;
    }
    else
    {
        nodLS *temp = cap;
        while (temp->next)
        {
            temp = temp->next;
        }
        temp->next = nou;
    }
    return cap;
}

void traversareLS(nodLS *cap)
{
    nodLS *temp = cap;
    while (temp)
    {
        printf("Cod: %d, Denumire: %s, Pret: %5.2f\n", temp->inf.cod, temp->inf.denumire, temp->inf.pret);
        temp = temp->next;
    }
}

void traversareLP(nodLP *cap)
{
    nodLP *temp = cap;
    int i = 0;
    while (temp)
    {
        printf("Lista %d:\n", i++);
        traversareLS(temp->inf);
        temp = temp->next;
    }
}

nodLP *inserareLP(nodLP *cap, nodLS *l)
{
    nodLP *nou = malloc(sizeof(nodLP));
    nou->next = NULL;
    nou->inf = l;
    if (cap == NULL)
    {
        return nou;
    }
    nodLP *tmp = cap;
    while (tmp->next)
    {
        tmp = tmp->next;
    }
    tmp->next = nou;
}

void dezalocareLS(nodLS **cap)
{
    while (*cap)
    {
        nodLS *temp = *cap;
        *cap = (*cap)->next;
        free(temp->inf.denumire);
        free(temp);
    }
}
void dezalocareLP(nodLP **cap)
{
    while (*cap)
    {
        nodLP *temp = *cap;
        *cap = (*cap)->next;
        dezalocareLS(&temp->inf);
        free(temp);
    }
}

int main()
{
    nodLP *cap = NULL;
    FILE *f = fopen("fisier.txt", "r");
    nodLS *l1 = NULL;
    nodLS *l2 = NULL;
    int nrEl;
    char buffer[100];
    produs p;
    fscanf(f, "%d", &nrEl);
    for (int i = 0; i < nrEl; i++)
    {
        fscanf(f, "%d", &p.cod);
        fscanf(f, "%s", buffer);
        p.denumire = (char *)malloc((strlen(buffer) + 1) * sizeof(char));
        strcpy(p.denumire, buffer);
        fscanf(f, "%f", &p.pret);
        if (p.cod % 2 == 0)
            l1 = inserareLS(l1, p);
        else
            l2 = inserareLS(l2, p);
    }

    fclose(f);
    cap = inserareLP(cap, l1);
    cap = inserareLP(cap, l2);

    traversareLP(cap);

    dezalocareLP(&cap);
    l1 = NULL;
    l2 = NULL;
    traversareLP(cap);

    return 0;
}
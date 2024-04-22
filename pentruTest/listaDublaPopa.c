#include <stdio.h>
#include <string.h>
#include <malloc.h>

typedef struct ContBancar
{
    char iban[25];
    char *titular;
    char moneda[4];
    float sold;

} ContBancar;

typedef struct Nod
{
    struct Nod *next;
    struct Nod *prev;
    ContBancar inf;
} Nod;

typedef struct ListaDubla
{
    Nod *u, *p;
} ListaDubla;

typedef struct IBAN
{
    char iban[25];
} IBAN;

void adauga(ListaDubla *lista, ContBancar c)
{
    Nod *nou = malloc(sizeof(Nod));
    nou->inf = c;
    nou->next = nou->prev = NULL;
    if (lista->p == NULL)
    {
        lista->p = lista->u = nou;
    }
    else
    {
        lista->u->next = nou;
        nou->prev = lista->u;
        lista->u = nou;
    }
}

void parcurge(ListaDubla l)
{
    printf("\nNORMALA\n");
    while (l.p)
    {
        printf("\nIban: %s, Titular:%s, Moneda:%s, SOLD:%.2f", l.p->inf.iban, l.p->inf.titular,
               l.p->inf.moneda, l.p->inf.sold);
        l.p = l.p->next;
    }
}
void parcurgereInversa(ListaDubla l)
{
    printf("\nINVERSA\n");
    while (l.u)
    {
        printf("\nIban: %s, Titular:%s, Moneda:%s, SOLD:%.2f", l.u->inf.iban, l.u->inf.titular,
               l.u->inf.moneda, l.u->inf.sold);
        l.u = l.u->prev;
    }
}

void dezalocare(ListaDubla *l)
{
    Nod *tmp = l->p;
    while (l->p)
    {
        tmp = l->p;
        l->p = l->p->next;
        free(tmp->inf.titular);
        free(tmp);
    }
    l->u = NULL;
}

IBAN *salvare_iban(ListaDubla *l, int *n, char *currency)
{
    *n = 0;
    Nod *tmp = l->p;
    while (tmp)
    {
        if (strcmp(tmp->inf.moneda, currency) == 0)
        {
            *n = *n + 1;
        }
        tmp = tmp->next;
    }
    IBAN *v = malloc(sizeof(IBAN) * (*n));
    tmp = l->p;
    int i = 0;
    while (tmp)
    {
        Nod *next = tmp->next;
        if (strcmp(tmp->inf.moneda, currency) == 0)
        {
            strcpy(v[i++].iban, tmp->inf.iban);
            if (tmp->prev)
            {
                tmp->prev->next = tmp->next;
            }
            else
            {
                l->p = tmp->next;
            }
            if (tmp->next)
            {
                tmp->next->prev = tmp->prev;
            }
            else
            {
                l->u = tmp->prev;
            }
            free(tmp->inf.titular);
            free(tmp);
        }
        tmp = next;
    }
    return v;
}

void inserare(ListaDubla *l, int index, Nod *nou)
{
    int i = 0;
    Nod *current = l->p;
    while (current)
    {
        Nod *next = current->next;
        if (index == i)
        {
            if (current->prev == NULL)
            {
                nou->next = l->p;
                l->p->prev = nou;
                l->p = nou;
                nou->prev = NULL;
            }
            else if (current->next == NULL)
            {
                l->u->next = nou;
                nou->prev = l->u;
                l->u = nou;
                l->u->next = NULL;
            }
            else
            {
                current->prev->next = nou;
                nou->prev = current->prev;
                nou->next = current;
                current->prev = nou;
            }
        }
        i++;
        current = next;
    }
}

void stergere()
{
}

int main()
{
    ListaDubla lista;
    lista.u = lista.p = NULL;
    FILE *f = fopen("Conturi.txt", "r");
    int nr;
    fscanf(f, "%d", &nr);
    for (int i = 0; i < nr; i++)
    {
        ContBancar c;
        fscanf(f, "%s", c.iban);
        char buffer[200];
        fscanf(f, " %[^\n]", buffer);
        c.titular = malloc(strlen(buffer) + 1);
        strcpy(c.titular, buffer);
        fscanf(f, "%s", c.moneda);
        fscanf(f, "%f", &c.sold);
        adauga(&lista, c);
    }
    parcurge(lista);
    parcurgereInversa(lista);

    /*printf("\nIBAN:");
    int n = 0;
    IBAN* v = salvare_iban(&lista, &n, "EUR");
    for (int i = 0; i < n; i++) {
        printf("%s ", v[i].iban);
    }*/

    Nod *nou = malloc(sizeof(Nod));
    ContBancar c;
    strcpy(c.iban, "RO123232BCR");
    c.titular = malloc(strlen("ANDREI SERBAN") + 1);
    strcpy(c.titular, "ANDREI SERBAN");
    strcpy(c.moneda, "EUR");
    c.sold = 1000;
    nou->inf = c;
    nou->next = nou->prev = NULL;
    inserare(&lista, 1, nou);
    parcurge(lista);
    dezalocare(&lista);
}
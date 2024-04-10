#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

struct ContBancar
{
    char iban[25];  // 24 bytes pt iban + 1 byte terminator string
    char moneda[4]; // 3 bytes pt cod moneda + 1 byte terminator string
    char *titular;
    float sold;
};

struct NodLD
{
    struct ContBancar *pcb;
    struct NodLD *next, *prev;
};

typedef struct ContBancar ContBancar;
typedef struct NodLD NodLD;

struct ListaDubla
{
    NodLD *p, *u;
};

typedef struct ListaDubla ListaDubla;

ListaDubla inserare_sfarsit(ListaDubla lista, ContBancar *pcont)
{
    NodLD *nou = (NodLD *)malloc(sizeof(NodLD));
    nou->pcb = pcont;
    nou->next = NULL;

    if (lista.p == NULL)
    {
        // lista este empty
        nou->prev = NULL;
        lista.p = lista.u = nou;
    }
    else
    {
        // lista contine cel putin 1 nod
        nou->prev = lista.u;
        lista.u->next = nou;
        lista.u = nou;
    }

    return lista;
}

struct IBAN
{
    char iban[25];
};
typedef struct IBAN IBAN;

// salvare conturi bancare cu aceeasi moneda
// conturile bancare sunt stocate in lista dubla
// functia intoarce vectorul: elemente de tip IBAN

IBAN *salveaza_conturi_vector(ListaDubla lista, char *currency, unsigned char *n)
{
    *n = 0;
    NodLD *temp = lista.p;
    while (temp)
    {
        if (strcmp(temp->pcb->moneda, currency) == 0)
            *n += 1;
        temp = temp->next;
    }

    IBAN *vector = (IBAN *)malloc((*n) * sizeof(IBAN));

    unsigned char i = 0;
    temp = lista.p;

    while (temp)
    {
        if (strcmp(temp->pcb->moneda, currency) == 0)
        {
            strcpy(vector[i].iban, temp->pcb->iban);
            i += 1;
        }
        temp = temp->next;
    }

    return vector;
}

// interschimb noduri adiacente in lista dubla pe baza de pozitie [i,i+1]
void interschimbaAdiacente(ListaDubla *lista, int poz)
{
    if (poz < 0 || lista->p == NULL || lista->p->next == NULL)
    {
        printf("Pozitie invalida sau lista prea scurta.\n");
        return;
    }

    NodLD *temp = lista->p;
    int i = 0;

    // Parcurgem lista pana la nodul de pe pozitia i
    for (i = 0; i < poz && temp->next != NULL; i++)
    {
        temp = temp->next;
    }

    // Verificam daca interschimbarea este posibila
    if (temp == NULL || temp->next == NULL)
    {
        printf("Pozitie invalida pentru interschimbare.\n");
        return;
    }

    NodLD *nod1 = temp;
    NodLD *nod2 = temp->next;
    NodLD *precedent = nod1->prev;
    NodLD *urmator = nod2->next;

    if (precedent)
    {
        precedent->next = nod2;
    }
    else
    {
        lista->p = nod2;
    }

    nod2->prev = precedent;
    nod1->prev = nod2;
    nod2->next = nod1;
    nod1->next = urmator;

    if (urmator)
    {
        urmator->prev = nod1;
    }
    else
    {
        lista->u = nod1;
    }
}

// interschimb noduri oarecare in lista dubla pe baza de pozitie [i,j]
void interschimbaOricare(ListaDubla *lista, int i, int j)
{
    if (i == j)
        return; // Nodurile sunt identice, nu este necesara nicio operatie.

    // Asiguram ca i < j pentru a simplifica logica
    if (i > j)
    {
        int temp = i;
        i = j;
        j = temp;
    }

    NodLD *temp = lista->p;
    NodLD *primul = NULL, *alDoilea = NULL;
    int k = 0;

    // Cautam nodurile la pozitiile i si j
    while (temp != NULL && (primul == NULL || alDoilea == NULL))
    {
        if (k == i)
            primul = temp;
        if (k == j)
            alDoilea = temp;
        temp = temp->next;
        k++;
    }

    // Daca unul dintre noduri nu a fost gasit, operatia nu poate continua
    if (primul == NULL || alDoilea == NULL)
        return;

    // Ajustam pointerii pentru nodurile invecinate cu primul si al doilea
    if (primul->prev)
        primul->prev->next = alDoilea;
    if (alDoilea->next)
        alDoilea->next->prev = primul;

    // Daca nodurile sunt adiacente, tratam acest caz separat
    if (primul->next == alDoilea)
    {
        primul->next = alDoilea->next;
        alDoilea->prev = primul->prev;
        alDoilea->next = primul;
        primul->prev = alDoilea;
    }
    else
    {
        // Nodurile nu sunt adiacente, schimbam legaturile dintre ele
        NodLD *tempPrev = primul->prev;
        NodLD *tempNext = primul->next;

        primul->prev = alDoilea->prev;
        primul->next = alDoilea->next;

        alDoilea->prev = tempPrev;
        alDoilea->next = tempNext;

        if (primul->next)
            primul->next->prev = primul;
        if (primul->prev)
            primul->prev->next = primul;
        if (alDoilea->next)
            alDoilea->next->prev = alDoilea;
        if (alDoilea->prev)
            alDoilea->prev->next = alDoilea;
    }

    // Daca interschimbam capul listei, actualizam pointerul la inceput
    if (i == 0)
        lista->p = alDoilea;
    // Daca interschimbam coada listei, actualizam pointerul la sfarsit
    if (j == k - 1)
        lista->u = primul;
}

// sortare lista dubla (folosind interschimbare de mai sus)
void sortareLista(ListaDubla *lista)
{
    int swapped;
    NodLD *ptr1;
    NodLD *lptr = NULL;

    // Verificarea daca lista este goala sau are un singur element
    if (lista->p == NULL || lista->p->next == NULL)
        return;

    do
    {
        swapped = 0;
        ptr1 = lista->p;

        while (ptr1->next != lptr)
        {
            if (ptr1->pcb->sold > ptr1->next->pcb->sold)
            {
                // interschimbarea nodurilor ptr1 si ptr1->next
                if (ptr1 == lista->p)
                { // Daca interschimbam primul nod
                    lista->p = ptr1->next;
                }
                if (ptr1->next == lista->u)
                { // Daca interschimbam ultimul nod
                    lista->u = ptr1;
                }

                NodLD *tmp = ptr1->next;
                ptr1->next = tmp->next;
                if (tmp->next != NULL)
                {
                    tmp->next->prev = ptr1;
                }
                tmp->prev = ptr1->prev;
                if (ptr1->prev != NULL)
                {
                    ptr1->prev->next = tmp;
                }
                tmp->next = ptr1;
                ptr1->prev = tmp;

                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

int main()
{
    FILE *f = fopen("Conturi.txt", "r");
    ListaDubla listaD;
    listaD.p = listaD.u = NULL; // lista dubla este empty

    char nume_buff[128];
    struct ContBancar contB;
    fscanf(f, "%s\n", contB.iban);
    while (strlen(contB.iban))
    {
        fscanf(f, "%[^\n]", nume_buff);
        contB.titular = (char *)malloc(strlen(nume_buff) + 1);
        strcpy(contB.titular, nume_buff);

        fscanf(f, "%s", contB.moneda);
        fscanf(f, "%f", &contB.sold);

        ContBancar *pContB = (ContBancar *)malloc(sizeof(ContBancar));
        *pContB = contB;

        // inserare cont bancar in lista dubla
        listaD = inserare_sfarsit(listaD, pContB);

        contB.titular = NULL; // pentru a elimina partajarea dintre ultimul nod inserat si contB
        contB.iban[0] = 0;
        fscanf(f, "%s\n", contB.iban);
    }

    // traversare lista dubla in ambele sensuri
    NodLD *temp = listaD.p;
    printf("Lista dubla prim->ultim:\n");
    while (temp)
    {
        printf("%s %s\n", temp->pcb->iban, temp->pcb->titular);
        temp = temp->next;
    }
    printf("Lista dubla ultim->prim:\n");
    temp = listaD.u;
    while (temp)
    {
        printf("%s %s\n", temp->pcb->iban, temp->pcb->titular);
        temp = temp->prev;
    }

    // salvare conturi bancare in vector de IBAN-uri
    unsigned char size = 0;
    IBAN *conturi = salveaza_conturi_vector(listaD, "EUR", &size);
    printf("Conturi bancare in EUR:\n");
    for (unsigned char i = 0; i < size; i++)
        printf("%s\n", conturi[i].iban);

    sortareLista(&listaD);
    printf("Lista dubla sortata dupa sold:\n");
    temp = listaD.p;
    while (temp)
    {
        printf("%s %s %f\n", temp->pcb->iban, temp->pcb->titular, temp->pcb->sold);
        temp = temp->next;
    }
    // dezalocare lista dubla
    while (listaD.p)
    {
        temp = listaD.p;
        listaD.p = listaD.p->next;
        if (listaD.p != NULL)
            listaD.p->prev = NULL;

        free(temp->pcb->titular); // dezalocare string pt titular
        free(temp->pcb);          // dezalocare ContBancar
        free(temp);               // dezalocare nod lista dubla (NodLD)
    }
    listaD.u = NULL;

    printf("Lista dubla ultim->prim dupa dezalocare:\n");
    temp = listaD.u;
    while (temp)
    {
        printf("%s %s\n", temp->pcb->iban, temp->pcb->titular);
        temp = temp->prev;
    }

    // dezalocare vector de conturi bancare
    free(conturi);
    conturi = NULL;
    fclose(f);
    f = NULL;
    return 0;
}
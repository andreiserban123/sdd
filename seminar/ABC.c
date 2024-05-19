#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#define HTABLE_SIZE 200

struct ContBancar
{
    char iban[25];  // 24 bytes pt iban + 1 byte terminator string
    char moneda[4]; // 3 bytes pt cod moneda + 1 byte terminator string
    char *titular;
    float sold;
};

struct NodABC
{
    unsigned short int cheie;
    struct ContBancar data;
    struct NodABC *st, *dr;
};

typedef struct ContBancar ContBancar;
typedef struct NodABC NodABC;

NodABC *inserare_nod_arbore(NodABC *r, ContBancar contB,
                            unsigned short int key, unsigned char *flag)
{
    if (r != NULL)
    {
        if (key < r->cheie)
        {
            r->st = inserare_nod_arbore(r->st, contB, key, flag);
        }
        else
        {
            if (key > r->cheie)
            {
                r->dr = inserare_nod_arbore(r->dr, contB, key, flag);
            }
            else
            {
                *flag = 0; // flag setat pe ESEC!
            }
        }
    }
    else
    {
        // locul unde nodul nou se insereaza
        NodABC *nou = (NodABC *)malloc(sizeof(NodABC));
        nou->cheie = key;
        nou->data = contB;
        nou->st = NULL;
        nou->dr = NULL;

        *flag = 1; // flag setat pe SUCCES!
        r = nou;
    }

    return r;
}

void inordine(NodABC *r)
{
    if (r)
    {
        inordine(r->st);
        printf("%d ", r->cheie);
        inordine(r->dr);
    }
}

void dezalocareArbore(NodABC **rad)
{
    if (*rad)
    {
        dezalocareArbore(&(*rad)->st);
        dezalocareArbore(&(*rad)->dr);
        free((*rad)->data.titular);
        free((*rad));
    }
    *rad = NULL;
}

int cautareDupaKey(NodABC *root, ContBancar *c, unsigned short int key)
{
    if (root)
    {
        if (root->cheie == key)
        {
            *c = root->data;
            c->titular = malloc(strlen(root->data.titular) + 1);
            strcpy(c->titular, root->data.titular);
            return 1;
        }
        else if (key < root->cheie)
        {
            cautareDupaKey(root->st, c, key);
        }
        else
        {
            cautareDupaKey(root->dr, c, key);
        }
    }
    else
    {
        return 0;
    }
}

void cautareDupaIBAN(NodABC *root, ContBancar *c, char iban[25], int *ok)
{
    if (root)
    {
        cautareDupaIBAN(root->st, c, iban, ok);
        cautareDupaIBAN(root->dr, c, iban, ok);
        if (strcmp(root->data.iban, iban) == 0)
        {
            *c = root->data;
            c->titular = malloc(strlen(root->data.titular) + 1);
            strcpy(c->titular, root->data.titular);
            *ok = 1;
        }
    }
}

int main()
{
    FILE *f = fopen("ConturiABC.txt", "r");
    NodABC *root = NULL; // arbore binar de cautare empty
    unsigned short int key;
    char nume_buff[128];

    struct ContBancar contB;
    fscanf(f, "%hu", &key);
    while (key != 0)
    {
        fscanf(f, "%s\n", contB.iban);

        fscanf(f, "%[^\n]", nume_buff);
        contB.titular = (char *)malloc(strlen(nume_buff) + 1);
        strcpy(contB.titular, nume_buff);

        fscanf(f, "%s", contB.moneda);
        fscanf(f, "%f", &contB.sold);

        // inserare cont bancar in arbore binar de cautare
        unsigned char inserat = 0;
        root = inserare_nod_arbore(root, contB, key, &inserat);
        if (inserat != 0)
        {
            contB.titular = NULL; // pentru a elimina partajarea dintre ultimul nod inserat si contB
            contB.iban[0] = 0;
        }
        else
        {
            // contul bancar nu a fost inserat in ABC
            free(contB.titular); // dezalocare mem heap pt titular
        }
        key = 0; // reinitializare cheie pentru a verifica existenta urmatorului cont bancar in fisier

        fscanf(f, "%hu", &key);
    }
    fclose(f);

    printf("Arbore dupa creare: ");
    inordine(root);
    printf("\n\n");

    // TODO: cautare cont bancar dupa cheie

    int ok = cautareDupaKey(root, &contB, 61);
    if (ok == 1)
    {
        printf("A fost gasit contul cu titular : %s", contB.titular);

        free(contB.titular);
    }
    else
    {
        printf("NU a fost gasit contul dupa cheie!");
    }

    // TODO: cautare cont bancar dupa iban

    ok = 0;
    cautareDupaIBAN(root, &contB, "RO04BRDE1234567890556222", &ok);
    if (ok == 1)
    {
        printf("\nA fost gasit contul cu titular : %s", contB.titular);

        free(contB.titular);
    }
    else
    {
        printf("\nNU a fost gasit un cont cu acel IBAN!");
    }
    // TODO: dezalocare ABC

    dezalocareArbore(&root);
    return 0;
}
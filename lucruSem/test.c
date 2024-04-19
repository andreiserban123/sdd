#include <stdio.h>
#include <malloc.h>

char g[100];

typedef struct ContBancar
{
    int cod;
    char IBAN[24];
    double sold;
} ContBancar;

typedef struct NodLS
{
    struct NodLS *next;
    ContBancar inf;
} NodLS;

int main()
{
    return 0;
}
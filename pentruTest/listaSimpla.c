#include <stdio.h>
#include <string.h>
#include <malloc.h>

typedef struct
{
	int nrMatricol;
	char *nume;
	float medie;
} student;

typedef struct
{
	struct NodLS *next;
	student inf;
} NodLS;

NodLS *inserareNOD(NodLS *cap, student s)
{

	NodLS *nou = malloc(sizeof(NodLS));
	nou->inf.nrMatricol = s.nrMatricol;
	nou->inf.medie = s.medie;
	nou->inf.nume = malloc(strlen(s.nume) + 1);
	strcpy(nou->inf.nume, s.nume);
	nou->next = NULL;

	if (cap == NULL)
	{
		cap = nou;
	}
	else
	{
		NodLS *tmp = cap;

		while (tmp->next != NULL)
		{
			tmp = tmp->next;
		}
		tmp->next = nou;
	}

	return cap;
}

void traversare(NodLS *cap)
{
	while (cap != NULL)
	{
		printf("\nNr. Matricol:%d,  NUME: %s,  MEDIE:%5.2f", cap->inf.nrMatricol, cap->inf.nume, cap->inf.medie);
		cap = cap->next;
	}
}

void dezalocare(NodLS **cap)
{
	NodLS *tmp = *cap;
	while ((*cap) != NULL)
	{
		tmp = *cap;
		*cap = (*cap)->next;
		free(tmp->inf.nume);
		free(tmp);
	}
	*cap = NULL;
}

void salvareVector(NodLS *cap, int nr, student **v)
{
	*v = malloc(sizeof(student) * nr);
	int i = 0;
	while (cap != NULL)
	{
		(*v)[i].nrMatricol = cap->inf.nrMatricol;
		(*v)[i].nume = malloc(strlen(cap->inf.nume) + 1);
		strcpy((*v)[i].nume, cap->inf.nume);
		(*v)[i].medie = cap->inf.medie;
		i++;
		cap = cap->next;
	}
}
void afisareVect(student *v, int nr)
{
	for (int i = 0; i < nr; i++)
	{
		printf("\nNr. Matricol:%d,  NUME: %s,  MEDIE:%5.2f", v[i].nrMatricol, v[i].nume, v[i].medie);
	}
}

NodLS *stergereNOD(NodLS *cap, int nrMatricol)
{

	if (cap == NULL)
	{
		printf("Lista este goala\n");
		return NULL;
	}

	NodLS *tmp = cap;
	// daca studentul este la inceputul listei sau finalul listei
	if (cap->inf.nrMatricol == nrMatricol)
	{
		cap = cap->next;
		free(tmp->inf.nume);
		free(tmp);
		return cap;
	}
	NodLS *prev = tmp;
	tmp = tmp->next;
	while (tmp != NULL)
	{
		if (tmp->inf.nrMatricol == nrMatricol)
		{
			prev->next = tmp->next;
			free(tmp->inf.nume);
			free(tmp);
			return cap;
		}
		prev = tmp;
		tmp = tmp->next;
	}
	printf("\nStudentul nu a fost gasit\n");
	return cap;
}

void dezalocareVect(student **v, int nr)
{
	for (int i = 0; i < nr; i++)
	{
		free((*v)[i].nume);
	}
	free(*v);
	*v = NULL;
}

NodLS *inserareLaPozitie(NodLS *cap, student s, int poz)
{

	NodLS *nou = malloc(sizeof(NodLS));
	nou->next = NULL;
	nou->inf.nrMatricol = s.nrMatricol;
	nou->inf.medie = s.medie;
	nou->inf.nume = malloc(strlen(s.nume) + 1);
	strcpy(nou->inf.nume, s.nume);

	if (cap == NULL)
	{
		return nou;
	}

	if (poz == 0 || poz < 0)
	{
		nou->next = cap;
		return nou;
	}
	int i = 0;
	NodLS *tmp = cap, *prev = cap, *ultim = NULL;
	tmp = tmp->next;
	i++;
	while (tmp != NULL)
	{
		if (i == poz)
		{
			prev->next = nou;
			nou->next = tmp;
			return cap;
		}
		prev = tmp;
		if (tmp->next == NULL)
		{
			ultim = tmp;
		}
		tmp = tmp->next;
		i++;
	}
	ultim->next = nou;
	return cap;
}

int main()
{
	int nr;
	NodLS *cap = NULL;
	FILE *f = fopen("listaSimpla.txt", "r");
	if (f != NULL)
	{
		fscanf(f, "%d", &nr);
		for (int i = 0; i < nr; i++)
		{
			student s;
			fscanf(f, "%d", &s.nrMatricol);
			char buffer[200];
			fscanf(f, " %[^\n]", buffer);
			s.nume = malloc(strlen(buffer) + 1);
			strcpy(s.nume, buffer);
			fscanf(f, "%f", &s.medie);
			cap = inserareNOD(cap, s);
		}

		traversare(cap);

		student s1;
		s1.nrMatricol = 4000;
		s1.nume = malloc(strlen("Arabela") + 1);
		strcpy(s1.nume, "Arabela");
		s1.medie = 4.5;
		cap = inserareLaPozitie(cap, s1, 10000);

		cap = stergereNOD(cap, 4000);

		/*cap = stergereNOD(cap, 5000);
		cap = stergereNOD(cap, 1000);*/
		printf("\n---------------------\n");
		traversare(cap);

		// printf("\n-------------salvare vector---------------\n");
		/*student* v;
		salvareVector(cap, nr, &v);
		afisareVect(v, nr);*/
		dezalocare(&cap);
		// dezalocareVect(&v, nr);
		fclose(f);
	}
	else
	{
		printf("FISIERUL NU S-A DESCHIS CU SUCCESS!");
	}
	return 0;
}
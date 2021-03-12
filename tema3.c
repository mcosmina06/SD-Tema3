//***NUME Mandru, PRENUME Cosmina***
//****GRUPA 313CB, SERIA CB*********

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <stdint.h>
#include <stdbool.h>

typedef struct nod
{
	char *info;
	int nr_fii;
	bool isvisited; 
	bool isfinal;
	bool istouch; //true - fisier, false - director
	struct nod* parinte;
	struct nod** lista_fii;
}TNodArb, *TNA, **ANA;

//functie ce aloca un nod din arbore
TNA Aloca_Nod_Arb(char x) 
{
	TNA aux;
	
	aux = (TNA)malloc(sizeof(TNodArb));
	if(!aux) 
	{
		return NULL;
	}
	
	aux->info = strdup(&x);
	aux->nr_fii = 0;
	aux->isvisited = false;
	aux->isfinal = false;
	aux->istouch = false;
	aux->parinte = NULL;
	aux->lista_fii = (ANA)malloc(27 * sizeof(TNA));
	if(!aux->lista_fii) 
	{
		return NULL;
	}

	return aux;
}

//functie ce cauta un nod in arbore, daca acesta este gasit returneaza
//nodul, in caz contrar returneaza NULL
TNA find_nod(ANA L, char x, int nr_fii) 
{	
	int i;

	for(i = 0; i < nr_fii; i++) 
	{
		if(x == *(L[i]->info)) 
		{
			return L[i];
		}
	}

	return NULL;
}

//functie de inserare inarbore
//variabila file este 0 daca numele de inserat este un director si
// 1 daca este fisier
int Ins_Arb(char *x,  ANA arb, int file) 
{	
	TNA curent = *arb, aux;
	int i, j;

	//prima situatie cand arborele este null: primul nod(radacina) va contine
	//caracterul "/" adica primul caracter din cuvantul ce trebuie inserat
	if(*arb == NULL) 
	{	
		*arb = Aloca_Nod_Arb(x[0]);
		curent = *arb;
		for(i = 1; i < strlen(x); i++) 
		{
			aux = Aloca_Nod_Arb(x[i]);
			aux->parinte = curent;	
			//se creaza legatura dintre noduri
			curent->lista_fii[0] = aux;
			curent->nr_fii++;
			curent = curent->lista_fii[0];
			//se marcheaza sfarsitul de cuvant si daca este fisier
			if(i == strlen(x) - 1)
			{
				if(file == 1)
				{
					curent->istouch = true;
				}
				curent->isfinal = true;
			}
		}
		return 1;
	}
	
	for(i = 1; i < strlen(x); i++) 
	{	
		//este cautat nodul cu carcaterl din cuvantul
		aux = find_nod(curent->lista_fii, x[i], curent->nr_fii);
		//daca ascesta este gasit caracterul nu este inserat dar se
		//"face legatura"
		if(aux != NULL) 
		{
			curent = aux;
			if(i == strlen(x) - 1)
			{
				if(file == 1)
				{
					curent->istouch = true;
				}
				curent->isfinal = true;
			}
		}
		//daca nu este gasit este creat noul nod iar restul caracterelor
		//de la i + 1 sunt inserate in continuare fara a se mai efectua 
		//cautarea
		else 
		{
			aux = Aloca_Nod_Arb(x[i]);
			aux->parinte = curent;	
			curent->lista_fii[curent->nr_fii] = aux;
			curent->nr_fii++;
			curent = aux;
			if(i == strlen(x) - 1)
			{
				if(file == 1)
				{
					curent->istouch = true;
				}
				curent->isfinal = true;
			}
			for(j = i + 1; j < strlen(x); j++) 
			{
				aux = Aloca_Nod_Arb(x[j]);
				aux->parinte = curent;
				curent->lista_fii[curent->nr_fii] = aux;
				curent->nr_fii++;
				curent = curent->lista_fii[curent->nr_fii - 1];
				if(j == strlen(x) - 1)
				{
					if(file == 1)
					{
						curent->istouch = true;
					}
					curent->isfinal = true;
				}
			}
			return 1;
		}
	}
	return 1;
}

//functie de eliberare a memoriei pentru arbore
void free_arb(ANA arb) 
{	
	int i;
	
	if(*arb == NULL || (*arb)->nr_fii == 0)
	{
		return;
	}

	for(i = 0; i < (*arb)->nr_fii; i++) 
	{
		if((*arb)->lista_fii[i]) 
		{
			free_arb(&(*arb)->lista_fii[i]);
			if((*arb)->lista_fii[i]->info != NULL) 
			{
				free((*arb)->lista_fii[i]->info);
				(*arb)->lista_fii[i]->info = NULL;
			}
		}
	}

	if((*arb)->lista_fii != NULL) 
	{
		free((*arb)->lista_fii);
		(*arb)->lista_fii = NULL;
		(*arb)->nr_fii = 0;
	}
}

//functie ce marcheaza toate note nodurile ca fiind nevizitate
void mark_all_unvisited(TNA arb) 
{
	int i;
	
	if(arb == NULL) {
		return;
	}

	arb->isvisited = false;
	
	for(i = 0; i < arb->nr_fii; i++) {
		mark_all_unvisited(arb->lista_fii[i]);
	}
}

//functie ce face nodurile pe o ramura data ce au fost vizitate si
//returneaza numarul de fii ce nu au fost vizitati, utila in functia ls
int make_subbranch_visited(TNA start_node, TNA arb_start) 
{
	int i, nr_fii_nevizitati = 0;
	
	if (start_node == NULL) 
	{
		return 0;
	}
	
	while(start_node) 
	{
		//functia parcurge arborele in "sus"
		start_node->isvisited = true;
		start_node = start_node->parinte;
		
		if (start_node == NULL || start_node == arb_start) 
		{
			return 0;
		}
		
		//se numara fii nodului ce nu au fost vizitati, adica afisati
		for(i = 0; i < start_node->nr_fii; i++) 
		{
			if (start_node->lista_fii[i]->isvisited == false) 
			{
				nr_fii_nevizitati++;
			}
		}
		
		if (nr_fii_nevizitati != 0) 
		{
			break;
		}
	}

    return nr_fii_nevizitati;
}

//functie de cauatare in arbore a unei cai catre un fisier sau director
TNA search(TNA arb, char *x)
{
	TNA aux, curent = arb;
	int i;

	if(arb == NULL)
	{
		return NULL;
	}

	if(x[0] != *curent->info)
	{
		return NULL;
	}

	//se ia fiecare caracter din cuvant si se cauta in arbore
	//daca unul dintre ele nu a fost gasit numai are rost cautarea
	//si se returneaza NULL
	for(i = 1; i < strlen(x); i++) 
	{	
		aux = find_nod(curent->lista_fii, x[i], curent->nr_fii);
		if(aux != NULL) 
		{
			curent = aux;
		}
		else
		{
			return NULL;
		}
	}

	if(aux != NULL)
	{
		return aux;
	}
	else
	{
		return NULL;
	}
}

//functie ce verifica existenta unui folder/fisier in arbore
//functia primeste ca parametru calea catre fisier(inclusiv numele 
//fisierului ce trbuie inserat)
int exist_node(TNA arb, char *name)
{
	char *aux;
	int i, k;
	TNA aux_arb;

	//este parcurs cuvantul si retinuta ultima pozitie a /
	for(i = 0; i < strlen(name); i++)
	{
		if(name[i] == '/')
		{
			k = i;
		}
	}
	
	//daca k este 0 inseamna ca directorul cautat este chiar /
	//deci exista
	if(k == 0)
	{
		return 1;
	}

	aux = malloc((k + 1) * sizeof(char));
	if(!aux)
	{
		free(aux);
		return 0;
	}
	
	//este creat cuvantul ce trebuie cautat in arbore
	//(calea unde se doreste inserarea)
	for(i = 0; i < k; i++)
	{
		aux[i] = name[i];
	}
	aux[k] = '\0';
	aux_arb = search(arb, aux);
	free(aux);
	//daca nu a fost gasit se returneaza 0 altfel 1
	if(aux_arb == NULL)
	{
		return 0;
	}
	return 1;
}

//functie ce merge pe fiecare ramura si creaza cuvantul in vectorul x ce 
//este apoi pus in vectorul x_v
void DFS_one_branch_ls(TNA arb, TNA arb_start, bool *stop, int dim, char *x, 
	char ***x_v, int *dim2, int *nr_fii_neviz)
{
	int i;

	if(arb == NULL || *stop == true || arb->isvisited == true) 
	{
		return;
	}

	if((*(arb->info) != '/')) 
	{
		if(dim == 0) 
		{
			x = (char*)malloc(sizeof(char));
		}
		else 
		{
			x = (char *)realloc(x, (dim+1) * sizeof(char));
		}
		//se creaza numele
		x[dim] = *(arb->info);
		dim++;
		//daca caracterul introdus este final de cuvant se introduce in vectorul x_v
		if(arb->isfinal == true && (arb->nr_fii == 0 || 
			arb->lista_fii[0]->isvisited == false))
		{
			if(*dim2 == 0) 
			{
				*x_v = (char**)malloc(sizeof(char*));
			}
			else 
			{
				*x_v = (char**)realloc(*x_v, ((*dim2)+1) * sizeof(char*));
			}
			if(!*x_v) 
			{
				return;
			}
			x[dim] = '\0';
			(*x_v)[*dim2] = strdup(x);
			(*dim2)++;
		}
	}

	//daca s-a ajuns la "/" - sfarsitul dedirector sau numai exista fii se
	//opreste recursivitatea pe ramura
	if(arb->nr_fii == 0 || (*(arb->info) == '/')) 
	{
		*stop = true;
		*nr_fii_neviz = make_subbranch_visited(arb, arb_start);
		free(x);
		return;
	}

	for(i = 0; i < arb->nr_fii; i++) 
	{
		DFS_one_branch_ls(arb->lista_fii[i], arb_start, stop, dim,
		 x, x_v, dim2, nr_fii_neviz);
	}
}

//functie ce apeleaza functia anterioara de recursivitate pentru fiecare
//ramura ce porneste din root
char** DFS_all_branches_ls(TNA arb, int* dim2)
{
	bool stop;
	int i = 0, dim1 = 0, nr_fii_neviz;
	char *x = NULL, **x_v = NULL;
	mark_all_unvisited(arb);

	while(1) {
		stop = false;
		nr_fii_neviz = 0;
		DFS_one_branch_ls(arb->lista_fii[i], arb, &stop, dim1, x, 
			&x_v, dim2, &nr_fii_neviz);
		if(stop == false)
		{
			break;
		}
		//se avanseaza pe urmatoare ramura care porneste din radacina
		//in momentul in care toate nodurile de pe ramura anterioara
		//au fost vizitate
		if(nr_fii_neviz == 0)
		{
			i++;
		}
		if(i == arb->nr_fii)
		{
			break;
		}
	}

	mark_all_unvisited(arb);
	return x_v;
}

//functie ce sorteaza vectorul care contine numele directoarelor/fisierelor 
//ce trebuiesc afisate
void sort(char** x, int dim)
{
	int i, ok = 1;
	char *aux;

	while(ok == 1)
	{
		ok = 0;
		for(i = 0; i < dim - 1; i++)
		{	
			if(strcmp(x[i], x[i + 1]) > 0)
			{
				aux = x[i];
				x[i] = x[i + 1];
				x[i + 1] = aux;
				ok = 1;
			}
		}
	}
}

//functia de afisare a directoarelor
void ls(TNA arb, char* curent_directory, char* ls_directory)
{
	int i, dim = 0;
	char **x, *aux;
	TNA aux_arb;
	
	//daca nu exista nimic de afisat se afiseaza spatiu
	if(arb == NULL)
	{
		printf("\n");
		return;
	}

	//daca directorul curent nu este root se cauta ultimul nod din numele
	//acestuia
	if(strcmp(curent_directory, "/") != 0)
	{
		aux_arb = search(arb, curent_directory);
	}
	//daca se afiseaza intreg arborele
	else
	{
		aux_arb = arb;
	}

	if(aux_arb->nr_fii == 0)
	{
		printf("\n");
		return;
	}

	aux = malloc(50 * sizeof(char));

	//daca s-a primit ca parametru ca ls_directory nul inseamna ca se 
	//afiseaza directorul curent
	if(strcmp(ls_directory, "nul") == 0)
	{
		if(strcmp(curent_directory, "/") != 0)
		{
			strcpy(aux, curent_directory);
			strcat(aux, "/");
			aux_arb = search(arb, aux);
		}	
	}
	else
	{
		if(ls_directory[0] != '/')
		{
			strcpy(aux, curent_directory);
			if(strcmp(curent_directory, "/") != 0)
			{
				strcat(aux, "/");
			}
			strcat(aux, ls_directory);
			strcat(aux, "/");
		}
		else
		{
			strcpy(aux, ls_directory);
			strcat(aux, "/");
		}
		aux_arb = search(arb, aux);
	}

	//functia aceata intoarce vectorul ce contine numele directoarelor
	//ce trebuiesc afisate ce este mai intai sortat
	x = DFS_all_branches_ls(aux_arb, &dim);
	sort(x, dim);
	
	for(i = 0; i < dim; i++)
	{
		if(i == dim - 1)
		{
			printf("%s\n", x[i]);
		}
		else
		{
			printf("%s ", x[i]);
		}
		free(x[i]);
		x[i] = NULL;
	}
	free(aux);
	free(x);
}

//functia mkdir sau touch: este introdus un fisier daca file = 1 si director daca file = 0
int mkdir_touch(ANA arb, char* name_directory, char* curent_directory, 
	FILE* ferr, int file)
{
	TNA aux_arb;
	int check;
	char *aux = malloc(50 * sizeof(char));
	
	if(!aux)
	{
		return 0;
	}
	//daca numele directorului/fisierului nu incepe cu / inseamna ca
	//acesta trebuie introdus in directorul curent
	if(name_directory[0] != '/')
	{
		strcpy(aux, curent_directory);
		
		if(strcmp(curent_directory, "/") != 0)
		{
			strcat(aux, "/");
		}
		
		strcat(aux, name_directory);
		
		aux_arb = search(*arb, aux);

		//daca nodul returnat de functia search este diferit de NULL si
		//nodul nu are niciun fiu se poate afirma ca folderul/fisierul
		//exista deja
		if(aux_arb != NULL && aux_arb->nr_fii == 0)
		{
			fprintf(ferr, "%s: already exists\n", name_directory);
			free(aux);
			return 0;	
		}

		//se verifica daca directorul in care se doreste inserarea exista
		check = exist_node(*arb, aux);
		if(check == 0)
		{
			fprintf(ferr, "%s: No such file or directory\n", name_directory);
			free(aux);
			return 0;
		}
		
		Ins_Arb(aux, arb, file);		
	}

	//daca numele directorului/fisierului incepe cu / inseamna ca 
	//este data calea dorita in care se doreste fisierul indiferent 
	//de curent_directory
	else
	{	
		check = exist_node(*arb, name_directory);
		
		if(check == 0)
		{
			fprintf(ferr, "%s: No such file or directory\n", name_directory);
			free(aux);
			return 0;
		}
		
		Ins_Arb(name_directory, arb, file);
	}

	free(aux);
	return 0;	
}

//functia cd_back trateaza caurile ".." sau "../../."
void cd_back(TNA arb, char* curent_directory, char* name_directory)
{
	char *p, *aux, *copy, *aux2;
	int k = 0, w = 0, i;

	aux = malloc(100 * sizeof(char));
	if(!aux)
		return;
	if(strcmp(name_directory, "..") == 0)
	{	
		copy = malloc(50*sizeof(char));
		if(!copy)
		{
			free(aux);
			return;
		}
		strcpy(copy, curent_directory);
		p = strtok(copy, "/");
		//numar "/" din curent directory
		while(p != NULL)
		{
			k++;
			p = strtok(NULL, "/");
		}
		k--;
		p = strtok(curent_directory, "/");
		//construiesc noul curent directory pana la ultimul "/"
		while(p != NULL && k != 0)
		{
			k--;
			aux2 = strdup("/");
			strcat(aux2, p);
			strcat(aux, aux2);
			p = strtok(NULL, "/");
		}
		strcpy(curent_directory, aux);
		//cazul in care curent directory este "/"
		if(strlen(curent_directory) == 0)
		{
			strcpy(curent_directory, "/");
		}
		free(copy);
		free(aux);
		return;
	}

	//asemanator functiei anterioare doar ca proceseul se repeta in functie
	//de cate "/" sunt in name directory
	if(strncmp(name_directory, "../", 3) == 0)
	{
		p = strtok(name_directory, "/");
		while(p != NULL)
		{
			w++;
			p = strtok(NULL, "/");
		}
		for(i = 0; i < w; i++)
		{	
			p = strtok(curent_directory, "/");
			while(p != NULL)
			{
				k++;
				p = strtok(NULL, "/");
			}
			k--;
			p = strtok(curent_directory, "/");
			while(p != NULL && k != 1)
			{	
				k--;
				strcat(aux, p);
				p = strtok(NULL, "/");
			}
		strcpy(curent_directory, aux);
		}
		free(aux);
		return;
	}	
	free(aux);
}

//functia cd de schimbare a directorului curent
void cd(TNA arb, char* curent_directory, char* name_directory, FILE *ferr)
{
	TNA aux_arb;
	char *aux;

	aux = malloc(100 * sizeof(char));
	if(!aux)
		return;

	//cazul in care curent directory este "/"
	if(strcmp(curent_directory, "/") == 0)
	{
		//nu exista un "director parinte"
		if(strncmp(name_directory, "..", 2) == 0)
		{
			fprintf(ferr, "%s: No such file or directory\n", name_directory);
			free(aux);
			return;
		}
		if(name_directory[0] != '/')
		{	
			strcpy(aux, curent_directory);
			strcat(aux, name_directory);
			aux_arb = search(arb, aux);
			//cazul in care nu exista directorul in arbore
			if(aux_arb == NULL)
			{
				fprintf(ferr, "%s: No such file or directory\n", name_directory);
				free(aux);
				return;
			}
			//cazul in care directorul dat ca argument este de fapt un fisier
			if(aux_arb->istouch == true)
			{
				fprintf(ferr, "%s: Not a directory\n", name_directory);
				free(aux);
				return;				
			}
			else
			{
				strcpy(curent_directory, aux);
				free(aux);
				return;
			}
		}
		else
		{
			strcpy(curent_directory, name_directory);
		}
		free(aux);
		return;
	}

	if(strcmp(name_directory, "..") == 0 || (strncmp(name_directory, "../", 3) == 0))
	{
		cd_back(arb, curent_directory, name_directory);
		free(aux);
		return;
	}

	if(strcmp(curent_directory, "/") != 0)
	{
		if(name_directory[0] != '/')
		{	
			strcpy(aux, curent_directory);
			strcat(aux, "/");
			strcat(aux, name_directory);
		}
		else
		{
			strcpy(aux, name_directory);
		}
		aux_arb = search(arb, aux);
		if(aux_arb == NULL)
		{
			fprintf(ferr, "%s: No such file or directory\n", name_directory);
			free(aux);
			return;
		}
		else
		{
			strcpy(curent_directory, aux);
			free(aux);
			return;
		}
	}
}

//functia mv primeste ca parametru directorul curent, fisierul ce trebuie mutat
// si calea pentru mutare
void mv(ANA arb, char *curent_directory, char *file_to_mv, char *path_to_mv, 
	FILE *ferr)
{
	TNA aux_arb;
	char *aux = malloc(100 * sizeof(char));
	if(!aux)
	{
		return;
	}

	//la fel ca mai sus se veridica daca fisierul ce trebuie mutat
	//se afla in directorul curent sau este data calea pana la acesta
	if(file_to_mv[0] != '/')
	{	
		strcpy(aux, curent_directory);
		if(strcmp(curent_directory, "/") != 0)
		{
			strcat(aux, "/");
		}
		strcat(aux, file_to_mv);
	}
	else
	{
		strcpy(aux, file_to_mv);
	}

	//se verifica existenta fisierul si daca este este fisier sau nu
	aux_arb = search(*arb, aux);
	
	if(aux_arb == NULL || aux_arb->istouch == false)
	{
		fprintf(ferr, "%s: No such file or directory\n", file_to_mv);
		free(aux);
		return;
	}

	if(path_to_mv[0] != '/')
	{	
		strcpy(aux, curent_directory);
		
		if(strcmp(curent_directory, "/") != 0)
		{
			strcat(aux, "/");
		}
		
		strcat(aux, path_to_mv);
		aux_arb = search(*arb, aux);
		//se verifica daca folderul in care se doreste mutarea exista sau nu
		if(aux_arb == NULL)
		{
			fprintf(ferr, "%s: No such file or directory\n", path_to_mv);
			free(aux);
			return;
		}
		//se verifica daca calea la care se doreste mutarea este folder
		if(aux_arb->istouch == true)
		{
			fprintf(ferr, "%s: Not a directory\n", path_to_mv);
			free(aux);
			return;
		}
	}
}

//functia cp este asemanatoare cele mv doar ca nu executa si stergerea 
//fisierului dupa ce acesta a fost mutat in folderul dorit
void cp(ANA arb, char *file_to_cp, char *path_to_cp, char *curent_directory, 
	FILE *ferr)
{
	TNA aux_arb;
	char *aux = malloc(100 * sizeof(char));
	if(!aux)
	{
		return;
	}

	if(path_to_cp[0] != '/')
	{	
		strcpy(aux, curent_directory);
		
		if(strcmp(curent_directory, "/") != 0)
		{
			strcat(aux, "/");
		}
		
		strcat(aux, path_to_cp);
		aux_arb = search(*arb, aux);
		
		//se verifica existenta folderului in care se dorste mutarea
		if(aux_arb == NULL)
		{
			fprintf(ferr, "%s: No such file or directory\n", path_to_cp);
			free(aux);
			return;
		}
		
		else
		{
			if(file_to_cp[0] != '/')
			{
				strcat(aux, "/");
				strcat(aux, file_to_cp);
			}
			Ins_Arb(aux, arb, 1);
			free(aux);
			return;
		}
	}
}

//functia rmdir sterge nodurile din arbore ce alcatuiesc numele directorului
//ce trebuie sters
void rmdir(ANA arb, char* name_directory, char* curent_directory, FILE *ferr)
{
	TNA aux_arb;
	char *aux = malloc(100 * sizeof(char));
	if(!aux)
	{
		return;
	}

	if(name_directory[0] != '/')
	{	
		strcpy(aux, curent_directory);
		
		if(strcmp(curent_directory, "/") != 0)
		{
			strcat(aux, strdup("/"));
		}
		
		strcat(aux, name_directory);
		aux_arb = search(*arb, aux);
		
		//se verifica daca exista direcotul ce trebuie sters
		// a doua conditie din if trateaza cazul in care exista spre exemplu
		// exista folderul "manual" dar se doreste a fi ster "man" care acesta
		//nu exista
		if(aux_arb == NULL || (aux_arb && aux_arb->isfinal == false))
		{
			fprintf(ferr, "%s: No such file or directory\n", name_directory);
			free(aux);
			return;
		}
		
		//se verifica daca directorul este gol
		if(aux_arb->nr_fii != 0)
		{
			fprintf(ferr, "%s: Directory not empty\n", name_directory);
			free(aux);
			return;
		}
	}
}

//functia rm asemanatoare cu cea rmdir
void rm(ANA arb, char *name_directory, char *curent_directory, FILE *ferr)
{
	TNA aux_arb;
	char *aux = malloc(100 * sizeof(char));
	if(!aux)
	{
		return;
	}

	if(name_directory[0] != '/')
	{	
		strcpy(aux, curent_directory);

		if(strcmp(curent_directory, "/") != 0)
		{
			strcat(aux, strdup("/"));
		}

		strcat(aux, name_directory);
		aux_arb = search(*arb, aux);
		
		if(aux_arb == NULL || (aux_arb && aux_arb->isfinal == false))
		{
			fprintf(ferr, "%s: No such file or directory\n", name_directory);
			free(aux);
			return;
		}
	}
}

int main()
{
	TNA arb = NULL;
	int i, n;
	char *mv_path;
	char line[256], *p;
	char* filenamein = strdup("input.in");
	char* filenameout = strdup("output.out");
	char* filenameerr = strdup("error.out");
	char *ls_directory = strdup("nul");
	char *curent_directory = malloc(100 * sizeof(char));
	 
	strcpy(curent_directory, "/");

	FILE *fin = fopen(filenamein, "r");

	if (fin == NULL)
	{
        fprintf(stderr, "ERROR: Can't open file %s", filenamein);
        free(filenamein);
        free(filenameout);
        free(filenameerr);
        free(ls_directory);
        free(curent_directory);
        return -1;
    }

	FILE *fout = fopen(filenameout, "a");

	if (fout == NULL) 
	{
        fprintf(stderr, "ERROR: Can't open file %s", filenameout);
        free(filenamein);
        free(filenameout);
        free(filenameerr);
        free(ls_directory);
        free(curent_directory);
        return -1;
    }

	FILE *ferr = fopen(filenameerr, "a");

	if (ferr == NULL) 
	{
        fprintf(stderr, "ERROR: Can't open file %s", filenameerr);
        free(filenamein);
        free(filenameout);
        free(filenameerr);
        free(ls_directory);
        free(curent_directory);
        return -1;
    }

	fgets(line, sizeof(line), fin);
	n = atoi(line);
	for(i = 0; i < n; i++)
	{
		fgets(line, sizeof(line), fin);
		p = strtok(line, "\n");
		p = strtok(p, " ");
		if(strcmp(p, "mkdir") == 0) 
		{	
			p = strtok(NULL, " ");
			while (p != NULL) 
			{
				mkdir_touch(&arb, p, curent_directory, ferr, 0);
				p = strtok(NULL, " ");
			}
		}
		
		else if(strcmp(p, "ls") == 0)
		{
			p = strtok(NULL, " ");
			if(p == NULL) 
			{
				ls(arb, curent_directory, ls_directory);
			}
			else 
			{
				ls(arb, curent_directory, p);
			}
		}

		else if(strcmp(p, "cd") == 0)
		{
			p = strtok(NULL, " ");
			cd(arb, curent_directory, p, ferr);
		}

		else if(strcmp(p, "pwd") == 0)
		{
			p = strtok(NULL, " ");
			printf("%s\n", curent_directory);
		}

		else if(strcmp(p, "mv") == 0)
		{
			p = strtok(NULL, " ");
			mv_path = strdup(p);
			p = strtok(NULL, " ");
			if(p == NULL)
			{
				fprintf(ferr, "mv %s: missing operand\n", mv_path);
			}
			else
			{
				mv(&arb, curent_directory, mv_path, p, ferr);
			}
		}

		else if(strcmp(p, "touch") == 0) 
		{	
			p = strtok(NULL, " ");
			while (p != NULL) 
			{
				mkdir_touch(&arb, p, curent_directory, ferr, 1);
				p = strtok(NULL, " ");
			}
		}
		else if(strcmp(p, "cp") == 0) 
		{	
			p = strtok(NULL, " ");
			mv_path = strdup(p);
			p = strtok(NULL, " ");
			cp(&arb, mv_path, p, curent_directory, ferr);
		}
		else if(strcmp(p, "rm") == 0)
		{
			p = strtok(NULL, " ");
			while (p != NULL) 
			{
				rm(&arb, p, curent_directory, ferr);
				p = strtok(NULL, " ");
			}
		}
		else if(strcmp(p, "rmdir") == 0)
		{
			p = strtok(NULL, " ");
			while (p != NULL) 
			{
				rmdir(&arb, p, curent_directory, ferr);
				p = strtok(NULL, " ");
			}
		}
		else
		{
			fprintf(ferr, "%s: command not found\n", p);
		}
	}
	free_arb(&arb);
    free(filenamein);
    free(filenameout);
    free(filenameerr);
    free(curent_directory);
    free(ls_directory);
    free(mv_path);
    fclose(fout);
    fclose(ferr);
    fclose(fin);
	return 0;
}


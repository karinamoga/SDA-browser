/*MOGA KARINA 311 CB*/
#include "structuri.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// functia cauta cel mai mare id din celelate taburi si returneaza id nou mai mare ca 1
int GenereazaIDNou(TTabList lista) {
	int max_id = -1;  // presupunem ca nu ex niciun tab
	TTabList p = lista->next; // pointer p care pointeaza spre primul element din lidts de dupa santinela
// parcurgem lista si gasim id cel mai mare
	while (p != lista) { // pana cand ajungem la santinela
		if (p->tab->id > max_id)
			max_id = p->tab->id;
		p = p->next;
	} 

	return max_id + 1;
}


// insereaza o celula noua la finalul listei inainte de santinela
TTabList InserareFinalTab(TTabList lista, TTab *tab) {
	TTabList nou = CreeazaCelulaTab(tab);
	if (!nou) return NULL;
// legaturi dintre nou santinela si ultimul element
	nou->next = lista;
	nou->prev = lista->prev;
	lista->prev->next = nou;
	lista->prev = nou;

	return nou;
}

// TASK 1 NEW TAB
// creeaza un tab nou, il insereaza in lista si setare activ
int NewTab(TBrowser *br, TPage *pagina) {
	if (!br || !pagina) return 0;

	int id_nou = GenereazaIDNou(br->list); // generam id unic
	TTab *tab_nou = CreeazaTab(id_nou, pagina);
	if (!tab_nou) return 0;

	TTabList cel = InserareFinalTab(br->list, tab_nou);
	if (!cel) {
		free(tab_nou);
		return 0;
	}

	//setez current_tab pe tabul legat in lista ( devine activ)
	br->current_tab = cel->tab;

	return 1;
}



// task 2  close
// inchide tabul curent, il elimina si dupa setare tab stanga ca activ
int CloseTab(TBrowser *br, FILE *fout) {
	if (!br || !br->current_tab || !br->list) return 0; 

	// nu putem elimina tabul cu 0 
	if (br->current_tab->id == 0) {
		fprintf(fout, "403 Forbidden\n");
		return 0;
	}

	TTabList p = br->list->next;

	while (p != br->list) {
		if (p->tab == br->current_tab) { // cautam in lista tabul nostru curent din browser
			TTabList stanga = p->prev;

			// Eliminare din lista
			p->prev->next = p->next;
			p->next->prev = p->prev;

			// daca lista ramane goala dupa stergere
			if (br->list->next == br->list) {
				br->current_tab = NULL;
			} else {
				// daca stanga este santinela, luam tabul din dreapta
				if (stanga == br->list) // daca tabul din stanga e santinela 
					br->current_tab = br->list->next->tab;
				else
					br->current_tab = stanga->tab;
			}

			FreeTab(p->tab);
			free(p);
			return 1;
		}
		p = p->next;
	}
	return 0;
}



// task 9 print
// afiseaza idurile taburilor circulare si descrierea paginii curente 
void PrintTabs(TBrowser *br, FILE *fout) {
	if (!br || !br->list || !br->current_tab)
		return;
// cautam celula curenta in lista
	TTabList start = br->list->next;

	// gasim celula in care se afla tabul curent
	while (start != br->list) {
		if (start->tab == br->current_tab)
			break;
		start = start->next;
	}

	// pornim de la tabul curent si  mergem circular spre dreapta
	TTabList p = start;
	do {
		fprintf(fout, "%d ", p->tab->id);
		p = p->next;
		if (p == br->list) p = p->next; // sari peste santinela
	} while (p != start);

	fprintf(fout, "\n%s\n", br->current_tab->current_page->description);
}


// 	task 3 open id 
// deschiode tabul cu id dat daca exista 
int OpenTab(TBrowser *br, int id, FILE *fout) {
	if (!br || !br->list) return 0;

	TTabList p = br->list->next;
	while (p != br->list) {
		if (p->tab->id == id) {
			br->current_tab = p->tab;
			return 1;
		}
		p = p->next;
	}

	fprintf(fout, "403 Forbidden\n");
	return 0;
}


// schimb tabul curent cu urmatorul din lista 
void NextTab(TBrowser *br) {
	if (!br || !br->list || !br->current_tab)
		return;

	TTabList p = br->list->next;
	while (p != br->list) {
		if (p->tab == br->current_tab) {
			// Treci la următorul nod
			if (p->next != br->list) {
				br->current_tab = p->next->tab;
			} else {
				// dacă următorul este santinela, sari peste ea
				br->current_tab = br->list->next->tab;
			}
			return;
		}
		p = p->next;
	}
}
// schimba cu cel anterior, opusu lu next 
void PrevTab(TBrowser *br) {
	if (!br || !br->list || !br->current_tab)
		return;

	TTabList p = br->list->next;
	while (p != br->list) {
		if (p->tab == br->current_tab) {
			// mergem spre stânga (prev), sărim peste santinelă dacă e nevoie
			if (p->prev != br->list) {
				br->current_tab = p->prev->tab;
			} else {
				br->current_tab = br->list->prev->tab;
			}
			return;
		}
		p = p->next;
	}
}

// TASK 6 page id
//Deschide o pagină nouă în tabul curent și actualizează istoricul de navigare (backward și forward).

void DeschidePagina(TBrowser *br, TPage *noua_pagina) {
	if (!br || !br->current_tab || !noua_pagina)
		return;

	TTab *tab = br->current_tab;

	// salvam pagina curenta in stiva backward
	TCellStack *aux = (TCellStack*)malloc(sizeof(TCellStack)); // se creeaza un nou nod in stiva back
	if (!aux) return;
	aux->page = tab->current_page; // salvam pagina care era deja curenta
	// push in stiva
	aux->next = tab->backward; // va deveni noul varf
	tab->backward = aux; 

	// golim complet stiva forward
	// pt ca vizitez o pagina noua nu mai merg in forward
	TStack tmp;
	while (tab->forward) {
		tmp = tab->forward;
		tab->forward = tab->forward->next;
		free(tmp);
	}

	// actualizam pagina curenta
	tab->current_page = noua_pagina;
}

// task 7 backward
// revine la pagina anteripara
void Backward(TBrowser *br, FILE *fout) {
	if (!br || !br->current_tab)
		return;

	TTab *tab = br->current_tab;

	if (!tab->backward) { // daca stiva e goala
		fprintf(fout, "403 Forbidden\n");
		return;
	}

	// scoatem pagina din stiva backward
	TCellStack *pop = tab->backward;
	tab->backward = tab->backward->next;

	// adaugam pagina curenta in stiva forward
	TCellStack *new_node = (TCellStack *)malloc(sizeof(TCellStack));
	if (!new_node) return;
	new_node->page = tab->current_page;
	new_node->next = tab->forward;
	tab->forward = new_node;

	// Seteaza pagina scoasa ca pagina curenta
	tab->current_page = pop->page;
	free(pop);
}



//task 8 forward
void Forward(TBrowser *br, FILE *fout) {
	if (!br || !br->current_tab)
		return;

	TTab *tab = br->current_tab;

	if (!tab->forward) {
		fprintf(fout, "403 Forbidden\n");
		return;
	}

	// scoate pagina din stiva forward
	TCellStack *pop = tab->forward; 
	tab->forward = tab->forward->next;

	// adaugă pagina curenta în stiva backward
	TCellStack *new_node = (TCellStack *)malloc(sizeof(TCellStack));
	if (!new_node) return;
	new_node->page = tab->current_page;
	new_node->next = tab->backward;
	tab->backward = new_node;

	//  setează pagina scoasa ca pagina curenta
	tab->current_page = pop->page;
	free(pop);
}



// backward dupa curenta dupa forawrd
void PrintHistory(TBrowser *br, int id, FILE *fout) {
	if (!br || !br->list)
		return;

	// Cauta tabul cu ID-ul dat
	TTabList p = br->list->next;
	while (p != br->list) {
		if (p->tab->id == id)
			break;
		p = p->next;
	}

	if (p == br->list) {
		fprintf(fout, "403 Forbidden\n");
		return;
	}

	TTab *tab = p->tab;

	// afiseaza forward de la baza la varf
	// mai intai copiem în stiva auxiliara ca sa le putem afisa in ordine
	TStack aux = NULL, tmp = tab->forward;
	while (tmp) {
		TStack nou = (TStack)malloc(sizeof(TCellStack));
		nou->page = tmp->page;
		nou->next = aux;
		aux = nou;
		tmp = tmp->next;
	}
	while (aux) {
		fprintf(fout, "%s\n", aux->page->url);
		TStack de_sters = aux;
		aux = aux->next;
		free(de_sters);
	}

	// 3. Afișează pagina curentă
	fprintf(fout, "%s\n", tab->current_page->url);

	// 4. Afișează BACKWARD de la vârf la bază (normal)
	tmp = tab->backward;
	while (tmp) {
		fprintf(fout, "%s\n", tmp->page->url);
		tmp = tmp->next;
	}
}



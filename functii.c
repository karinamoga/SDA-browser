/*MOGA KARINA 311 CB*/
#include "structuri.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// creeaza pag noua
TPage* CreeazaPagina(int id, const char *url, const char *descriere) {
	TPage *p = (TPage *)malloc(sizeof(TPage));
	if (!p) return NULL;

	p->id = id;
	strncpy(p->url, url, sizeof(p->url) - 1); // copiem url 
	p->url[sizeof(p->url) - 1] = '\0';

	p->description = (char *)malloc(strlen(descriere) + 1); // alocare spatiu pt descriere + copiere
	if (!p->description) {
		free(p);
		return NULL;
	}
	strcpy(p->description, descriere);

	return p;
}


// creeaza un tab nou care comntine o pagina
TTab* CreeazaTab(int id, TPage *pagina) {
	TTab *tab = (TTab *)malloc(sizeof(TTab)); // alocam memorie pt noul tab
	if (!tab) return NULL;

	tab->id = id;
	tab->current_page = pagina;
    // stivele s goale la inceput
	tab->backward = NULL;
	tab->forward = NULL;

	return tab;
}

// creeaza o celula dintr o lista circulara cu santinela care contine un tab
TTabList CreeazaCelulaTab(TTab *tab) {
	TTabList cel = (TTabList)malloc(sizeof(TCellTab));
	if (!cel) return NULL;

	cel->tab = tab;
    // nu  e legata inca in lista, prev si next sunt lg de ea insdasi
	cel->prev = cel; 
	cel->next = cel;
// sa iab ambele legaturi duse spre ea insasi 
	return cel;
}


// creeaza si returneaza santinela unei liste circ de taburi
TTabList InitListaTaburi() {
	TTabList sentinel = (TTabList)malloc(sizeof(TCellTab));
	if (!sentinel) return NULL;

	sentinel->tab = NULL; // daca nu e tab valid
	sentinel->prev = sentinel;
	sentinel->next = sentinel;

	return sentinel;
}
// initializez struct browserului
TBrowser* InitBrowser() {
	TBrowser *br = (TBrowser *)malloc(sizeof(TBrowser));
	if (!br) return NULL;
// alocare lista taburi
	br->list = InitListaTaburi();
	if (!br->list) {
		free(br);
		return NULL;
	}

	br->current_tab = NULL; // nu ex tab cyrent pana se creeaza
	return br;
}
// distrg stiva backward sau forward
void DistrugeStiva(TStack *vf) {
	TStack aux;
	while (*vf) {
		aux = *vf;
		*vf = (*vf)->next;
		free(aux);
	}
}
// elibereaza tab, inclusiv stivele
void FreeTab(TTab *tab) {
	if (!tab) return;
	DistrugeStiva(&tab->backward);
	DistrugeStiva(&tab->forward);
	free(tab); // pagina curenta nu o eliberam,ci o data in main
}
// cauta pagina dupa id
TPage* GasestePagina(TPage **pagini, int n, int id) {
	for (int i = 0; i < n; i++) {
		if (pagini[i]->id == id)
			return pagini[i];
	}
	return NULL;
}
// elibereaza toata lista circulara de taburi
void FreeListaTaburi(TTabList lista) {
	if (!lista) return;

	TTabList p = lista->next, urm;
	while (p != lista) {
		urm = p->next;
		FreeTab(p->tab);  // elibereaza si stiveke
		free(p);          // elibereaza celula
		p = urm;
	}
	free(lista); // elibereaza santinela
}
void FreeBrowser(TBrowser *br) {
	if (!br) return;
	FreeListaTaburi(br->list);
	free(br);
}


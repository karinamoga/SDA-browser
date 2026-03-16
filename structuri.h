/*MOGA KARINA 311 CB*/
#ifndef STRUCTURI_H
#define STRUCTURI_H

#include <stdio.h>

// Pagina si este folosita in fiecare tab ca pagina curenta sau in istoric ( backword/ forward)
typedef struct {
	int id;
	char url[150];
	char *description;
} TPage; // este o pagina web



// stiva
// o celula dintr o stiva fol pt istoricul paginilor in fiecare tab
typedef struct cell_stack {
	TPage *page;  // pointer catre pagina salvata in istoric
	struct cell_stack *next; // pointer catre urm nod din stiva adica urm pagina
} TCellStack, *TStack; // TCellStack e nodu din stiva si Tstack e adresa adica stiva in sine



// Tab 
// fiecare tab are o pagina curenta si 2 stive back si forward
typedef struct {
	int id;
	TPage *current_page;
	TStack backward;
	TStack forward;
} TTab;



// o lista dublu inlantuita in care sunt legate celule de taburi
typedef struct list_cell {
	TTab *tab;  // pointer catre un tab
	struct list_cell *prev, *next;  // tab stanga, dreapta
} TCellTab, *TTabList;

// browser
typedef struct {
	TTab *current_tab; // pointer catre tabul activ 
	TTabList list; // lista circulara cu santinela care are toate taburile deschise
} TBrowser;



// Alocare
TPage* CreeazaPagina(int id, const char *url, const char *descriere);
TTab* CreeazaTab(int id, TPage *pagina);
TTabList CreeazaCelulaTab(TTab *tab);
TTabList InitListaTaburi();
TBrowser* InitBrowser();
void DistrugeStiva(TStack *vf);
void FreeTab(TTab *tab);
void PrintTabs(TBrowser *br, FILE *fout);
TPage* GasestePagina(TPage **pagini, int n, int id);
void FreeListaTaburi(TTabList lista);
void FreeBrowser(TBrowser *br);


// Taskuri
int NewTab(TBrowser *br, TPage *pagina);
int CloseTab(TBrowser *br, FILE *fout);
int OpenTab(TBrowser *br, int id, FILE *fout);
void NextTab(TBrowser *br);
void PrevTab(TBrowser *br);
void DeschidePagina(TBrowser *br, TPage *noua_pagina);
void Backward(TBrowser *br, FILE *fout);
void Forward(TBrowser *br, FILE *fout);
void PrintHistory(TBrowser *br, int id, FILE *fout);

#endif

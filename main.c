/*MOGA KARINA 311 CB*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structuri.h"

#define MAX_LINE 256
#define MAX_PAGINI 50

int main() {
	FILE *fin = fopen("tema1.in", "r");
	FILE *fout = fopen("tema1.out", "w");

	if (!fin || !fout)
		return 1;

	//citesc paginile 
	int nr_pagini;
	fscanf(fin, "%d\n", &nr_pagini);

	TPage **pagini = malloc(nr_pagini * sizeof(TPage*)); // alocare vector la pagini
	if (!pagini) return 1;

	for (int i = 0; i < nr_pagini; i++) {
		int id;
		char url[150];
		char descriere[MAX_LINE];

		fscanf(fin, "%d\n", &id);
		fgets(url, sizeof(url), fin);
		url[strcspn(url, "\n")] = '\0'; // scot \n

		fgets(descriere, sizeof(descriere), fin);
		descriere[strcspn(descriere, "\n")] = '\0';

		pagini[i] = CreeazaPagina(id, url, descriere); // creez pagina si o bag in vector
	}

	//pagina impl
	TPage *paginaImplicita = CreeazaPagina(0, "https://acs.pub.ro/", "Computer Science");

	// init browser
	TBrowser *browser = InitBrowser(); // alocam struct browser si dupa inseram primul tab
	NewTab(browser, paginaImplicita);

	//comenzi
	int nrcomenzi;
	fscanf(fin, "%d\n", &nrcomenzi);
	char comanda[MAX_LINE];

	for (int i = 0; i < nrcomenzi; i++) {
		fgets(comanda, sizeof(comanda), fin);
		comanda[strcspn(comanda, "\n")] = '\0';

		if (strcmp(comanda, "NEW_TAB") == 0) {
			NewTab(browser, paginaImplicita);
		}
        else if (strcmp(comanda, "CLOSE") == 0) {
            CloseTab(browser, fout);
        }
        else if (strncmp(comanda, "OPEN ", 5) == 0) {
            int id = atoi(comanda + 5);
            OpenTab(browser, id, fout);
        }
        
		else if (strcmp(comanda, "PRINT") == 0) {
			PrintTabs(browser, fout);
		}
		else if (strcmp(comanda, "NEXT") == 0) {
			NextTab(browser);
		}
		else if (strcmp(comanda, "PREV") == 0) {
			PrevTab(browser);
		}
		else if (strncmp(comanda, "PAGE ", 5) == 0) {
			int id = atoi(comanda + 5); // extragem id 
			TPage *p = GasestePagina(pagini, nr_pagini, id); // cautam pag cu id dorit
			if (!p) {
				fprintf(fout, "403 Forbidden\n"); // pagina nu exista
			} else {
				DeschidePagina(browser, p);
			}
		}
		else if (strcmp(comanda, "BACKWARD") == 0) {
			Backward(browser, fout);
		}
		else if (strcmp(comanda, "FORWARD") == 0) {
			Forward(browser, fout);
		}
		else if (strncmp(comanda, "PRINT_HISTORY ", 14) == 0) {
			int id = atoi(comanda + 14);
			PrintHistory(browser, id, fout);
		}
		// TODO: alte comenzi gen CLOSE, OPEN (dacă le implementezi separat)
	}

	// eliberare pagini
	for (int i = 0; i < nr_pagini; i++) {
		if (pagini[i]) {
			free(pagini[i]->description);
			free(pagini[i]);
		}
	}
	free(pagini);
    // pagina implicita
	free(paginaImplicita->description);
	free(paginaImplicita);

	// browser
	FreeBrowser(browser);

	fclose(fin);
	fclose(fout);
	return 0;
}

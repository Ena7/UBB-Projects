#include "Iterator.h"
#include "DO.h"
#include <iostream>
#include <cmath>

#include <exception>
using namespace std;

//teta(CAP)
DO::DO(Relatie r) {
	lg = 0;
	rel = r;
	for (int i = 0; i < CAP; i++) {
		tabel[i] = nullptr;
	}
}

//adauga o pereche (cheie, valoare) in dictionar
//daca exista deja cheia in dictionar, inlocuieste valoarea asociata cheii si returneaza vechea valoare
//daca nu exista cheia, adauga perechea si returneaza null

//Best: teta(1) - primul nod dintr-o lista inlantuita
//Worst: O(n) - toate disperseaza in aceeasi locatie / teta(n) - elementul nu a fost gasit
//Avg: O(1)
//Overall: O(n)
TValoare DO::adauga(TCheie c, TValoare v) {
	int cheie = abs(c % CAP);
	Nod* nod_curent = tabel[cheie];
	while (nod_curent != nullptr) {
		if (nod_curent->e.first == c) {
			TValoare vechi = nod_curent->e.second;
			nod_curent->e.second = v;
			return vechi;
		}
		nod_curent = nod_curent->urm;
	}
	Nod* nod_nou = new Nod(TElem(c, v), nullptr);
	lg++;
	nod_nou->urm = tabel[cheie];
	tabel[cheie] = nod_nou;
	return NULL_TVALOARE;
}

//cauta o cheie si returneaza valoarea asociata (daca dictionarul contine cheia) sau null

//Best: teta(1) - primul nod dintr-o lista inlantuita
//Worst: O(n) - toate disperseaza in aceeasi locatie / teta(n) - elementul nu a fost gasit
//Avg: O(1)
//Overall: O(n)
TValoare DO::cauta(TCheie c) const {
	int cheie = abs(c % CAP);
	Nod* nod_curent = tabel[cheie];
	while (nod_curent != nullptr) {
		if (nod_curent->e.first == c) {
			return nod_curent->e.second;
		}
		nod_curent = nod_curent->urm;
	}
	return NULL_TVALOARE;
}

//sterge o cheie si returneaza valoarea asociata (daca exista) sau null

//Best: teta(1) - primul nod dintr-o lista inlantuita
//Worst: O(n) - toate disperseaza in aceeasi locatie / teta(n) - elementul nu a fost gasit
//Avg: O(1)
//Overall: O(n)
TValoare DO::sterge(TCheie c) {
	int cheie = abs(c % CAP);
	//verific ca exista o lista inlantuita la cheia respectiva
	if (tabel[cheie] == nullptr) {
		return NULL_TVALOARE;
	}
	//verific daca e primul nod din lista inlantuita
	if (tabel[cheie]->e.first == c) {
		TValoare valoare = tabel[cheie]->e.second;
		Nod* nod_sters = tabel[cheie];
		tabel[cheie] = tabel[cheie]->urm;
		delete nod_sters;
		lg--;
		return valoare;
	}
	//incepand de la al doilea nod pana la ultimul nod
	Nod* nod_curent = tabel[cheie];
	while (nod_curent->urm != nullptr) {
		if (nod_curent->urm->e.first == c) {
			TValoare valoare = nod_curent->urm->e.second;
			Nod* nod_sters = nod_curent->urm;
			nod_curent->urm = nod_curent->urm->urm;
			delete nod_sters;
			lg--;
			return valoare;
		}
		nod_curent = nod_curent->urm;
	}
	return NULL_TVALOARE;
}

//returneaza numarul de perechi (cheie, valoare) din dictionar

//teta(1)
int DO::dim() const {
	return lg;
}

//verifica daca dictionarul e vid

//teta(1)
bool DO::vid() const {
	return lg == 0;
}

//teta(1)
Iterator DO::iterator() const {
	return  Iterator(*this);
}

//teta(CAP*n)
DO::~DO() {
	for (int i = 0; i < CAP; i++) {
		while (tabel[i] != nullptr) {
			Nod* nod = tabel[i];
			tabel[i] = tabel[i]->urm;
			delete nod;
		}
	}
}
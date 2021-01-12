#include "Iterator.h"
#include "DO.h"
#include <exception>

using namespace std;

//teta(n^2)
Iterator::Iterator(const DO& d) : dict(d) {
	curent = 0;
	lista = new TElem[dict.lg];
	
	//teta(CAP*n)
	int index = 0; //index curent lista
	for (int i = 0; i < CAP; i++) {
		if (dict.tabel[i] != nullptr) {
			Nod* nod_curent = dict.tabel[i];
			while (nod_curent != nullptr) {
				lista[index++] = nod_curent->e;
				nod_curent = nod_curent->urm;
			}
		}
	}
	//teta(n^2)
	for (int i = 0; i < dict.lg - 1; i++) {
		for (int j = i + 1; j < dict.lg; j++) {
			if (!dict.rel(lista[i].first, lista[j].first)) {
				std::swap(lista[i], lista[j]);
			}
		}
	}
}

//teta(1)
void Iterator::prim() {
	curent = 0;
}

//teta(1)
void Iterator::urmator() {
	if (!valid()) {
		throw exception();
	}
	curent++;
}

//teta(1)
bool Iterator::valid() const {
	return curent < dict.lg;
}

//teta(1)
TElem Iterator::element() const {
	if (!valid()) {
		throw exception();
	}
	return lista[curent];
}

//teta(n)
Iterator::~Iterator() {
	delete[] lista;
}


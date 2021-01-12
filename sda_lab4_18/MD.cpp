#include "MD.h"
#include "IteratorMD.h"
#include <exception>
#include <iostream>

using namespace std;

//Teta(1)
MD::MD() {
	cap = 2;
	lg = 0;
	prim = -1;
	ultim = -1;
	primLiber = 0;
	urm = new int[cap];
	prec = new int[cap];
	e = new TElem[cap];
	//inceputul listei spatiului liber
	urm[0] = 1;
	//sfarsitul listei spatiului liber
	urm[1] = -1;
}

//teta(n) (lista de dimensiunea n se va redimensiona la 2n; initializarea spatiului suplimentar va necesita tot n pasi)
void MD::redim() {
	int cap_nou = cap * 2;
	int* urm_nou = new int[cap_nou];
	int* prec_nou = new int[cap_nou];
	TElem* e_nou = new TElem[cap_nou];
	memcpy(urm_nou, urm, cap * sizeof(int));
	memcpy(prec_nou, prec, cap * sizeof(int));
	memcpy(e_nou, e, cap * sizeof(TElem));
	cap = cap_nou;
	delete[] urm; 
	delete[] prec; 
	delete[] e;
	urm = urm_nou; prec = prec_nou; e = e_nou;
	//dupa redimensionare, initializez lista spatiului liber de la capatul listei vechi pana la capatul liste noi
	for (int i = cap / 2; i < cap - 1; i++) {
		urm[i] = i + 1;
	}
	urm[cap - 1] = -1;
	primLiber = cap / 2;

}

//Teta(1)
//sterge primul nod din lista spatiului liber
int MD::aloca() {
	int i = primLiber;
	primLiber = urm[primLiber];
	return i;
}

//Teta(1)
//se adauga la i inceputul listei spatiului liber
void MD::dealoca(int i) {
	urm[i] = primLiber;
	primLiber = i;
}

//best: teta(1) - redimensionarea nu are loc
//worst: teta(n) - redimensionarea are loc
//average: teta(n) ??
//overall: O(n)
int MD::creeazaNod(TElem elem) {
	if (primLiber == -1) {
		redim();
	}
	int i = aloca();
	e[i] = elem;
	urm[i] = -1;
	prec[i] = -1;
	return i;
}

//Teta(1)
void MD::adauga(TCheie c, TValoare v) {
	int nou = creeazaNod(make_pair(c, v));
	urm[nou] = prim;
	prec[nou] = -1;
	//daca exista un element urmator
	if (urm[nou] != -1) {
		prec[urm[nou]] = nou;
	}
	prim = nou;
	//daca nu exista in lista alte elemente
	if (lg == 0) {
		ultim = prim;
	}
	lg++;
}

//best: teta(1) - primul element din lista
//worst: teta(n) - ultimul element din lista
//average: teta(n)
//overall: O(n)
bool MD::sterge(TCheie c, TValoare v) {
	TElem elem = make_pair(c, v);
	//daca lista nu are elemente
	if (lg == 0) {
		return false;
	}
	//daca este primul element din lista
	if (e[prim] == elem) {
		int p = prim;
		//daca exista un element urmator
		if (urm[prim] != -1) {
			prec[urm[prim]] = -1;
		}
		prim = urm[prim];
		//daca este singurul element din lista
		if (lg == 1) {
			ultim = -1;
		}
		dealoca(p);
		lg--;
		return true;
	}
	int nod_curent = urm[prim];
	//se executa daca exista cel putin doua noduri
	while (nod_curent != -1) {
		if (e[nod_curent] == elem) {
			//daca este ultimul element din lista
			if (nod_curent == ultim) {
				ultim = prec[nod_curent];
				urm[prec[nod_curent]] = -1;
			}
			//daca elementul nu este nici primul, nici ultimul
			else {
				urm[prec[nod_curent]] = urm[nod_curent];
				prec[urm[nod_curent]] = prec[nod_curent];
			}
			dealoca(nod_curent);
			lg--;
			return true;
		}
		nod_curent = urm[nod_curent];
	}
	return false;
}

//teta(n)
vector<TValoare> MD::cauta(TCheie c) const {
	vector<TValoare> valori;
	int nod_curent = prim;
	while (nod_curent != -1) {
		if (e[nod_curent].first == c) {
			valori.push_back(e[nod_curent].second);
		}
		nod_curent = urm[nod_curent];
	}
	return valori;
}

//teta(1)
int MD::dim() const {
	return lg;
}

//teta(1)
bool MD::vid() const {
	return lg == 0;
}

//teta(1)
IteratorMD MD::iterator() const {
	return IteratorMD(*this);
}

//teta(1)
MD::~MD() {
	delete[] urm;
	delete[] prec;
	delete[] e;
}

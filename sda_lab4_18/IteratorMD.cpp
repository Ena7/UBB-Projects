#include "IteratorMD.h"
#include "MD.h"

using namespace std;

//teta(1)
IteratorMD::IteratorMD(const MD& _md) : md(_md) {
	curent = md.prim;
}

//teta(1)
TElem IteratorMD::element() const {
	if (!valid()) {
		throw exception();
	}
	return md.e[curent];
}

//teta(1)
bool IteratorMD::valid() const {
	return curent != -1;
}

//teta(1)
void IteratorMD::urmator() {
	if (!valid()) {
		throw exception();
	}
	curent = md.urm[curent];
}

//teta(1)
void IteratorMD::prim() {
	curent = md.prim;
}

//best: teta(1)
//worst: teta(n)
//average: teta(n) ??
//overall: O(n)
void IteratorMD::avanseazaKPasi(int k) {
	if (k <= 0) {
		throw exception();
	}
	for (int i = 0; i < k; i++) {
		urmator();
	}
	if (!valid()) {
		throw exception();
	}
}

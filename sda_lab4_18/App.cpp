#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  

#include <iostream>
#include "TestExtins.h"
#include "TestScurt.h"
#include "MD.h"
#include "IteratorMD.h"
#include <assert.h>

using namespace std;

int main() {
	{
		testAll();
		testAllExtins();

		MD m;
		m.adauga(1, 100);
		m.adauga(2, 200);
		m.adauga(3, 300);
		m.adauga(1, 500);
		m.adauga(2, 600);
		m.adauga(4, 800);
		IteratorMD im = m.iterator();
		im.element();
		im.urmator();
		im.element();
		im.avanseazaKPasi(3);
		im.element();
		try { im.avanseazaKPasi(2); }
		catch (...) { cout << "Exceptie\n"; }
	}
	_CrtDumpMemoryLeaks();
	cout << "End";
	return 0;
}
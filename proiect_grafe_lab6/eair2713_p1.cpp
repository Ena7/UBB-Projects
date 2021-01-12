#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>

#include <fstream>
#include <vector>
using std::vector;
#include <algorithm>
#include <string>
using std::string;

class Nod {
public:
	int numar;
	int grad;
	int culoare;
	vector<Nod*> noduri_adia;
	Nod(int numar, int grad, int culoare) :numar{ numar }, grad{ grad }, culoare{ culoare } {}
};

class Muchie {
public:
	int x;
	int y;
};

class Graf {
public:
	int N;
	int M;
	vector<Muchie> muchii;
	vector<Nod*> noduri;
	Graf(int N, int M) : N{ N }, M{ M } {}
	~Graf() {
		for (const auto& nod : noduri) {
			delete nod;
		}
	}
};

void colorare(const string& file_in, const string& file_out) {


	std::ifstream fin; fin.open(file_in);
	std::ofstream fout; fout.open(file_out);

	int N, M;
	fin >> N >> M;
	Graf G{ N, M };

	Muchie muchie;
	while (fin >> muchie.x >> muchie.y) {
		G.muchii.push_back(muchie);
	}
	//fiecare nod va avea implicit gradul 0 si culoarea -1
	for (int i = 0; i < G.N; i++) {
		G.noduri.push_back(new Nod{ i, 0, -1 });
	}
	//setam gradul, respectiv nodurile adiacente fiecarui nod
	for (const auto& muchie : G.muchii) {
		G.noduri[muchie.x]->grad++;
		G.noduri[muchie.y]->grad++;
		G.noduri[muchie.x]->noduri_adia.push_back(G.noduri[muchie.y]);
		G.noduri[muchie.y]->noduri_adia.push_back(G.noduri[muchie.x]);
	}
	//sortez nodurile dupa grad, descrescator
	std::sort(G.noduri.begin(), G.noduri.end(), [](const auto& a, const auto& b) { return a->grad > b->grad; });

	int colorari = 0;
	int C = 0;
	while (colorari < G.N) {
		//parcurgem nodurile in ordinea gradului
		for (auto& nod : G.noduri) {
			//daca nodul nu are culoare, iar nodurile adiacente nu au culoarea curenta
			if (nod->culoare == -1 && std::find_if(nod->noduri_adia.begin(), nod->noduri_adia.end(), [&C](const auto n) { return n->culoare == C; }) == nod->noduri_adia.end()) {
				nod->culoare = C;
				colorari++;
			}
		}
		//trecem la urmatoarea culoare
		C++;
	}
	fout << C << "\n";
	std::sort(G.noduri.begin(), G.noduri.end(), [](const auto& a, const auto& b) { return a->numar < b->numar; });
	for (const auto& nod : G.noduri) {
		fout << nod->culoare << " ";
	}

	fin.close();
	fout.close();
}

int main() {
	colorare("g2_in.txt", "g2_out.txt");

	_CrtDumpMemoryLeaks();
	return 0;
}
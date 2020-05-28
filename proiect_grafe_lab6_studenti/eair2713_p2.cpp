#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>

#include <fstream>
#include <algorithm>
#include <vector>
using std::vector;
#include <string>
using std::string;
#include <queue>
using std::queue;
#include <map>

class Nod {
public:
	int numar;
	vector<int> noduri_adia;
	Nod(int numar) :numar{ numar } {}
};

class Muchie {
public:
	int x;
	int y;
	int cap;
	Muchie(int x, int y, int cap) :x{ x }, y{ y }, cap{ cap } {}
};

class Graf {
public:
	int N;
	int camine;
	int M;
	vector<Muchie> muchii;
	vector<Nod> noduri;
	Graf(int N, int camine, int M) : N{ N }, camine{ camine }, M{ M } {}
};

//cu ajutorul parcurgerii in latime, cautam o cale de la supersursa catre destinatie
bool BFS(vector<int>& parinti, Graf& graf) {
	vector<bool> vizitat;
	//setam fiecare nod ca nevzitat
	for (int i = 0; i < graf.N; i++) {
		vizitat.push_back(false);
	}
	queue<Nod> coada;
	//initial punem supersursa in coada, avand parintele -1 si o marcam ca vizitat
	coada.push(graf.noduri[graf.N - 1]);
	vizitat[graf.noduri[graf.N - 1].numar] = true;
	parinti[graf.noduri[graf.N - 1].numar] = -1;
	while (!coada.empty()) {
		Nod nod = coada.front();
		coada.pop();
		for (const auto& n : nod.noduri_adia) {
			if (vizitat[n] == false) {
				coada.push(graf.noduri[n]);
				parinti[n] = nod.numar;
				vizitat[n] = true;
			}
		}
	}
	//daca destinatia nu a fost vizitata, inseamna ca nu s-a gasit o cale
	return vizitat[graf.noduri[graf.N - 2].numar] == true;
}

void studenti(const string& file_in, const string& file_out) {

	std::ifstream fin; fin.open(file_in);
	std::ofstream fout; fout.open(file_out);

	int N, camine, M;
	fin >> N >> camine >> M;
	//vom adauga grafului supersursa, si vom adauga muchii de la supersursa la surse atatea cate surse sunt
	Graf G{ N + 1, camine, M + camine };
	std::map<int, int> copie_surse;

	int x, y, cap;
	while (fin >> x >> y >> cap) {
		G.muchii.push_back({ x,y,cap });
	}
	//muchiile de la supersursa la surse vor avea capacitate infinita
	for (int i = 0; i < camine; i++) {
		G.muchii.push_back({ G.N - 1,i,INT_MAX });
		copie_surse[i] = 0;
	}
	for (int i = 0; i < G.N; i++) {
		G.noduri.push_back(Nod{ i });
	}
	for (const auto& muchie : G.muchii) {
		G.noduri[muchie.x].noduri_adia.push_back(G.noduri[muchie.y].numar);
		if (muchie.x < camine) {
			//retin debitul maxim care poate iesi din fiecare nod sursa
			copie_surse[muchie.x] += muchie.cap;
		}
	}

	int debit_maxim = 0;

	//initializez vectorul de parinti
	vector<int> parinti;
	for (int i = 0; i < G.N; i++) {
		parinti.push_back(-2);
	}
	//parcurgerea in latime va returna fals daca nu s-a gasit o cale catre destinatie
	while (BFS(parinti, G)) {
		int bottleneck = INT_MAX;
		//parcurg calea gasita de la destinatie la supersursa si determin muchia cu capacitatea cea mai mica
		for (int nod_curent = G.N - 2; nod_curent != G.N - 1; nod_curent = parinti[nod_curent]) {
			int nod_parinte = parinti[nod_curent];
			Muchie& m = *std::find_if(G.muchii.begin(), G.muchii.end(), [&](const auto& it) { return nod_parinte == it.x && nod_curent == it.y; });
			bottleneck = std::min(bottleneck, m.cap);
		}
		//parcurg calea gasita de la destinatie la supersursa si scad capacitatea fiecarei muchii cu minimul gasit
		for (int nod_curent = G.N - 2; nod_curent != G.N - 1; nod_curent = parinti[nod_curent]) {
			int nod_parinte = parinti[nod_curent];
			Muchie& m = *std::find_if(G.muchii.begin(), G.muchii.end(), [&](const auto& it) { return nod_parinte == it.x && nod_curent == it.y; });
			m.cap -= bottleneck;
			//daca capacitatea unei muchii ajunge 0, consider ca acele 2 noduri nu mai sunt adiacente (sterg din lista de adiacente)
			for (const auto& much : G.muchii) {
				if (much.cap == 0) {
					G.noduri[much.x].noduri_adia.erase(std::remove(G.noduri[much.x].noduri_adia.begin(), G.noduri[much.x].noduri_adia.end(), much.y), G.noduri[much.x].noduri_adia.end());
				}
			}
		}
		//adaug cea mai mica capacitate la debitul maxim
		debit_maxim += bottleneck;
	}
	fout << debit_maxim << '\n';
	//din fiecare nod sursa, scad debitul maxim cu capcitatea muchiilor respective din graful rezidual
	for (const auto& muchie : G.muchii) {
		if (muchie.x < camine) {
			copie_surse[muchie.x] -= muchie.cap;
		}
	}
	for (const auto& surse : copie_surse) {
		fout << surse.second << " ";
	}

	fin.close();
	fout.close();
}

int main() {
	studenti("g4_in.txt", "g4_out.txt");

	_CrtDumpMemoryLeaks();
	return 0;
}
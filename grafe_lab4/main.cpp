#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
// ^ ^ verificare memory leaks ^ ^

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <memory>
#include <queue>
#include <unordered_map>
#include <limits>
using std::string;
using std::vector;
using std::cout;

struct Muchie {
    int extr1;
    int extr2;
};

//graf neorientat neponderat
class Graf {
public:
    int n;
    int adia[21][21] = {};
    vector<Muchie> m;

    //citeste din fisier
    Graf(const string fisier) {
        std::ifstream fin;  fin.open(fisier);
        fin >> n;
        Muchie muchie;
        while (fin >> muchie.extr1 >> muchie.extr2)
            m.push_back(muchie);

        for (const auto& muchie: m)
            adia[muchie.extr1][muchie.extr2] = adia[muchie.extr2][muchie.extr1] = 1; 

        fin.close();
    } 

    Graf(int noduri) : n{ noduri } {}

    void copie_adia(int temp[21][21]) {
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++)
                temp[i][j] = adia[i][j];
    }

    void afisare() {
        cout << std::endl;
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++)
                cout << adia[i][j] << " ";

            cout << std::endl;
        }
        cout << std::endl;
    }

    // ### CODARE PRUFER ###
    vector<int> codare_prufer(const int radacina) {
        int temp[21][21];
        copie_adia(temp);
        vector<int> codare; // K
        vector<int> varfuri; // T
        //varfurile sunt sortate crescator (fara radacina)
        for (int varf = 1; varf <= n; varf++)
            if (varf != radacina)
                varfuri.push_back(varf);

        while (varfuri.size() != 0) {
            for (int i = 1; i <= n; i++) {
                if (std::find(varfuri.begin(), varfuri.end(), i) == varfuri.end() || i == radacina) //este radacina sau nu s-a gasit
                    continue;

                int pos; //pozitia nodului adiacent (folosit daca i este frunza)
                int nr = 0; //calculez numarul de adiacente
                for (int j = n; j >= 1; j--)
                    if (temp[i][j] == 1) {
                        nr++;
                        pos = j;
                    }  

                if (nr == 1) { //este frunza
                    //stergem frunza din arbore
                    temp[i][pos] = 0;
                    temp[pos][i] = 0;
                    varfuri.erase(std::remove(varfuri.begin(), varfuri.end(), i), varfuri.end());
                    codare.push_back(pos); //adaug predecesorul frunzei
                    break;
                }
            }
        }
        return codare;
    }
};

struct MuchiePond {
    int extr1;
    int extr2;
    int cost;
};

//graf neorientat ponderat
class GrafPond {
public:
    int n;
    int adia[21][21] = {};
    vector<MuchiePond> m;

    GrafPond(int noduri) : n{ noduri } {}

    //citeste din fisier
    GrafPond(const string fisier) {
        std::ifstream fin; fin.open(fisier);
        fin >> n;
        MuchiePond muchie;
        while (fin >> muchie.extr1 >> muchie.extr2 >> muchie.cost)
            m.push_back(muchie);

        for (const auto& muchie : m)
            adia[muchie.extr1][muchie.extr2] = adia[muchie.extr2][muchie.extr1] = muchie.cost;
    }

    void copie_adia(int temp[21][21]) {
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++)
                temp[i][j] = adia[i][j];
    }

    void afisare() {
        cout << std::endl;
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++)
                cout << std::setw(2) << adia[i][j] << " ";

            cout << std::endl;
        }
        cout << std::endl;
    }

    // ### KRUSKAL ###
    GrafPond kruskal() {
        GrafPond arb{ n };
        std::unique_ptr<int[]> arbori = std::make_unique<int[]>(n + 1);
        //initial, fiecare nod apartine unui "arbore"
        for (int i = 1; i <= n; i++)
            arbori[i] = i;

        //sortez crescator dupa cost
        std::sort(m.begin(), m.end(), [](const auto& m1, const auto& m2) { return m1.cost < m2.cost; });

        for(auto const& muchie : m) //pentru fiecare muchie
            if (arbori[muchie.extr1] != arbori[muchie.extr2]) { //verific daca cele doua extremitati nu apartin aceluiasi arbore
                arb.adia[muchie.extr1][muchie.extr2] = arb.adia[muchie.extr2][muchie.extr1] = muchie.cost;
                int pos_extr1 = arbori[muchie.extr1];
                int pos_extr2 = arbori[muchie.extr2];
                for (int i = 1; i <= n; i++) //unesc cei doi arbori
                    if (arbori[i] == pos_extr2)
                        arbori[i] = pos_extr1;
            }
        return arb;
    }

    // ### PRIM ###
    GrafPond prim(int radacina) {
        GrafPond arb{ n };
        vector<int> Q; //lista de noduri nevizitate
        vector<int> U; //lista de noduri vizitate
        U.push_back(radacina);
        for (int i = 1; i <= n; i++)
            if(i != radacina)
                Q.push_back(i);

        while (Q.size() != 0) {
            int mini = std::numeric_limits<int>::max();
            int posi = 0, posj = 0;
            for (const auto& i : U) { //pentru fiecare nod vizitat
                for (int j = 1; j <= n; j++) {
                    //(i,j) este adiacent && costul este mai mic decat minimul && nodul j a fost nevizitat
                    if (adia[i][j] != 0 && adia[i][j] < mini && std::find(Q.begin(), Q.end(), j) != Q.end())  {
                        posi = i;
                        posj = j;
                        mini = adia[i][j];
                    }
                }
            }
            arb.adia[posi][posj] = arb.adia[posj][posi] = mini;
            U.push_back(posj);
            Q.erase(std::remove(Q.begin(), Q.end(), posj), Q.end()); 
        }
        return arb;
    }
};

//cod este K
// ### DECODARE PRUFER ###
Graf decodare_prufer(vector<int> cod, int noduri) {
    Graf graf{ noduri };
    for (int i = 1; i <= noduri - 1; i++) {
        int x = cod[0];  //primul element din K
        int y;  //cel mai mic numar natural nenul care nu se gaseste in K
        for (y = 1 ;; y++)
            if (std::find(cod.begin(), cod.end(), y) == cod.end())
                break;

        graf.adia[x][y] = graf.adia[y][x] = 1;
        cod.erase(cod.begin());
        cod.push_back(y);
    }
    return graf;
}

/*  \/ \/ \/ HUFFMAN \/ \/ \/  */

//litera va contine un caracter si o frecventa
struct Litera {
    char car;
    int fr;
    Litera() = default;
    Litera(char car, int fr) : car{ car }, fr{ fr } {}
};

//se citesc literele (fiecare avand o frecventa), mesajul dintr-un fisier
//alfabetul reprezinta un vector cu literele (si frecventa lor) existente in mesaj
void citeste_mesaj(vector<Litera>& alfabet, string& mesaj, const string fisier) {
    std::ifstream fin;  fin.open(fisier);
    int n;
    fin >> n;
    for (int i = 1; i <= n; i++) {
        Litera litera;
        fin >> litera.car >> litera.fr;
        alfabet.push_back(litera);
    }
    fin >> mesaj;
    fin.close();
}

//un varf din arborele binar contine adresa catre varful stang si varful drept, precum si litera
//frunza va avea la adresa varfului stang si drept nula
class Varf {
public:
    Varf* stanga;
    Varf* dreapta;
    Litera lit;

    Varf(Litera continut) {
        stanga = nullptr;
        dreapta = nullptr;
        lit = continut;
    }
};

//configuram coada cu prioritati pentru a ordona literele dupa frecventa 
//prima litera din coada are frecventa cea mai mica in coada, analog ultima litera
struct Compara {
    bool operator()(Varf* v1, Varf* v2) { return v1->lit.fr > v2->lit.fr; };
};

//codificam literele
//parcurgem recursiv arborele binar creat
void codifica_huffman(std::unordered_map<char, string>& dictionar, Varf* radacina, string cod) {
    if (radacina == nullptr)
        return;

    //daca varful contine o litera, adaguam la dictionar
    if (radacina->lit.car != '#')
        dictionar[radacina->lit.car] = cod;

    codifica_huffman(dictionar, radacina->stanga, cod + "0");
    codifica_huffman(dictionar, radacina->dreapta, cod + "1");

    delete radacina;
}

// ### CODARE HUFFMAN ###
void huffman(vector<Litera> alfabet, string mesaj) {
    Varf* stanga;
    Varf* dreapta;
    Varf* tatal;
    //literele sunt ordonate dupa frecventa intr-o coada cu prioritati
    std::priority_queue <Varf*, vector<Varf*>, Compara> Q;
    for (const auto& litera : alfabet)
        Q.push(new Varf(litera));

    while (Q.size() > 1) {
        //se extrag cele doua varfuri cu cele mai mici frecvente
        stanga = Q.top();
        Q.pop();

        dreapta = Q.top();
        Q.pop();

        //tatal va avea adresa celor doi fii (varfurile stanga si dreapta) si ca frecventa suma frecventelor lor
        //tatal nu va contine o litera de interes, asa ca vom marca acest lucru prin '#'
        tatal = new Varf(Litera('#', stanga->lit.fr + dreapta->lit.fr));
        tatal->stanga = stanga;
        tatal->dreapta = dreapta;

        //inseram tatal in coada
        Q.push(tatal);
    }
    //dictionarul va avea ca si chei literele, iar fiecarei chei i se va asocia codul respectiv
    std::unordered_map<char, string> dictionar;

    //ultimul varf ramas in coada este radacina arborelui binar
    codifica_huffman(dictionar, Q.top(), "");
    for (const char& c : mesaj)
        cout << dictionar[c];

    cout << std::endl;
}

// ^ ^ ^ HUFFMAN ^ ^ ^

int main() {
    {
        Graf g{ "a2.txt" }; // <- fisier graf neorientat neponderat
        cout << "Graful initial";
        g.afisare();

        //CODARE PRUFER
        cout << "Codificare Prufer\n";
        for (const auto& i : g.codare_prufer(1))
            cout << i << " ";
        cout << std::endl;

        //DECODARE PRUFER
        cout << "\nDecodificare Prufer";
        decodare_prufer(g.codare_prufer(1), g.n).afisare();

        //CODARE HUFFMAN
        vector<Litera> alfabet;
        string mesaj;
        citeste_mesaj(alfabet, mesaj, "mesaj.txt"); // <- fisier mesaj (string)
        cout << "Mesajul '"<< mesaj << "' codificat Huffman:\n";
        huffman(alfabet, mesaj);
        cout << std::endl;

        GrafPond grp{ "a2pond.txt" }; // <- fisier graf neorientat ponderat
        cout << "Graful ponderat initial";
        grp.afisare();

        //ALGORITM KRUSKAL
        cout << "Algoritmul lui Kruskal";
        grp.kruskal().afisare();

        //ALGORITM PRIM
        cout << "Algoritmul lui Prim";
        grp.prim(1).afisare();

    }

    //verficare memory leaks;
    _CrtDumpMemoryLeaks();
    return 0;
 
}

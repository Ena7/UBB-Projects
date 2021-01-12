#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

void afisare(int n, float dist[], string nume){
    cout<<"\n"<<nume<<"\n";
    cout<<"Nod   ";
    for(int i=1; i<=n; i++)
        cout<<setw(3)<<i<<" ";
    cout<<"\nDist  ";
    for(int i=1; i<=n; i++)
        if(dist[i] == INFINITY)
            cout<<"  - ";
        else
            cout<<setw(3)<<dist[i]<<" ";
}
/*
    n - numarul de varfuri
    dist - distantele de la varful sursa la celelalte varfuri
    vsted - -1 daca varful respectiv a fost deja analizat
    returneaza: varful cu distanta cea mai mica, neanalizat. -1 daca nu se mai poate ajunge la un alt varf
*/
int extract_min(int vsted[], float dist[], int n){
    float mini = INFINITY;
    int ind = -1;
    for(int i=1; i<=n; i++)
        if(dist[i] < mini && vsted[i] != -1){
            ind = i;
            mini = dist[i];
        }
    if (ind != -1){
        int aux = vsted[ind];
        vsted[ind] = -1;
        return aux;
    }
    return -1;
}

void initializare(float dist[], int start, int n){
    dist[start] = 0;
    for(int i=1; i<=n; i++)
        if(i != start)
            dist[i] = INFINITY;
}

void relax(float dist[], int u, int v, float adia[21][21]){
    if(dist[v] > dist[u] + adia[u][v])
        dist[v] = dist[u] + adia[u][v];
}

/*
    n - numarul de varfuri
    start - varful sursa
    adia - matricea de adiacenta a grafului (va fi scrisa ponderea in loc de 1 si infinit in loc de 0)
    afis - true (daca se doreste afisarea), false (in caz contrar)
    dist - distantele de la varful sursa la celelalte varfuri
*/
void dijkstra(int start, int n, float adia[21][21], bool afis, float dist[]){
    int vsted[21];

    initializare(dist, start, n);

    for(int i=1; i<=n; i++)
        vsted[i] = i;

    int indn = 1;
    while(indn <= n){
        int u = extract_min(vsted, dist, n);
        indn++;
        if (u != -1){
            for(int v=1; v<=n; v++)
                if(adia[u][v] != INFINITY)
                    relax(dist, u, v, adia);
        }
    }
    if(afis == true)
        afisare(n, dist, " - DIJKSTRA - ");
}
/*
    n - numarul de varfuri
    start - varful sursa
    adia - matricea de adiacenta a grafului (va fi scrisa ponderea in loc de 1 si infinit in loc de 0)
    afis - true (daca se doreste afisarea), false (in caz contrar)
    johnson - true (daca acest algoritm este folosit de algoritmul Johnson, pentru matricea extinsa), false (contrar)
    dist - distantele de la varful sursa la celelalte varfuri
    returneaza - true (daca nu exista ciclu negativ), false (contrar)
*/
bool ford(int start, int n, float adia[21][21], bool afis, bool johnson, float dist[]){

    initializare(dist, start, n);

    for(int i=1; i<n; i++)
        for(int u=1; u<=n; u++)
            for(int v=1; v<=n; v++)
                if(adia[u][v] || (johnson == true && u == n))
                    relax(dist, u, v, adia);
   int ok = 1;
   for(int u=1; u<=n && ok == 1; u++)
        for(int v=1; v<=n && ok == 1; v++)
            if(adia[u][v] || (johnson == true && u == n))
                if(dist[v] > dist[u] + adia[u][v])
                    ok = 0;
    if(!ok){
        cout<<"\nExista un circuit negativ!";
        return false;
    }
    if(afis == true)
        afisare(n, dist, " - BELLMAN-FORD - ");

    return true;
}

void dijkstra_simple(int start, int n, float adia[21][21], bool afis){
    float dist[21];
    dijkstra(start, n, adia, afis, dist);
}

void ford_simple(int start, int n, float adia[21][21], bool afis, bool johnson){
    float dist[21];
    ford(start, n, adia, afis, johnson, dist);
}

/*
    n - numarul de varfuri
    start - varful sursa
    adia - matricea de adiacenta a grafului (va fi scrisa ponderea in loc de 1 si infinit in loc de 0)
    newadia - noua matrice de adiacenta avand doar ponderi pozitive
    dist - distantele de la varful sursa la celelalte varfuri
*/
void johnson(int start, int n, float adia[21][21]){
    float dist[21], newadia[21][21];

    for(int u=1; u<=n; u++)
        for(int v=1; v<=n; v++)
            newadia[u][v] = adia[u][v];

    for(int i=1; i<=n+1; i++)
        adia[n+1][i] = adia[i][n+1] = 0;

    //verifcam cu Bellman-Ford daca exista cicluri negative
    if(!ford(n+1, n+1, adia, false, true, dist))
        return;

    //calculam noile ponderi pozitive
    for(int u=1; u<=n; u++)
        for(int v=1; v<=n; v++)
            if(newadia[u][v] != INFINITY)
                newadia[u][v] = adia[u][v] + dist[u] - dist[v];

    //aplicam Dijkstra pentru varful sursa
    dijkstra(start, n, newadia, false, dist);

    afisare(n, dist, " - JOHNSON - ");

    cout<<"\n\nNoul graf in algoritmul Johnson\n";
    for(int i=1;i<=n;i++){
        for(int j=1;j<=n;j++)
            if(newadia[i][j] == INFINITY) cout<<setw(2)<<0<<" ";
            else cout<<setw(2)<<newadia[i][j]<<" ";
        cout<<endl;
    }
}
/*
    n - numarul de varfuri
    lst - lista de muchii cu triplete (varful sursa, varful destinatie, ponderea)
    adia - matricea de adiacenta a grafului (va fi scrisa ponderea in loc de 1 si infinit in loc de 0)
*/
int main(){
    int n,lst[65]; float adia[21][21];
    ifstream f;
    //am pus la dispozitie 7 grafuri: m1.txt, m2.txt, ..., m7.txt
    f.open("m3.txt");
    f>>n;
    int lg = 0;
    while(f>>lst[lg+1]>>lst[lg+2]>>lst[lg+3])
        lg += 3;
    for(int i=1; i<=n; i++)
        for(int j=1; j<=n; j++)
            adia[i][j] = INFINITY;

    for(int i=1; i<=lg; i+=3)
        adia[lst[i]][lst[i+1]] = lst[i+2];

    int start;
    cout<<"Varful start: "; cin>>start;

    dijkstra_simple(start, n, adia, true);

    ford_simple(start, n, adia, true, false);

    johnson(start, n, adia);

    f.close();
    return 0;
}

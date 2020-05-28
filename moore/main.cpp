#include <iostream>
#include <cmath>
using namespace std;

//8 1 2 1 4 2 6 4 5 6 5 6 8 6 7 7 3 0 0
//graf de 8 noduri si 8 muchii
//6 1 2 1 4 2 3 3 4 4 2 5 3 5 6 0 0
//graf de 6 noduri si 7 muchii

void Moore(int start, int n, int adia[21][21]){
    int que[21]; //coada
    float dist[21]; //vector cu distantele de la nodul start la fiecare nod

    //distanta nodului start este 0, in rest este infinit
    dist[start] = 0;
    for(int i=1; i<=n; i++)
        if(i != start)
            dist[i] = INFINITY;

    //initializam coada cu -1 in caz ca nu exista drum intre nodul start si alt nod
    for(int i=1; i<=n; i++)
        que[i] = -1;

    int nrq = 0;
    que[nrq++] = start;
    int indq = 0;

    while(indq < n || que[indq+1] == -1){
        int x = que[indq++];
        for(int y=1; y<=n; y++)
            if(adia[x][y] == 1)
                if(dist[y] == INFINITY){
                    que[nrq++] = y;
                    dist[y] = dist[x] + 1;
                }
    }
    cout<<"Nod   ";
    for(int i=1; i<=n; i++)
        cout<<i<< " ";
    cout<<"\nDist  ";
    for(int i=1; i<=n; i++)
        if(dist[i] == INFINITY)
            cout<<"- ";
        else
            cout<<dist[i]<< " ";
}

int main() {
    int n,lst[45],adia[21][21],tranz[21][21];
    cout<<"Noduri: "; cin>>n;
    cout<<"Muchii pana la 0 0: ";
    int lg = 0;
    while(1) {
        cin>>lst[lg+1]>>lst[lg+2];
        if(lst[lg+1] == 0 && lst[lg+2] == 0)
            break;
        lg += 2;
    }
    cout<<endl;
    for(int i=1; i<=n; i++)
        for(int j=1; j<=n; j++)
            tranz[i][j] = adia[i][j] = 0;

    for(int i=1; i<=lg; i+=2)
        tranz[lst[i]][lst[i+1]] = adia[lst[i]][lst[i+1]] = 1;

    //Matricea inchiderii tranzitive (daca exista un drum intre 2 noduri)
    //Se foloseste matricea de adiacenta
    for(int k=1; k<=n; k++)
        for(int i=1; i<=n; i++)
            for(int j=1; j<= n; j++)
                if(tranz[i][j] == 0)
                    tranz[i][j] = tranz[i][k] * tranz[k][j];

    for(int i=1; i<=n; i++) {
        for(int j=1; j<=n; j++)
            cout<<tranz[i][j]<<" ";
        cout<<endl;
    }

    //Determinare cel mai scurt trasreu din punctul a in b (sau la celalalte puncte).
    //Algoritmul lui Moore
    int start;
    cout<<"\nVarful start: "; cin>>start;
    Moore(start, n, adia);

    return 0;
}

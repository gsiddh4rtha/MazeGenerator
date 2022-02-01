#include <iostream>
#include <time.h>
#include <conio.h>
#include <vector>
#include "lib/random.hpp"

using namespace std;
using Random = effolkronium::random_static;

#define SB (int)'|'
#define NSB (int)'_'
#define SPACE (int)' '
#define MARK (int)'*'

#define MAX_LIMIT 500

int N;
int grid[MAX_LIMIT+1][2*MAX_LIMIT+1];

void createGrid(){
    for (int i = 0;i < N+1;i++)
        for (int j = 0;j < 2*N+1;j++)
            grid[i][j] = (i != 0)?((j%2 == 0)?SB:NSB):NSB;
}

void displayGrid(){
    cout << "\n";
    for (int i = 0;i < N+1;i++){
        for (int j = 0;j < 2*N+1;j++){
            cout << (char)grid[i][j];
        }
        cout << endl;
    }
    cout << "\n";
}

//COORDINATE FUNCTION
struct coord{
    int x;
    int y;
};

bool coord_cmp(coord a,coord b){
    if (a.x == b.x && a.y == b.y) return true;
    return false;
}


//CELL FUNCTIONS
struct cell{
    bool vs = false;
    coord u,d,l,r;
};
cell *mtx;

void intialize(cell *p){
    int L,M;
    L = M = 0;

    for (int i = 0;i < N*N;i++){
        L = i%N;
        M = i/N;

        p[i].u.x = M;
        p[i].u.y = 2*L + 1;

        p[i].d.x = M+1;
        p[i].d.y = 2*L + 1;

        p[i].l.x = M+1;
        p[i].l.y = 2*L;

        p[i].r.x = M+1;
        p[i].r.y = 2*L + 2;
    }
}

coord common_side(cell a,cell b){
    //Tells the direction with respect to a.

    if (coord_cmp(a.u,b.d)) return a.u;
    if (coord_cmp(a.d,b.u)) return a.d;
    if (coord_cmp(a.l,b.r)) return a.l;
    if (coord_cmp(a.r,b.l)) return a.r;
}

int chooseRand(int I,int *l){
    
    //Finding neighbours
    vector<int> vnbr;
    int vn = 0;

    vector<int> nbr;
    int n = 0;

    int pU = I - N;
    int pD = I + N;
    int pL = I - 1;
    int pR = I + 1;

    if (pU >= 0 && pU < N*N){
        if (mtx[pU].vs == false){
            vn++;
            vnbr.push_back(pU);
        }

        nbr.push_back(pU);
        n++;
    }

    if (pD >= 0 && pD < N*N){
        if (mtx[pD].vs == false){
            vn++;
            vnbr.push_back(pD);
        }

        nbr.push_back(pD);
        n++;
    }

    if (pL >= 0 && pL < N*N && pL/N == I/N){
        if (mtx[pL].vs == false){
            vn++;
            vnbr.push_back(pL);
        }

        nbr.push_back(pL);
        n++;
    }

    if (pR >= 0 && pR < N*N && pR/N == I/N){
        if (mtx[pR].vs == false){
            vn++;
            vnbr.push_back(pR);
        }

        nbr.push_back(pR);
        n++;
    }
     
    int pos;
    if (vn > 0){
        pos = vnbr.at(Random::get(0,vn-1));
        mtx[pos].vs = true;

        cell curr = mtx[I];
        cell nxt = mtx[pos];

        coord s = common_side(curr,nxt);

        grid[s.x][s.y] = (grid[s.x][s.y] == SB)?NSB:SPACE;
        *l = *l - 1;
    }
    else if (vn == 0){
        pos = nbr.at(Random::get(0,n-1));
    }
    return pos;
}

//ALDOUS_BRODER Algorithm
void Aldous_Broder(cell* m){
    int lim = N*N - 1; //cI already selected
    int cI = 0; //Setting intial position to (0)

    mtx[cI].vs = true;

    while(lim != 0){
        cI = chooseRand(cI,&lim);
    }

    grid[m[0].l.x][m[0].l.y] = MARK;
    grid[m[N*N-1].r.x][m[N*N-1].r.y] = MARK;
}

int main(){
    int n;
    cout << "Enter size of Maze(N): ";
    cin >> n;

    while(n < 2 || n > 500){
        cin >> n;
    }

    N = n;

    mtx = new cell[N*N];
    intialize(mtx);
    
    createGrid();
    Aldous_Broder(mtx);
    displayGrid();

    char ch;
    while(ch = getch()){
        if (ch == 27){
            std::exit(0);
        }
    }
    return 0;
}

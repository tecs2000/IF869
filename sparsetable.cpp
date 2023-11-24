#include <bits/stdc++.h>

using namespace std;

#define A 1664525
#define C 1013904223

string Func;
int Seed, N, Type, Total_Op, Freq_Query, Freq_Update;

int current;

int NEXT_RAND(int current) { return ((A * current) + C) % (4*N); }

int LOG2(int number) { 
    int k = 0;
    while((1 << k) < number) k++;
    return k;
}

int main(){
    cin >> Seed >> N >> Func >> Total_Op >> Freq_Query >> Freq_Update;

    int L = LOG2(N);
    int sparse_table[N][L];
    current = Seed;

    // PRE-PROCESSAMENTO 1
    for(int i = 0; i < N; i++){
        if (i == 0) sparse_table[0][i] = current;
        else sparse_table[0][i] = NEXT_RAND(current);
    }

    // PRE-PROCESSAMENTO 2
    for(int i = 1; i < L; i++){
        for(int j = 0; j + (1 << i) - 1 < N; j++){
            if(Func == "MAX")
                sparse_table[i][j] = max(sparse_table[i - 1][j], 
                                         sparse_table[i - 1][j + (1 << (i - 1))]);
            else if(Func == "MIN")
                sparse_table[i][j] = min(sparse_table[i - 1][j], 
                                         sparse_table[i - 1][j + (1 << (i - 1))]);
            else
                sparse_table[i][j] = sparse_table[i - 1][j] + 
                                     sparse_table[i - 1][j + (1 << (i - 1))];
        }
    }

    // TODO - QUERY AND UPDATE
    return 0;
}
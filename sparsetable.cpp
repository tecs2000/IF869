#include <bits/stdc++.h>

using namespace std;

#define A 1664525
#define C 1013904223

string FUN;
uint32_t Seed;
int COL, L, T_Op, Q, U;

int current;

int NEXT_RAND(int current);
int QRY(int L, int R, int* sparse_table[]);
void UPD(int index, int* sparse_table[]);
int LOG2(int number);

int main()
{
    int case_number = 0;

    while (1)
    {
        cin >> Seed >> COL >> FUN >> T_Op >> Q >> U;

        L = LOG2(COL) + 1; // + 1 to include the line 0

        int* sparse_table[L];
        for (int i = 0; i < COL; i ++)
            sparse_table[i] = (int*)malloc(COL * sizeof(int));

        current = Seed;

        // PRE-PROCESSAMENTO 1
        for (int i = 0; i < COL; i++)
            if (i == 0)
                sparse_table[0][i] = current;
            else
                sparse_table[0][i] = NEXT_RAND(current);
        
        // PRE-PROCESSAMENTO 2
        for (int i = 1; i < L; i++)
            for (int j = 0; j + (1 << i) - 1 < COL; j++)
                if (FUN == "MAX")
                    sparse_table[i][j] = max(sparse_table[i - 1][j],
                                             sparse_table[i - 1][j + (1 << (i - 1))]);
                else if (FUN == "MIN")
                    sparse_table[i][j] = min(sparse_table[i - 1][j],
                                             sparse_table[i - 1][j + (1 << (i - 1))]);
                else
                    sparse_table[i][j] = sparse_table[i - 1][j] +
                                         sparse_table[i - 1][j + (1 << (i - 1))];
            
        // SIMULADOR DE OPERACOES
        cout << "caso " << case_number << endl;

        for (int i = 0; i < T_Op; i++)
        {
            int x = NEXT_RAND(current);
            string OP = (x % (Q + U)) < Q ? "QRY" : "UPD";

            int result;

            if (OP == "QRY")
            {
                int L = NEXT_RAND(current) % COL;
                int R = L + 1 + (NEXT_RAND(current) % (COL - L));

                result = QRY(L, R, sparse_table);
            }
            else
            {
                int index = NEXT_RAND(current) % COL;
                UPD(index, sparse_table);
                result = QRY(index, COL, sparse_table);
            }

            cout << result << endl;
        }

        case_number++;
    }

    return 0;
}

int NEXT_RAND(int current) { return ((A * current) + C) % (4 * COL); }

int LOG2(int number)
{
    int k = 0;
    while ((1 << (k + 1)) <= number) k++;
    return k;
}

int QRY(int L, int R, int* sparse_table[])
{
    int max_fit = LOG2(R - L);
    int result;

    if (FUN == "MAX")
        result = max(sparse_table[max_fit][L], sparse_table[max_fit][R - max_fit - 1]);

    else if (FUN == "MIN")
        result = min(sparse_table[max_fit][L], sparse_table[max_fit][R - max_fit - 1]);

    else
    {
        result = sparse_table[max_fit][L];
        L += (1 << max_fit);
        int left = R - L;

        while (left > 0)
        {   
            max_fit = LOG2(left);
            result += sparse_table[max_fit][L];
            
            L += (1 << max_fit);
            left = R - L;
        }
    }
    return result;
}

void UPD(int index, int* sparse_table[])
{
    int valor = NEXT_RAND(current) % (4 * COL);

    sparse_table[0][index] = valor;

    for (int line = 1; line < L; line++)
        for (int col = max(0, index - (1 << line) + 1); col < index; col++)
        {
            if (FUN == "MAX")
                sparse_table[line][col] = max(sparse_table[line - 1][col],
                                              sparse_table[line - 1][(col + (1 << (line - 1)))]);
            else if (FUN == "MIN")
                sparse_table[line][col] = min(sparse_table[line - 1][col],
                                              sparse_table[line - 1][(col + (1 << (line - 1)))]);
            else
                sparse_table[line][col] = sparse_table[line - 1][col] +
                                          sparse_table[line - 1][col + (1 << (line - 1))];
        }
}

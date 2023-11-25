#include <bits/stdc++.h>

using namespace std;

#define A 1664525
#define C 1013904223

string FUN;
int Seed, N, Type, T_Op, Q, U;

int current;

int NEXT_RAND(int current) { return ((A * current) + C) % (4 * N); }

int LOG2(int number)
{
    int k = 0;
    while ((1 << (k + 1)) < number)
        k++;
    return k;
}

int main()
{
    int case_number = 0;

    while (1)
    {
        cin >> Seed >> N >> FUN >> T_Op >> Q >> U;

        int Lg = LOG2(N) + 1; // + 1 to include the line 0
        int sparse_table[N][Lg];
        current = Seed;

        // PRE-PROCESSAMENTO 1
        for (int i = 0; i < N; i++)
        {
            if (i == 0)
                sparse_table[0][i] = current;
            else
                sparse_table[0][i] = NEXT_RAND(current);
        }

        // PRE-PROCESSAMENTO 2
        for (int i = 1; i < Lg; i++)
        {
            for (int j = 0; j + (1 << i) - 1 < N; j++)
            {
                if (FUN == "MAX")
                    sparse_table[i][j] = max(sparse_table[i - 1][j],
                                             sparse_table[i - 1][j + (1 << (i - 1))]);
                else if (FUN == "MIN")
                    sparse_table[i][j] = min(sparse_table[i - 1][j],
                                             sparse_table[i - 1][j + (1 << (i - 1))]);
                else
                    sparse_table[i][j] = sparse_table[i - 1][j] +
                                         sparse_table[i - 1][j + (1 << (i - 1))];
            }
        }

        // SIMULADOR DE OPERACOES
        cout << "caso " << case_number << endl;

        for (int i = 0; i < T_Op; i++)
        {
            int x = NEXT_RAND(current);
            string OP = (x % (Q + U)) < Q ? "QRY" : "UPD";

            if (OP == "QRY")
            {
                // MODULARIZAR
                int L = NEXT_RAND(current) % N;
                int R = L + 1 + (NEXT_RAND(current) % (N - L));

                int result;
                int max_fit = LOG2(L - R);

                if (FUN == "MAX")
                    result = max(sparse_table[max_fit][L], sparse_table[max_fit][R - max_fit - 1]);

                else if (FUN == "MIN")
                    result = min(sparse_table[max_fit][L], sparse_table[max_fit][R - max_fit - 1]);

                else
                {
                    result = sparse_table[max_fit][L];
                    int left = R - L - (1 << max_fit);
                    while (left > 0)
                    {
                        int prev_max = max_fit;
                        int new_max_fit = LOG2(left);

                        result += sparse_table[new_max_fit][R - left];

                        left -= (1 << new_max_fit);
                    }
                }

                cout << result << endl;
            }
            else
            {
                // ADICIONAR PRINT DO UPDATE E MODULARIZAR O CODIGO
                int index = NEXT_RAND(current) % N;
                int valor = NEXT_RAND(current) % (4 * N);

                sparse_table[0][index] = valor;

                for (int line = 1; line < Lg; line++)
                {
                    for (int col = max(0, index - (1 << line) + 1); col < index; col++)
                    {

                        if (FUN == "MAX")
                            sparse_table[line][col] = max(sparse_table[line - 1][col],
                                                          sparse_table[line - 1][(col + (1 << (i - 1)))]);

                        else if (FUN == "MIN")
                            sparse_table[line][col] = min(sparse_table[line - 1][col],
                                                          sparse_table[line - 1][(col + (1 << (i - 1)))]);

                        else
                        {
                            sparse_table[line][col] = sparse_table[line - 1][col] +
                                                      sparse_table[line - 1][col + (1 << (i - 1))];
                        }
                    }
                }
            }
        }
    }

    return 0;
}
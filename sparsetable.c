#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define ui32 uint32_t
#define A 1664525
#define C 1013904223

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

ui32 current;
int *LOG;

ui32 Seed;
char FUN[4];
int COL, L, T_Op, Q, U, M;

ui32 NEXT_RAND();
void PRECOMPUTE_LOGS();
int QRY(int L, int R, int *sparse_table[]);
void UPD(int index, int *sparse_table[]);

int main()
{
    int case_number = 0;

    while (scanf("%u", &Seed) == 1)
    {
        scanf("%d", &COL);
        scanf("%s", FUN);
        scanf("%d", &T_Op);
        scanf("%d", &Q);
        scanf("%d", &U);

        PRECOMPUTE_LOGS();

        L = LOG[COL - 1] + 1; // + 1 to include the line 0
        M = 4 * COL;

        int *sparse_table[L];
        for (int i = 0; i < L; i++)
            sparse_table[i] = (int *)malloc(COL * sizeof(int));

        current = Seed;

        // PRE-PROCESSAMENTO 1
        for (int i = 0; i < COL; i++)
            if (i == 0)
                sparse_table[0][i] = (Seed % M);
            else
                sparse_table[0][i] = (NEXT_RAND() % M);

        // PRE-PROCESSAMENTO 2
        for (int i = 1; i < L; i++)
            for (int j = 0; j + (1 << i) - 1 < COL; j++)
                if (FUN[1] == 'A')
                    sparse_table[i][j] = max(sparse_table[i - 1][j],
                                             sparse_table[i - 1][j + (1 << (i - 1))]);
                else if (FUN[1] == 'I')
                    sparse_table[i][j] = min(sparse_table[i - 1][j],
                                             sparse_table[i - 1][j + (1 << (i - 1))]);

                else
                    sparse_table[i][j] = sparse_table[i - 1][j] +
                                         sparse_table[i - 1][j + (1 << (i - 1))];

        // SIMULADOR DE OPERACOES
        printf("caso %d\n", case_number);

        for (int i = 0; i < T_Op; i++)
        {
            ui32 x = NEXT_RAND();
            int result;

            if ((x % (Q + U)) < Q)
            {
                int L = (NEXT_RAND() % COL);
                int R = L + 1 + (NEXT_RAND() % (COL - L));

                result = QRY(L, R, sparse_table);
            }
            else
            {
                int index = (NEXT_RAND() % COL);
                UPD(index, sparse_table);
                result = QRY(index, COL, sparse_table);
            }

            printf("%d\n", result);
        }

        printf("\n");

        for (int i = 0; i < L; i++)
            free(sparse_table[i]);

        free(LOG);

        case_number++;
    }

    return 0;
}

ui32 NEXT_RAND()
{
    current = (A * current) + C;
    return current;
}

void PRECOMPUTE_LOGS()
{
    LOG = (int *)malloc(COL * sizeof(int));

    int k;
    for (int i = 0; i < COL; i++)
    {
        int k = 0;
        while ((1 << (k + 1)) <= i)
            k++;
        LOG[i] = k;
    }
}

int QRY(int L, int R, int *sparse_table[])
{
    int max_fit = LOG[R - L - 1];
    int result;

    if (FUN[1] == 'A')
        result = max(sparse_table[max_fit][L], sparse_table[max_fit][R - (1 << max_fit)]);

    else if (FUN[1] == 'I')
        result = min(sparse_table[max_fit][L], sparse_table[max_fit][R - (1 << max_fit)]);

    else
    {
        result = sparse_table[max_fit][L];
        L += (1 << max_fit);
        int left = R - L;

        while (left > 0)
        {
            max_fit = LOG[left - 1];
            result += sparse_table[max_fit][L];

            L += (1 << max_fit);
            left = R - L;
        }
    }
    return result;
}

void UPD(int index, int *sparse_table[])
{
    int valor = (NEXT_RAND() % M);

    sparse_table[0][index] = valor;

    for (int line = 1; line < L; line++)
        for (int col = max(0, index - (1 << line) + 1); col <= index; col++)
        {
            if (FUN[1] == 'A')
                sparse_table[line][col] = max(sparse_table[line - 1][col],
                                              sparse_table[line - 1][(col + (1 << (line - 1)))]);
            else if (FUN[1] == 'I')
                sparse_table[line][col] = min(sparse_table[line - 1][col],
                                              sparse_table[line - 1][(col + (1 << (line - 1)))]);
            else
                sparse_table[line][col] = sparse_table[line - 1][col] +
                                          sparse_table[line - 1][col + (1 << (line - 1))];
        }
}

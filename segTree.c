#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ui unsigned int
#define odd(a) ((a % 2) == 1)

struct RNG
{
    ui A;
    ui C;
    ui last;
    ui seed;
};

void RNG_init(struct RNG *rng, ui seed)
{
    rng->A = 1664525;
    rng->C = 1013904223;
    rng->last = 0;
    rng->seed = seed;
}

ui RNG_next(struct RNG *rng)
{
    if (rng->last == 0)
    {
        rng->last = rng->seed;
        return rng->seed;
    }
    else
    {
        rng->last = (rng->A * rng->last) + rng->C;
        return rng->last;
    }
}

struct RNG *rng;

struct SegTree
{
    int *tree;
    int size;
    char *func;
};

void SegTree_init(struct SegTree *st, int size, char *func)
{
    st->size = size;
    st->tree = (int *)malloc(2 * size * sizeof(int));
    st->func = func;
}

void SegTree_build(struct SegTree *st)
{
    int size = st->size;
    char *func = st->func;

    for (int i = 0; i < size; i++)
        st->tree[i + size] = (int)(RNG_next(rng) % (4 * size));

    for (int i = size - 1; i > 0; i--)
    {
        if (strcmp(func, "MIN"))
            st->tree[i] = __min(st->tree[(2 * i)], st->tree[(2 * i) + 1]);

        else if (strcmp(func, "MAX"))
            st->tree[i] = __max(st->tree[(2 * i)], st->tree[(2 * i) + 1]);

        else
            st->tree[i] = st->tree[(2 * i)] + st->tree[(2 * i) + 1];
    }
}

void SegTree_update(struct SegTree *st, int i)
{
    int size = st->size;
    char *func = st->func;

    int newValue = (int)(RNG_next(rng) % (4 * size));

    i = i + size;

    st->tree[i] = newValue;

    i = i / 2;

    while (i > 0)
    {
        if (strcmp(func, "MIN"))
            st->tree[i] = __min(st->tree[(2 * i)], st->tree[(2 * i) + 1]);

        else if (strcmp(func, "MAX"))
            st->tree[i] = __max(st->tree[(2 * i)], st->tree[(2 * i) + 1]);

        else
            st->tree[i] = st->tree[(2 * i)] + st->tree[(2 * i) + 1];

        i = i / 2;
    }
}

int SegTree_rangeQuery(struct SegTree *st, int l, int r)
{
    int size = st->size;
    char *func = st->func;

    l = size + l;
    r = size + r;

    int eNeutro;
    int ans;

    if (strcmp(func, "MIN"))
        eNeutro = size * 5; // o universo dos valores da arvore é (size * 4), logo size * 5 é muito maior

    else if (strcmp(func, "MAX"))
        eNeutro = -1;

    else
        eNeutro = 0;

    ans = eNeutro;

    while (l < r)
    {
        if (odd(l)) // l é filho à direita
        {
            if (strcmp(func, "MIN"))
                ans = __min(ans, st->tree[l]);

            else if (strcmp(func, "MAX"))
                ans = __max(ans, st->tree[l]);

            else
                ans = ans + st->tree[l];

            l = l + 1;
        }
        if (!odd(r)) // r é filho à esquerda
        {
            r = r - 1; // sobe um nível

            if (strcmp(func, "MIN"))
                ans = __min(ans, st->tree[r]);

            else if (strcmp(func, "MAX"))
                ans = __max(ans, st->tree[r]);

            else
                ans = ans + st->tree[r];
        }

        l = l / 2;
        r = r / 2;
    }

    return ans;
}

int main()
{
    unsigned int s, n, w, q, u, p;
    char f[3];

    int caseCount = 0;

    struct SegTree *st;

    while (scanf("%u %u %s %u %u %u %u %u", &s, &n, &f, &w, &q, &u, &p) == 1)
    {
        rng = (struct RNG *)malloc(sizeof(struct RNG));
        RNG_init(rng, s);

        st = (struct SegTree *)malloc(sizeof(struct SegTree));
        SegTree_init(st, n, f);
        SegTree_build(st);

        printf("casp %d\n", caseCount);
        int result;

        for (int i = 0; i < w; i++)
        {
            int x = (int)(RNG_next(rng));
            char op;

            if (x % (q + u))
                op = 'Q'; // Query
            else
                op = 'U'; // Update

            if (op == 'U')
            {
                int index = (int)(RNG_next(rng) % n);
                SegTree_update(st, index);
                result = result = SegTree_rangeQuery(st, index, n);
            }
            else
            {
                int l = (int)(RNG_next(rng) % n);
                int r = l + 1 + (int)(RNG_next(rng) % (n - l));
                result = SegTree_rangeQuery(st, l, r);
            }

            if ((i % p) == 0)
                printf("%d\n", result);
        }

        printf("\n");

        free(st);
        free(rng);

        caseCount++;
    }
}
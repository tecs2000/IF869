#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ui unsigned int

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
};

void SegTree_init(struct SegTree *st, int size)
{
    st->size = size;
    st->tree = (int *)malloc(2 * size * sizeof(int));
}

void SegTree_build(struct SegTree *st, int size, char *func)
{
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

void SegTree_update(struct SegTree *st, int size, char *func)
{
    int newValue = (int)(RNG_next(rng) % (4 * size));
    int i = (int)(RNG_next(rng) % size);
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

void SegTree_rangeQuery(struct SegTree *st){

}

int main()
{
    unsigned int s, n, w, q, u, p;
    char f[3];

    struct SegTree *st;

    while (scanf("%u %u %s %u %u %u %u %u", &s, &n, &f, &w, &q, &u, &p) == 1)
    {
        rng = (struct RNG *)malloc(sizeof(struct RNG));
        RNG_init(rng, s);

        st = (struct SegTree *)malloc(sizeof(struct SegTree));
        SegTree_init(st, n);
        SegTree_build(st, n, f);

        for (int i = 0; i < w; i++)
        {
            int x = (int)(RNG_next(rng));
            char op;

            if (x % (q + u))
                op = 'Q'; // Query
            else
                op = 'U'; // Update

            if(op == 'U'){
                SegTree_update(st, n, f);
            }
            else{
                SegTree_rangeQuery(st);
            }

            // TODO: integralizar o tamanho e a funcao à st
        }

        free(st);
        free(rng);
    }
}
#include <stdlib.h>
#include <stdio.h>

#define ui unsigned int

#define odd(a) ((a % 2) == 1)

int min(int a, int b) { return ((a) < (b)) ? (a) : (b); }
int max(int a, int b) { return ((a) > (b)) ? (a) : (b); }
int sum(int a, int b) { return (a + b); }

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
    int n;
    int eNeutro;
    int (*func)(int, int); // Function pointer
};

void SegTree_init(struct SegTree *st, int n, char *func)
{
    st->n = n;
    st->tree = (int *)malloc(2 * n * sizeof(int));
    st->eNeutro = 0;

    if (func[1] == 'I')
    {
        st->func = min;
        st->eNeutro = __INT_MAX__;
    }

    else if (func[1] == 'A')
    {
        st->func = max;
        st->eNeutro = 0;
    }

    else
        st->func = sum;
}

void SegTree_build(struct SegTree *st)
{
    for (int i = 0; i < st->n; i++)
        st->tree[i + st->n] = (int)(RNG_next(rng) % (4 * st->n));

    for (int i = st->n - 1; i > 0; i--)
        st->tree[i] = st->func(st->tree[(2 * i)], st->tree[(2 * i) + 1]);
}

void SegTree_update(struct SegTree *st, int i)
{
    i += st->n;

    int newValue = (int)(RNG_next(rng) % (4 * st->n));

    st->tree[i] = newValue;

    while ((i = (i >> 1)) > 0)
        st->tree[i] = st->func(st->tree[(2 * i)], st->tree[(2 * i) + 1]);
}

int SegTree_rangeQuery(struct SegTree *st, int l, int r)
{
    l += st->n;
    r += st->n;

    int ans = st->eNeutro;

    while (l < r)
    {
        if (odd(l))                             // l é filho à direita
            ans = st->func(ans, st->tree[l++]); // avalia e caminha para a direita

        if (odd(r))                             // r é filho à direita
            ans = st->func(ans, st->tree[--r]); // r = r - 1; // sobe um nível

        l >>= 1;
        r >>= 1;
    }

    return ans;
}

int main()
{
    ui s, n, w, q, u, p;
    char f[4];
    int caseNumber = 0;

    struct SegTree *st;

    st = (struct SegTree *)malloc(sizeof(struct SegTree));
    rng = (struct RNG *)malloc(sizeof(struct RNG));

    while (scanf("%u %u %s %u %u %u %u", &s, &n, f, &w, &q, &u, &p) != EOF)
    {
        RNG_init(rng, s);

        SegTree_init(st, n, f);
        SegTree_build(st);

        printf("caso %d\n", caseNumber++);

        for (int i = 0; i < w; i++)
        {
            int x = (int)(RNG_next(rng) % (q + u));
            char op;

            if (x < q)
                op = 'Q'; // Query
            else
                op = 'U'; // Update

            if (op == 'U')
            {
                int index = (int)(RNG_next(rng) % n);
                SegTree_update(st, index);
                if ((i % p) == 0) printf("%d\n", SegTree_rangeQuery(st, index, n));
            }
            else
            {
                int l = (int)(RNG_next(rng) % n);
                int r = l + 1 + (int)(RNG_next(rng) % (n - l));
                if ((i % p) == 0) printf("%d\n", SegTree_rangeQuery(st, l, r));
            }
        }

        printf("\n");
    }

    free(rng);
    free(st);
    
    return 0;
}

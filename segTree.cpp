#include <bits/stdc++.h>
using namespace std;

#define uint uint32_t
#define odd(a) ((a % 2) == 1)

class RNG
{
private:
    uint A;
    uint C;
    uint curr;
    uint seed;

public:
    RNG(uint seed)
    {
        A = 1664525;
        C = 1013904223;
        this->curr = 0;
        this->seed = seed;
    }

    uint next()
    {
        if (this->curr == 0)
        {
            this->curr = this->seed;
            return this->seed;
        }
        else
        {
            this->curr = (this->A * this->curr) + this->C;
            return this->curr;
        }
    }
};

RNG *rng;

class SegTree
{

private:
    uint n;
    uint eNeutro;
    vector<uint> tree;
    function<uint(uint, uint)> func; // Function pouinter

public:
    SegTree(uint n, string func)
    {
        this->n = n;
        tree.resize(2 * n);
        this->eNeutro = 0;

        if (func == "MIN")
        {
            this->func = [](uint a, uint b){ return min(a, b); };
            this->eNeutro = INT_MAX;
        }
        else if (func == "MAX")
        {
            this->func = [](uint a, uint b){ return max(a, b); };
            eNeutro = 0;
        }
        else
            this->func = [](uint a, uint b){ return a + b; };
    }

    void build()
    {
        for (uint i = 0; i < n; i++)
            tree[i + n] = (rng->next() % (4 * n));

        for (uint i = n - 1; i > 0; i--)
            tree[i] = func(tree[(2 * i)], tree[(2 * i) + 1]);
    }

    void update(uint i, uint newValue)
    {
        for (tree[i += n] = newValue; i > 0; i >>= 1) 
            tree[i >> 1] = func(tree[i], tree[i^1]);
    }

    uint rangeQuery(uint l, uint r)
    {
        l += n;
        r += n;

        uint ans = eNeutro;

        while (l < r)
        {
            if (odd(l))                     // l é filho à direita
                ans = func(ans, tree[l++]); // avalia e caminha para a direita

            if (odd(r))                     // r é filho à direita
                ans = func(ans, tree[--r]); // r = r - 1; // sobe um nível

            l >>= 1;
            r >>= 1;
        }

        return ans;
    }
};

int main()
{
    uint s, n, w, q, u, p;
    string f;

    uint caseCount = 0;

    SegTree *st;

    while (cin >> s >> n >> f >> w >> q >> u >> p)
    {
        rng = new RNG(s);

        st = new SegTree(n, f);
        st->build();

        cout << "caso " << caseCount++ << endl;
        uint result;

        for (uint i = 0; i < w; i++)
        {
            uint x = (rng->next() % (q + u));
            char op;

            if (x < q)
                op = 'Q'; // Query
            else
                op = 'U'; // Update

            if (op == 'U')
            {
                uint index = (rng->next() % n);
                uint newValue = (rng->next() % (4 * n));
                st->update(index, newValue);
                result = st->rangeQuery(index, n);
            }
            else
            {
                uint l = (rng->next() % n);
                uint r = l + 1 + (rng->next() % (n - l));
                result = st->rangeQuery(l, r);
            }

            if ((i % p) == 0)
                cout << result << endl;
        }

        cout << endl;
    }

    return 0;
}

#include <bits/stdc++.h>

using namespace std;

#define ui unsigned int

enum Options
{
    FND = 1,
    INS,
    DEL
};

class RNG
{
private:
    ui A;
    ui C;
    ui last;
    ui seed;

public:
    RNG(ui seed)
    {
        A = 1664525;
        C = 1013904223;
        this->last = 0;
        this->seed = seed;
    }

    ui next()
    {
        if (this->last == 0)
        {
            this->last = this->seed;
            return this->seed;
        }
        else
        {
            this->last = (this->A * this->last) + this->C;
            return this->last;
        }
    }
};

class SkipList
{

public:
    void insert(ui key);
    void find(ui key);
    void del(ui key);
};

int main()
{
    ui s, u, b, n, f, i, d, p;
    cin >> s >> u >> b >> n >> f >> i >> d >> p;

    RNG *rng = new RNG(s);
    SkipList *sl = new SkipList();

    // burn-in
    for (ui i = 0; i < b; i++)
        sl->insert(rng->next());

    Options operation;
    bool print = 0;
    for (ui i = 0; i < n; i++)
    {
        ui x = rng->next() % (f + i + d);

        if (x < f) operation = FND;
        else if (x < (f + i)) operation = INS;
        else operation = DEL;

        x = rng->next() % u;
        if((i % p) == 0) print = 1;

        switch (operation)
        {
            case FND:
                if(print) {
                    sl->find(x);
                    printf("TODO");
                }
                
                break;

            case INS:
                sl->insert(x);

                if(print) {
                    printf("TODO");
                }

                break;

            case DEL:
                sl->del(x);

                if(print) {
                    printf("TODO");
                }

                break;
        }
    }
    return 0;
}
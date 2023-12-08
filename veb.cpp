#include <bits/stdc++.h>

using namespace std;

#define ui unsigned int

class RNG
{
private:
    ui A;
    ui C;
    ui Last;
    ui seed;

public:
    RNG(ui seed)
    {
        A = 1664525;
        C = 1013904223;
        this->Last = 0;
        this->seed = seed;
    }

    ui next()
    {
        if (this->Last == 0)
        {
            this->Last = this->seed;
            return this->seed;
        }
        else
        {
            this->Last = (this->A * this->Last) + this->C;
            return this->Last;
        }
    }
};

class VanEmdeBoas
{
private:
    int universeSize;
    int minimum;
    int maximum;
    int universeRoot;
    VanEmdeBoas *summary;
    vector<VanEmdeBoas *> clusters;

public:
    int high(int x) { return x / this->universeRoot; }

    int low(int x) { return x % this->universeRoot; }

    int generate_index(int x, int y) { return (x * this->universeRoot) + y; }
    // Constructor
    VanEmdeBoas(int size)
    {
        this->universeSize = size;
        this->universeRoot = sqrt(universeSize);
        this->minimum = universeSize;
        this->maximum = -1;

        // Base case
        if (size > 2)
        {
            summary = new VanEmdeBoas(this->universeRoot);
            clusters = vector<VanEmdeBoas *>(this->universeRoot, nullptr);

            // cada cluster recebe uma VEB(universe_root)
            for (int i = 0; i < this->universeRoot; i++)
                clusters[i] = new VanEmdeBoas(this->universeRoot);
        }
    }

    int remove(int key)
    {
        if (this->universeSize == 2)
        {
            if (key == this->minimum)
            {

                if (this->minimum != this->maximum)
                    this->minimum = this->maximum;

                else
                    this->minimum = this->universeSize;
            }
            if (key == this->maximum)
            {
                if (this->minimum != this->universeSize)
                    this->maximum = this->minimum;
                else
                    this->maximum = -1;
            }
            return 1;
        }
        if (key == this->minimum)
        {
            int i = this->summary->minimum;

            // nao ha outro elemento na veb
            if (i == this->summary->universeSize)
            {
                this->minimum = this->universeSize;
                this->maximum = -1;

                return 1;
            }
            // encontra o novo minimo e o remove recursivamente
            else
            {
                this->minimum = this->generate_index(i, this->clusters[i]->minimum);
                key = this->minimum;
            }
        }

        int h = this->high(key);
        int l = this->low(key);

        int result = 1 + this->clusters[h]->remove(l); // adicionar retorno (?)

        // removeu o ultimo elemento do cluster
        if (this->clusters[h]->minimum == this->clusters[h]->universeSize)
            this->summary->remove(h);

        // se a chave for o max da estrutura, procura o novo maximo
        if (key == this->maximum)
        {
            int i = this->summary->maximum;
            if (i == -1)
                this->maximum = this->minimum;

            else
                this->maximum = this->generate_index(i, this->clusters[i]->maximum);
        }

        return result;
    }

    int insert(int key)
    {
        if (this->universeSize == 2)
        {
            this->minimum = min(this->minimum, key);
            this->maximum = max(this->maximum, key);

            return 1;
        }

        // se veb vazia, min = max = key
        if (this->minimum == this->universeSize)
        {
            this->minimum = key;
            this->maximum = key;

            return 1;
        }

        if (key < this->minimum)
            swap(this->minimum, key);

        this->maximum = max(this->maximum, key);

        int high = this->high(key);
        int low = this->low(key);

        // se cluster vazio, insere no cluster e no summary
        if (this->clusters[high]->minimum == this->clusters[high]->universeSize)
            this->summary->insert(high);

        return (1 + this->clusters[high]->insert(low));
    }

    bool contains(int key)
    {
        // A key está fora do universo
        if (this->universeSize < key)
            return false;

        if (this->minimum == key || this->maximum == key)
            return true;
        else
        {
            // se o tamanho do universo é 2 e a chave
            // não é nem o max, nem o min, significa que ela
            // não está presente na estrutura
            if (this->universeSize == 2)
                return false;
            else
                return this->clusters[this->high(key)]->contains(this->low(key));
        }
    }

    int successor(int key)
    {
        // caso base
        if (this->universeSize == 2)
        {
            if (key == 0 && this->maximum == 1)
                return 1;
            else
                return -1;
        }
        // se o min presente é maior que a chave,
        // então ele é o seu sucessor
        else if (this->minimum != this->universeSize && key < this->minimum)
            return this->minimum;

        else
        {
            int max_incluster = this->clusters[this->high(key)]->maximum;

            int offset{0}, succ_cluster{0};

            // se o cluster não estiver vazio, encontra o sucessor da chave
            // dentro dele
            if (max_incluster != -1 && this->low(key) < max_incluster)
            {

                offset = this->clusters[this->high(key)]->successor(this->low(key));

                return this->generate_index(this->high(key), offset);
            }

            // se nao, procura no proximo cluster nao vazio
            else
            {
                succ_cluster = this->summary->successor(this->high(key));

                if (succ_cluster == -1)
                    return -1;
                // encontra o minimo dentro desse cluster
                else
                {
                    offset = this->clusters[succ_cluster]->minimum;
                    return this->generate_index(succ_cluster, offset);
                }
            }
        }
    }
};

int main()
{
    std::cin.tie(nullptr);
    ui seed, m, burn_in, n, i, f, d, p;

    std::cin >> seed >> m >> burn_in >> n >> i >> f >> d >> p;

    ui u = 1 << (1 << m);

    RNG *rng = new RNG(seed);
    VanEmdeBoas *veb = new VanEmdeBoas(u);

    // BURN-IN : inserções
    for (int k = 0; k < burn_in; k++)
        veb->insert(rng->next() % u);

    int should_print = 0;

    for (int k = 0; k < n; k++)
    {
        int x = rng->next() % (i + f + d);

        should_print = (k % p) == 0 ? 1 : 0;

        if (x < i) // ins
        {
            x = (rng->next() % u);
            int nivel = 0;

            if (!veb->contains(x))
                nivel = veb->insert(x);

            if (should_print)
                printf("I %d\n", nivel);
        }
        else if (x < (i + f)) // suc
        {
            x = rng->next() % u;

            if (should_print)
            {
                int suc = veb->successor(x);

                if (suc == -1)
                    printf("S %d\n", u);

                else
                    printf("S %d\n", suc);
            }
        }
        else // del
        {
            int y = rng->next() % u;
            int succ = veb->successor(y);

            x = succ == -1 ? y : succ;

            int nivel = 0;
            if (veb->contains(x))
                nivel = veb->remove(x);

            if (should_print)
                printf("D %d\n", nivel);
        }
    }
    return 0;
}

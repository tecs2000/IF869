#include <bits/stdc++.h>

using namespace std;

#define ui32 uint32_t
#define A 1664525
#define C 1013904223

ui32 Seed, Current;

ui32 NEXT_RAND();

class Van_Emde_Boas
{

public:
    int universe_size;
    int minimum;
    int maximum;
    Van_Emde_Boas *summary;
    vector<Van_Emde_Boas *> clusters;

    int high(int x)
    {
        int div = ceil(sqrt(universe_size));
        return x / div;
    }

    int low(int x)
    {
        int mod = ceil(sqrt(universe_size));
        return x % mod;
    }

    int generate_index(int x, int y)
    {
        int ru = ceil(sqrt(universe_size));
        return x * ru + y;
    }

    // Constructor
    Van_Emde_Boas(int size)
    {
        universe_size = size;
        minimum = universe_size;
        maximum = -1;

        // Base case
        if (size <= 2)
        {
            summary = nullptr;
            clusters = vector<Van_Emde_Boas *>(0, nullptr);
        }
        else
        {
            int universe_root = ceil(sqrt(size));

            summary = new Van_Emde_Boas(universe_root);

            clusters = vector<Van_Emde_Boas *>(universe_root, nullptr);

            // cada cluster recebe uma VEB(universe_root)
            for (int i = 0; i < universe_root; i++)
                clusters[i] = new Van_Emde_Boas(universe_root);
        }
    }
};

int minimum(Van_Emde_Boas *veb)
{
    return (veb->minimum == veb->universe_size ? veb->universe_size : veb->minimum);
}

int maximum(Van_Emde_Boas *veb)
{
    return (veb->maximum == -1 ? -1 : veb->maximum);
}

int insert(Van_Emde_Boas *veb, int key, int nivel = 1)
{

    if (veb->universe_size == 2)
    {
        veb->minimum = min(veb->minimum, key);
        veb->maximum = max(veb->maximum, key);

        return nivel;
    }

    // se veb vazia, min = max = key
    if (veb->minimum == veb->universe_size)
    {
        veb->minimum = key;
        veb->maximum = key;

        return nivel;
    }
    
    if (key < veb->minimum)
    {

        // troca minimo global com a chave atual e
        // insere o antigo minimo global recursivamente
        swap(veb->minimum, key);
    }

    veb->maximum = max(veb->maximum, key);

    int high = veb->high(key);
    int low = veb->low(key);
    // se cluster vazio, insere no cluster e no summary
    if (minimum(veb->clusters[high]) == veb->universe_size)
        insert(veb->summary, high, ++nivel);

    insert(veb->clusters[high], low, ++nivel);

    return nivel;
}

bool contains(Van_Emde_Boas *veb, int key)
{

    // A key está fora do universo
    if (veb->universe_size < key)
        return false;

    if (veb->minimum == key || veb->maximum == key)
        return true;
    else
    {

        // se o tamanho do universo é 2 e a chave
        // não é nem o max, nem o min, significa que ela
        // não está presente na estrutura
        if (veb->universe_size == 2)
            return false;
        else
        {
            return contains(
                veb->clusters[veb->high(key)],
                veb->low(key));
        }
    }
}

int successor(Van_Emde_Boas *veb, int key)
{

    // caso base
    if (veb->universe_size == 2)
    {
        if (key == 0 && veb->maximum == 1)
            return 1;
        else
            return -1;
    }

    // se o min presente é maior que a chave,
    // então ele é o seu sucessor
    else if (veb->minimum != veb->universe_size && key < veb->minimum)
        return veb->minimum;
    else
    {
        int max_incluster = maximum(veb->clusters[veb->high(key)]);

        int offset{0}, succ_cluster{0};

        // se o cluster não estiver vazio, encontra o sucessor da chave
        // dentro dele
        if (max_incluster != -1 && veb->low(key) < max_incluster)
        {

            offset = successor(
                veb->clusters[veb->high(key)],
                veb->low(key));

            return veb->generate_index(veb->high(key), offset);
        }

        // se nao, procura no proximo cluster nao vazio
        else
        {

            succ_cluster = successor(veb->summary, veb->high(key));

            if (succ_cluster == -1)
                return -1;
            // encontra o minimo dentro desse cluster
            else
            {
                offset = minimum(veb->clusters[succ_cluster]);
                return veb->generate_index(succ_cluster, offset);
            }
        }
    }
}

// Function to find the predecessor of the given key
int predecessor(Van_Emde_Boas *veb, int key)
{

    // Base case: If the key is 1 and it's predecessor
    // is present then return 0 else return null
    if (veb->universe_size == 2)
    {
        if (key == 1 && veb->minimum == 0)
            return 0;
        else
            return -1;
    }

    // If the key is greater than maximum of the tree then
    // return key as it will be the predecessor of the key
    else if (veb->maximum != -1 && key > veb->maximum)
        return veb->maximum;
    
    else
    {
        // Find predecessor in the cluster of the key
        // First find minimum in the key to check whether
        // any key is present in the cluster
        int min_incluster = minimum(veb->clusters[veb->high(key)]);

        int offset{0}, pred_cluster{0};

        // If any key is present in the cluster then find
        // predecessor in the cluster
        if (min_incluster != veb->universe_size && veb->low(key) > min_incluster)
        {

            offset = predecessor(
                veb->clusters[veb->high(key)],
                veb->low(key));

            return veb->generate_index(veb->high(key), offset);
        }

        // Otherwise look for predecessor in the summary
        // which returns the index of predecessor cluster
        // with any key present
        else
        {

            pred_cluster = predecessor(
                veb->summary, veb->high(key));

            // If no predecessor cluster then...
            if (pred_cluster == -1)
            {
                if (veb->minimum != veb->universe_size && key > veb->minimum)
                    return veb->minimum;
                
                else
                    return -1;
            }

            // Otherwise find maximum in the predecessor
            // cluster
            else
            {
                offset = maximum(veb->clusters[pred_cluster]);

                return veb->generate_index(pred_cluster,
                                           offset);
            }
        }
    }
}

int remove(Van_Emde_Boas *veb, int key, int nivel = 1)
{

    // se só há um elemento, significa que é o key
    if (veb->maximum == veb->minimum)
    {
        veb->minimum = veb->universe_size;
        veb->maximum = -1;
    }
    else if (veb->universe_size == 2)
    {
        if (key == 0)
            veb->minimum = 1;
        else
            veb->minimum = 0;

        veb->maximum = veb->minimum;
    }
    else
    {
        // se queremos deletar o minimo, procuramos
        // o seu sucessor dentro da arvore
        if (key == veb->minimum)
        {

            int first_cluster = minimum(veb->summary);

            key = veb->generate_index(first_cluster, minimum(veb->clusters[first_cluster]));

            veb->minimum = key;
        }

        remove(veb->clusters[veb->high(key)], veb->low(key), nivel++);

        // se apos a remoção, o min se tornou universe_size, precisamos deletar
        // o cluster do summary também
        if (minimum(veb->clusters[veb->high(key)]) == veb->universe_size)
        {

            remove(veb->summary, veb->high(key));

            // se a chave é o max da veb tree
            if (key == veb->maximum)
            {
                int max_insummary = maximum(veb->summary);

                // max = -1 indica que só há um elemento agora na arvore: o minimo
                if (max_insummary == -1)
                    veb->maximum = veb->minimum;
                // se nao, procura o novo maximo e atribui
                else
                {
                    veb->maximum = veb->generate_index(
                        max_insummary,
                        maximum(veb->clusters[max_insummary]));
                }
            }
        }
        else if (key == veb->maximum)
        {
            veb->maximum = veb->generate_index(
                veb->high(key),
                maximum(veb->clusters[veb->high(key)]));
        }
    }

    return nivel;
}

int main()
{
    int m, burn_in, n, i, f, d, p;

    std::cin >> Seed >> m >> burn_in >> n >> i >> f >> d >> p;

    ui32 u = 1 << (1 << m);
    Van_Emde_Boas *veb = new Van_Emde_Boas(u);

    Current = Seed;

    // BURN-IN : inserções
    for (int k = 0; k < burn_in; k++)
    {
        if (k == 0)
            insert(veb, (Seed % u));
        else
            insert(veb, (NEXT_RAND() % u));
    }

    for (int k = 0; k < n; k++)
    {
        int x = NEXT_RAND() % (i + f + d);

        if (x < i)
        {
            // ins
            ui32 item = (NEXT_RAND() % u);
            if (!contains(veb, item))
            {
                int nivel = insert(veb, item);
                printf("I %d\n", nivel);
            }
            else
                printf("I 0\n");
        }
        else if (x < (i + f))
        {
            // suc
            int suc = successor(veb, NEXT_RAND() % u);
            if (suc == -1)
                printf("S %d\n", u);
            else
                printf("S %d\n", suc);
        }
        else
        {
            // del
            int y = NEXT_RAND() % u;
            x = successor(veb, y);

            if (x == -1)
                x = y;

            if (!contains(veb, x))
                printf("D 0\n");
            else
            {
                int nivel = remove(veb, x);
                printf("D %d\n", nivel);
            }
        }
    }
    return 0;
}

ui32 NEXT_RAND()
{
    Current = (A * Current) + C;
    return Current;
}
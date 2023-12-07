#include <bits/stdc++.h>

using namespace std;

#define ui unsigned int
#define A 1664525
#define C 1013904223

ui Seed, Current;

ui NEXT_RAND();

class Van_Emde_Boas
{

public:
    int universe_size;
    int minimum;
    int maximum;
    int sqrt;
    Van_Emde_Boas *summary;
    vector<Van_Emde_Boas *> clusters;

    int high(int x)
    {
        return x / this->sqrt;
    }

    int low(int x)
    {
        return x % this->sqrt;
    }

    int generate_index(int x, int y)
    {
        return (x * this->sqrt) + y;
    }

    // Constructor
    Van_Emde_Boas(int size)
    {
        universe_size = size;
        minimum = universe_size;
        maximum = -1;
        sqrt = ceil(sqrt(universe_size));

        // Base case
        if (size > 2)
        {
            summary = new Van_Emde_Boas(sqrt);

            clusters = vector<Van_Emde_Boas *>(sqrt, nullptr);

            // cada cluster recebe uma VEB(universe_root)
            for (int i = 0; i < sqrt; i++)
                clusters[i] = new Van_Emde_Boas(sqrt);
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
        swap(veb->minimum, key);

    veb->maximum = max(veb->maximum, key);

    int high = veb->high(key);
    int low = veb->low(key);

    // se cluster vazio, insere no cluster e no summary
    if (veb->clusters[high]->minimum == veb->universe_size)
        insert(veb->summary, high);

    return insert(veb->clusters[high], low, ++nivel);
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

int predecessor(Van_Emde_Boas *veb, int key)
{
    // caso base: se chave 1, e chave 0 esta presente
    // retorna 0, se nao, retorna -1
    if (veb->universe_size == 2)
    {
        if (key == 1 && veb->minimum == 0)
            return 0;
        else
            return -1;
    }

    // se a chave é maior que o maximo presente, entao o
    // maximo é o seu predecessor
    else if (veb->maximum != -1 && key > veb->maximum)
        return veb->maximum;
    
    else
    {
        // checa no cluster da chave se ha algum elemento
        int min_incluster = minimum(veb->clusters[veb->high(key)]);

        int offset{0}, pred_cluster{0};

        // se ha alguma chave no cluster e key é maior que o minimo presente
        // significa que o seu predecessor está lá
        if (min_incluster != veb->universe_size && veb->low(key) > min_incluster)
        {

            offset = predecessor(
                veb->clusters[veb->high(key)],
                veb->low(key));

            return veb->generate_index(veb->high(key), offset);
        }

        // se nao, procura pelo predecessor do seu cluster no summary
        // que vai retornar o cluster predecessor que possua uma chave presente 
        else
        {

            pred_cluster = predecessor(veb->summary, veb->high(key));

            // se nao ha um cluster predecessor...
            if (pred_cluster == -1)
            {
                if (veb->minimum != veb->universe_size && key > veb->minimum)
                    return veb->minimum;
                
                else
                    return -1;
            }

            // se ha, acha o predecessor dentro do cluster predecessor
            else
            {
                offset = maximum(veb->clusters[pred_cluster]);
                return veb->generate_index(pred_cluster, offset);
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

    ui u = 1 << (1 << m);
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

    int should_print = 0;

    for (int k = 0; k < n; k++)
    {
        int x = NEXT_RAND() % (i + f + d);
        should_print = (k % p) == 0 ? 1 : 0;

        // ins
        if (x < i)
        {
            ui item = (NEXT_RAND() % u);
            int nivel = 0;

            if (!contains(veb, item))
                nivel = insert(veb, item);

            if(should_print)
                printf("I %d\n", nivel);
        }
        // suc
        else if (x < (i + f))
        {
            if(should_print){
                int suc = successor(veb, NEXT_RAND() % u);

                if (suc == -1)
                    printf("S %d\n", u);

                else
                    printf("S %d\n", suc);
            }
        }
        // del
        else
        {
            int y = NEXT_RAND() % u;
            x = successor(veb, y);

            if (x == -1)
                x = y;
            
            int nivel = 0;
            if (contains(veb, x))
                nivel = remove(veb, x);
            
            if(should_print)
                printf("D %d\n", nivel);
        }
    }
    return 0;
}

ui NEXT_RAND()
{
    Current = (A * Current) + C;
    return Current;
}

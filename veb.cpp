#include <iostream>
#include <cmath>
#include <vector>

#define ui32 uint32_t
#define A 1664525
#define C 1013904223

ui32 Seed, Current;

ui32 NEXT_RAND();

class VEBTree {
private:
    int universeSize;
    int min, max;
    VEBTree* summary;
    std::vector<VEBTree*> clusters;

    int high(int x) {
        return static_cast<int>(std::floor(x / std::sqrt(universeSize)));
    }

    int low(int x) {
        return x % static_cast<int>(std::floor(std::sqrt(universeSize)));
    }

    int index(int x, int y) {
        return x * static_cast<int>(std::floor(std::sqrt(universeSize))) + y;
    }

public:
    VEBTree(int u) : universeSize(u) {
        if (universeSize <= 2) {
            min = -1;
            max = -1;
            summary = nullptr;
            return;
        }

        int highBits = high(universeSize);
        int clusterSize = static_cast<int>(std::ceil(std::sqrt(highBits)));
        
        summary = new VEBTree(clusterSize);
        clusters.resize(clusterSize);

        for (int i = 0; i < clusterSize; ++i) {
            clusters[i] = new VEBTree(static_cast<int>(std::ceil(std::sqrt(universeSize))));
        }

        min = -1;
        max = -1;
    }

    void insert(int x) {
        if (min == -1) {
            min = max = x;
        } else {
            if (x < min) {
                std::swap(x, min);
            }
            if (universeSize > 2) {
                int highX = high(x);
                int lowX = low(x);
                if (clusters[highX]->min == -1) {
                    summary->insert(highX);
                    clusters[highX]->min = clusters[highX]->max = lowX;
                } else {
                    clusters[highX]->insert(lowX);
                }
            }
            if (x > max) {
                max = x;
            }
        }
    }

    bool contains(int x) {
        if (x == min || x == max) {
            return true;
        } else if (universeSize <= 2) {
            return false;
        } else {
            return clusters[high(x)]->contains(low(x));
        }
    }

    void remove(int x) {
        if (min == max) {
            min = max = -1;
        } else if (universeSize <= 2) {
            if (x == 0) {
                min = 1;
            } else {
                min = 0;
            }
            max = min;
        } else {
            if (x == min) {
                int firstCluster = summary->min;
                x = index(firstCluster, clusters[firstCluster]->min);
                min = x;
            }
            clusters[high(x)]->remove(low(x));

            if (clusters[high(x)]->min == -1) {
                summary->remove(high(x));
                if (x == max) {
                    int summaryMax = summary->max;
                    if (summaryMax == -1) {
                        max = min;
                    } else {
                        max = index(summaryMax, clusters[summaryMax]->max);
                    }
                }
            } else if (x == max) {
                max = index(high(x), clusters[high(x)]->max);
            }
        }
    }

    int successor(int x) {
        if (universeSize <= 2) {
            if (x == 0 && max == 1) {
                return 1;
            }
            return -1;
        } else if (min != -1 && x < min) {
            return min;
        } else {
            int highX = high(x);
            int lowX = low(x);
            int maxLow = clusters[highX]->max;
            if (maxLow != -1 && lowX < maxLow) {
                int offset = clusters[highX]->successor(lowX);
                return index(highX, offset);
            } else {
                int succCluster = summary->successor(highX);
                if (succCluster == -1) {
                    return -1;
                } else {
                    int offset = clusters[succCluster]->min;
                    return index(succCluster, offset);
                }
            }
        }
    }
};


int main() {
    int u, burn_in, n, i, f, d, p;

    std::cin >> u >> burn_in >> n >> i >> f >> d >> p;

    ui32 universeSize = 1 << (1 << u);
    VEBTree veb(universeSize);

    Current = Seed;

    // BURN-IN : inserções
    for(int k = 0; k < burn_in; k++){
        if(k == 0)
            veb.insert((Seed % u));
        else
            veb.insert((NEXT_RAND() % u));
    }

    for(int k = 0; k < n; k++){
        int x = NEXT_RAND() % (i + f + d);

        if(x < i){
            // ins
            veb.insert(NEXT_RAND() % u);
        }
        else if (x < (i+f)){
            // suc
            veb.successor(NEXT_RAND() % u);
        }
        else {
            // del
            int y = NEXT_RAND() % u;
            x = veb.successor(y);

            if(x == -1) x = y;

            veb.remove(x);
        }
    }
    return 0;
}

ui32 NEXT_RAND()
{
    Current = (A * Current) + C;
    return Current;
}
// #include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#define endl '\n'
#define ui unsigned int

using namespace std;

class RGN {
    private:
        ui lastValue;
        ui a = 1664525, c = 1013904223;
        int nextCount = 0;
    
    public:
        RGN(ui seed) {
            this->lastValue = seed;
        }
        
        ui next() {
            if (nextCount == 0) {
                this->nextCount++;
                return this->lastValue;
            } else {
                this->nextCount++;
                this->lastValue = (a * this->lastValue  + c);
                return this->lastValue;
            }
        }
};

class vEBT {
    private:
        int size;
        int min;
        int max;
        int sizeSqrt;
        vEBT **clusters;
        vEBT *summary;

    int high(int value) {
        return value / this->sizeSqrt;
    }

    int low(int value) {
        return value % this->sizeSqrt;
    }

    int index(int high, int low) {
        return high * this->sizeSqrt + low;
    }

    
    public:
        vEBT(int size) {
            this->size = size;
            this->max = -1;
            this->min = size;
            this->sizeSqrt = sqrt(this->size);

            if (size > 2) {
                this->summary = new vEBT(this->sizeSqrt);

                this->clusters = new vEBT*[this->sizeSqrt];

                for (int i = 0; i < this->sizeSqrt; i++) {
                    this->clusters[i] = new vEBT(this->sizeSqrt);
                }
            }
        }
        
        int insert(int value) {
            if (value == this->min || value == this->max) {
                // if the value is already stored, just don't do nothing
                return 0;
            }
            if (this->size == 2) {
                // the base case of the recursion
                if (value < this->min) {
                    this->min = value;
                }
                if (value > this->max) {
                    this->max = value;
                }
                return 1;
            }
            if (this->min == this->size) {
                // if the structure is empty
                this->min = value;
                this->max = value;
                return 1;
            }
            if (value < this->min) {
                // if the value is the new min, swap
                int aux = this->min;
                this->min = value;
                value = aux;
            }
            if (value > this->max) {
                this->max = value;
            }
            int high = this->high(value);
            int low = this->low(value);
            if (this->clusters[high]->min == this->clusters[high]->size) {
                this->summary->insert(high);
            }
            int recursionReturn = this->clusters[high]->insert(low);
            if (recursionReturn == 0) {
                return 0;
            }
            return 1 + recursionReturn;
        }

        int successor(int value) {
            if (value < this->min) {
                return this->min;
            }
            if (value > this->max) {
                return this->size;
            }
            if (this->size == 2) {
                if (value == 0 && this->max == 1) {
                    return 1;
                }
                return 2;
            }
            int high = this->high(value);
            int low = this->low(value);
            if (low < this->clusters[high]->max) {
                int s = this->clusters[high]->successor(low);
                return this->index(high, s);
            }
            high = this->summary->successor(high);
            if (high == this->summary->size) {
                return this->size;
            }
            int s = this->clusters[high]->min;
            return this->index(high, s);
        }

        int remove(int value) {
            if (this->size == 2) {
                if (value == this->min || value == this->max) {
                    if (value == this->min) {
                        if (this->max != this->min) {
                            this->min = this->max;
                        } else {
                            this->min = 2;
                        }
                    }
                    if (value == this->max) {
                        if (this->min != this->size) {
                            this->max = this->min;
                        } else {
                            this->max = -1;
                        }
                    }
                    return 1;
                }
                return 0;
            } // until here it will always return
            if (value == this->min) {
                int i = this->summary->min;
                if (i == this->summary->size) {
                    this->min = this->size;
                    this->max = -1;
                    return 1;
                } // until here it will always return
                this->min = this->index(i, this->clusters[i]->min);
                value = this->min;
            }
            int high = this->high(value);
            int low = this->low(value);
            int recursionReturn = this->clusters[high]->remove(low);
            if (this->clusters[high]->min == this->clusters[high]->size) {
                this->summary->remove(high);
            }
            if (value == this->max) {
                int i = this->summary->max;
                if (i == -1) {
                    this->max = this->min;
                } else {
                    this->max = this->index(i, this->clusters[i]->max);
                }
            }
            if (recursionReturn == 0) {
                return 0;
            }
            return 1 + recursionReturn;
        }
};

vEBT* t;

int main(int argc, char *argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ui s, m, b, n, i, f, d, p;
    cin >> s >> m >> b >> n >> i >> f >> d >> p;

    RGN rng(s);

    ui U = (1U << (1U << m));
    t = new vEBT(U);

    for (int j = 0; j < b; j++) {
        t->insert(rng.next() % U);
    }

    int outputCount = 0;

    for (int j = 0; j < n; j++) {
        int x = rng.next() % (i+f+d);
        // int toMod = i+f+d;
        // x = x % toMod;
        if (x < i) {
            // INS
            x = rng.next() % U;
            int result = t->insert(x);
            if(j % p == 0) {
                outputCount++;
                cout << "I " << result << endl;
            }
        } else if (x < i+f) {
            // SUC
            x = rng.next() % U;
            if(j % p == 0) {
                int result = t->successor(x);
                outputCount++;
                cout << "S " << result << endl;
            }
        } else {
            // DEL
            int y = rng.next() % U;
            x = t->successor(y); // if y has no successor x = y
            if (U == x) {
                x = y;
            }
            int result = t->remove(x);
            if(j % p == 0) {
                outputCount++;
                cout << "D " << result << endl;
            }
        }
    }

    return 0;
}

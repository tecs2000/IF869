#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ui unsigned int

// RNG functions definition
void init(ui);
ui   next();

// Splay tree functions definition
int insert(int);
int query(int);


// STRUCTURES
typedef struct Node{

    int key;
    struct Node * left;
    struct Node * right;
} Node;


typedef struct SplayTree{
    struct Node *root;

    int (* insert)(int);
    int (* query)(int);
} SplayTree;

int insert(int key) {return 0;}

int query(int key) {return 0;}

typedef struct RNG
{
    ui A;
    ui C;
    ui last;
    ui seed;

    void (* init)(ui); // pointer to function
    ui (* next)(); // pointer to function
} RNG;

RNG *rng;

// FUNCTIONS
void init(ui seed)
{
    rng->A = 1664525;
    rng->C = 1013904223;
    rng->last = 0;
    rng->seed = seed;
};


ui next()
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
};


int main()
{
    ui s, u, b, n, i, q, p;
    scanf("%u %u %u %u %u %u %u", &s, &u, &b, &n, &i, &q, &p);

    rng = (RNG *) malloc(sizeof(RNG));
    rng->init = init;
    rng->next = next;

    rng->init(s);

    SplayTree * st = (SplayTree *) malloc(sizeof(SplayTree));
    st->insert = insert;
    st->query = query;

    // burn-in
    for(int i = 0; i < b; i++)
        st->insert(rng->next() % u);
    
    char print;
    char op[4];
    int result;
    
    for(int k = 0; k < n; k++){
        if((k % p) == 0) print = 1;

        ui x = rng->next();
        strcpy(op, (x % (i + q)) < i ? "ins" : "qry");

        x = rng->next() % u;
        if(op == "ins"){
            result = st->insert(x);
            if(print) printf("I %u %d", x, result);
        }
        else {
            result = st->query(x);
            if(print) printf("Q %u %d", x, result);
        }

        print = 0;
    }
    return 0;
}
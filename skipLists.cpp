#include <bits/stdc++.h>

using namespace std;

#define ui unsigned int

// GLOBALS
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

RNG *rng;

class Node {
public:
    int data;
    vector<Node*> next;     // To maintain the levels of the skip list
    
    Node(int data, int level) : data(data), next(level, nullptr) {} // declaring the data and the level of the node 
};

class SkipList {
private:
    Node* head;
    int level;

public:
    SkipList(){
        this->level = 1;
        this->head  = new Node(0, this->level);
    }

    vector<Node*> precursors(ui data){
        Node* current = this->head;
        vector<Node*> update(this->level, nullptr);

        for (int i = this->level - 1; i >= 0; i--) 
        {
            while (current->next[i] and current->next[i]->data < data) 
                current = current->next[i];
            
            // save the precursor of the new node at each level
            update[i] = current;
        }

        return update;
    }

    int insert(ui data){
        vector<Node*> update = this->precursors(data);

        if (update[0]->next[0] == nullptr || update[0]->next[0]->data != data){
            int nodeLevel = 1;
            int levelMax = this->level + 1;

            while (((rng->next() % 100) < 50) && (nodeLevel < levelMax))
                nodeLevel++;
            
            // resize the level of the sl
            if (this->level < nodeLevel) {
                head->next.resize(nodeLevel + 1, nullptr);
                this->level = nodeLevel;
            }

            Node* newNode = new Node(data, this->level);

            for (int i = 0; i < nodeLevel; i++) 
            {
                newNode->next[i] = update[i]->next[i];
                update[i]->next[i] = newNode;
            }

            return 1;
        }

        else
            return 0;
    };

    pair<int, int> find(ui data){
        Node* current = head;
        int visited = 1;
        int level = -1;

        for (int i = this->level - 1; i >= 0; i--) {
            /* keep on moving forward if the value of the next node 
             is less than the searching node otherwise move downward (handled by outer for loop) */
            while (current->next[i] && current->next[i]->data < data){
                current = current->next[i]; // moving forward
                visited++;
            }
            /* if find the node, save its level and stop the search */
            if(current->next[i] && current->next[i]->data == data){
                level = i;
                break;    
            }
        }

        if (level != -1)
            return {visited, level};
        
        else 
            return {visited, 0};
    };
    
    void del(ui key);
};

int main()
{
    ui s, u, b, n, f, i, d, p;
    cin >> s >> u >> b >> n >> f >> i >> d >> p;

    rng = new RNG(s);
    SkipList *sl = new SkipList();

    // burn-in
    for (ui i = 0; i < b; i++)
        sl->insert(rng->next());

    Options operation;
    bool print = 0;
    int printcount = 0;
    int result = 0;

    for (ui k = 0; k < n; k++)
    {   
        ui x = rng->next() % (f + i + d);

        if (x < f) operation = FND;
        else if (x < (f + i)) operation = INS;
        else operation = DEL;

        x = rng->next() % u;

        if((k % p) == 0) {
            print = 1;
            printcount++;
        }

        switch (operation)
        {
            case FND:
                if(print) {
                    pair<int, int> results = sl->find(x);
                    printf("F %d %d\n", results.first, results.second);
                }
                break;

            case INS:
                result = sl->insert(x);

                if(print) 
                    printf("I %d\n", result);

                break;

            case DEL:
                //sl->del(x);

                if(print) {
                    printf("TODO");
                }

                break;
        }

        print = 0;
    }
    return 0;
}
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
        this->head  = new Node(-1, this->level);
    }

    pair<vector<Node*>, int> precursors(ui data){
        Node* current = this->head;
        vector<Node*> update(this->level, nullptr);

        int count = 1;
        int visited = 1;

        for (int i = this->level - 1; i >= 0; i--) 
        {
            while (current->next[i] && current->next[i]->data < data) {
                current = current->next[i];
                visited++;                
            }

            // save the precursor of the new node at each level
            update[i] = current;
        }

        return {update, visited};
    }

    int insert(int data){
        vector<Node*> update = this->precursors(data).first;

        if (update[0]->next[0] == nullptr || update[0]->next[0]->data != data){
            int nodeLevel = 1;
            int levelMax = this->level + 1;

            while (((rng->next() % 100) < 50) && (nodeLevel < levelMax))
                nodeLevel++;
            
            // resize the level of the sl
            if (this->level < nodeLevel) {
                this->head->next.resize(nodeLevel, nullptr);
                update.resize(nodeLevel, this->head);
                this->level = nodeLevel;
            }

            Node* newNode = new Node(data, nodeLevel);

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
        pair<vector<Node*>, int> search_result = this->precursors(data);

        vector<Node*> update = search_result.first;
        int visited = search_result.second;

        if(update[0]->next[0] && update[0]->next[0]->data == data)
            return {visited, update[0]->next[0]->next.size()};
        
        else 
            return {visited, 0};
    };
    
    int del(ui data){
        vector<Node*> precursors = this->precursors(data).first;

        if(precursors[0]->next[0] == nullptr || precursors[0]->next[0]->data != data)
            return 0;
        
        Node* temp = precursors[0]->next[0];

        for(int i = 0; i < temp->next.size(); i++)
            precursors[i]->next[i] = temp->next[i];
    
        while (this->level > 1 && this->head->next[this->level - 1] == nullptr)
            this->level = this->level - 1;
        
        return 1;
    };
};

int main()
{
    ui s, u, b, n, f, i, d, p;
    cin >> s >> u >> b >> n >> f >> i >> d >> p;

    rng = new RNG(s);
    SkipList *sl = new SkipList();

    // burn-in
    for (ui i = 0; i < b; i++)
        sl->insert(rng->next() % u);

    Options operation;
    bool print = 0;
    int result = 0;

    for (ui k = 0; k < n; k++)
    {   
        ui x = rng->next() % (f + i + d);

        if (x < f) operation = FND;
        else if (x < (f + i)) operation = INS;
        else operation = DEL;

        x = rng->next() % u;

        if((k % p) == 0) 
            print = 1;
        
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
                result = sl->del(x);

                if(print) 
                    printf("D %d\n", result);
                
                break;
        }

        print = 0;
    }
    return 0;
}
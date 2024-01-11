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

RGN* rng;

class FindResult {
    public:
        int quantityOfNodesVisited;
        int heightOfNodeX;

        FindResult (int quantityOfNodesVisited, int heightOfNodeX) {
            this->quantityOfNodesVisited = quantityOfNodesVisited;
            this->heightOfNodeX = heightOfNodeX;
        }
};

class SkipList {
    private:
        class Node {
            public:
                ui key;
                ui val;
                ui height;
                vector<Node*> next;
                // next.height >= this.height
                // next.key = sucessor(this.key) || next.key == null

                Node (ui key, ui val, ui height) : key(key), val(val), height(height), next(height, nullptr) {}

        };

        ui maxHeight; // altura da lista (no mínimo 1)
        Node *head; // ponteiro para o primeiro nó da lista


        vector<Node*> getPrecursors(ui key, ui* quantityOfNodesVisited = nullptr) {
            vector<Node*> precursors(this->maxHeight, nullptr);
            Node *current = this->head;
            if (quantityOfNodesVisited != nullptr) {
                *quantityOfNodesVisited = 1; // Initialize the count of visited nodes
            }
            for (int level = this->maxHeight - 1; level >= 0; level--) {
                while (current->next[level] != nullptr && current->next[level]->key < key) {
                    current = current->next[level];
                    if (quantityOfNodesVisited != nullptr) {
                        *quantityOfNodesVisited += 1; // Increment the count of visited nodes
                    }
                }
                precursors[level] = current;
            }
            return precursors;
        }

        ui getMaxHeight() {
            return this->maxHeight + 1;
        }

        ui drawNewLevel() {
            ui newLevel = 1;
            while ((rng->next() % 100) < 50 && newLevel< this->getMaxHeight()) {
                newLevel++;
            }
            return newLevel; 
        }

    public:
        SkipList() {
            this->maxHeight = 1;
            this->head = new Node(0, 0, this->maxHeight);
        }

        int insert(int key) { // this could get a value as parameter
            vector<Node*> precursors = this->getPrecursors(key);
            if (precursors[0]->next[0] != nullptr && precursors[0]->next[0]->key == key) {
                // if key already exists
                return 0;
            }
            ui newLevel = this->drawNewLevel();
            if (newLevel > this->maxHeight) {
                this->head->next.resize(newLevel, nullptr);
                precursors.resize(newLevel, this->head);
                this->maxHeight = newLevel;
            }
            Node *newNode = new Node(key, 0, newLevel);
            for (int level = 0; level < newLevel; level++) {
                newNode->next[level] = precursors[level]->next[level];
                precursors[level]->next[level] = newNode;
            }
            return 1; 
        }

        FindResult find(int key) {
            ui* quantityOfNodesVisited = new ui(0);
            vector<Node*> precursors = this->getPrecursors(key, quantityOfNodesVisited);
            if (precursors[0]->next[0] != nullptr && precursors[0]->next[0]->key == key) {
                // if key exists
                return FindResult(*quantityOfNodesVisited, precursors[0]->next[0]->height);
            }
            return FindResult(*quantityOfNodesVisited, 0);
        }

        int remove(int key) {
            vector<Node*> precursors = this->getPrecursors(key);
            if (precursors[0]->next[0] == nullptr || precursors[0]->next[0]->key != key) {
                // if key does not exist
                return 0;
            }
            Node *nodeToRemove = precursors[0]->next[0];
            for (int level = 0; level < nodeToRemove->height; level++) {
                precursors[level]->next[level] = nodeToRemove->next[level];
            }
            while (this->maxHeight > 1 && this->head->next[this->maxHeight - 1] == nullptr) {
                this->maxHeight--;
            }
            return 1; 
        }
};

int main(int argc, char *argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ui seed, universeSize, burnInSize, numberOfOperations, findProbability, insertProbability, deleteProbability, printFrequency;
    cin >> seed >> universeSize >> burnInSize >> numberOfOperations >> findProbability >> insertProbability >> deleteProbability >> printFrequency;

    rng = new RGN(seed);
    // Após ler a entrada, o programa deve inicializar um RNG com semente S, e uma SL L inicialmente vazia.
    SkipList skipList = SkipList();

    // burn in OK
    for (int insertNumber = 0; insertNumber < burnInSize; insertNumber++) {
        skipList.insert(rng->next() % universeSize);
    }

    int printcount = 0;
    // operations
    for (int operationNumber = 0; operationNumber < numberOfOperations; operationNumber++) {
        int operation = rng->next() % (findProbability + insertProbability + deleteProbability);
        int data = rng->next() % universeSize;

        if (operation < findProbability) {
            // find
            if (operationNumber % printFrequency == 0) {
                printcount++;
                FindResult findResult = skipList.find(data);
                // F D H
                // F is just a flag
                // D é quantidade de nós diferentes visitados no caminho de busca, incluindo e head e terminando no precursor de X
                int d = findResult.quantityOfNodesVisited;
                // H é a altura do nó X, sendo 0 caso data n esteja na lista
                int h = findResult.heightOfNodeX;
                cout << "F " << d << " " << h << endl;
            }
        } else if (operation < findProbability + insertProbability) {
            // insert
            int r = skipList.insert(data);
            if (operationNumber % printFrequency == 0) {
                printcount++;
                // I R
                // I is just a letter to indicate that it is an insertion
                // R is the result, 1 if the insertion was successful, 0 otherwise (if the element already exists in the list)
                cout << "I " << r << endl;
            }
        } else {
            // delete
            int r = skipList.remove(data);
            if (operationNumber % printFrequency == 0) {
                printcount++;
                // D R
                // D is just a letter to indicate that it is a deletion
                // R is the result, 1 if the deletion was successful, 0 otherwise (if the element does not exist in the list)
                cout << "D " << r << endl;
            }
        }
    }

    return 0;
}
#include <bits/stdc++.h>

using namespace std;

#define ui unsigned int

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

// Define the structure for a Treap node
typedef struct Node
{
    int key;
    ui priority;
    struct Node *left;
    struct Node *right;
} Node;

class Treap
{
public:
    struct Node *root;

    Treap() : root(NULL) {}

    int insert(int key)
    {
        if (treapFind(key) == -1)
        {
            Node *newNode = createNode(key);
            this->root = treapInsert(this->root, newNode);
            return treapFind(key);
        }
        else
        {
            return -1;
        }
    }

    int query(int key)
    {
        return treapFind(key);
    }

    int del(int key)
    {
        int level = treapFind(key);
        if (level == -1)
        {
            return -1;
        }
        else
        {
            this->root = treapDelete(this->root, key);
            return level;
        }
    }

private:
    // Function to create a new Treap node
    Node *createNode(int key)
    {
        Node *newNode = new Node();

        newNode->key = key;
        newNode->priority = rng->next();
        newNode->left = newNode->right = NULL;

        return newNode;
    }

    int treapFind(int key)
    {
        Node *cur = this->root;
        int level = 0;

        while (cur != NULL)
        {
            if (key == cur->key)
            {
                return level;
            }
            else if (key < cur->key)
            {
                cur = cur->left;
            }
            else
            {
                cur = cur->right;
            }
            ++level;
        }
        return -1;
    }

    // Function to perform a split operation in the Treap
    pair<Node *, Node *> split(Node *root, int key)
    {
        if (root == NULL)
        {
            return {NULL, NULL};
        }
        else if (key == root->key)
        {
            return {root->left, root->right};
        }
        else if (key < root->key)
        {
            pair<Node *, Node *> result = split(root->left, key);
            Node *lt = result.first;
            root->left = result.second;

            return {lt, root};
        }
        else
        {
            pair<Node *, Node *> result = split(root->right, key);
            root->right = result.first;
            Node *gt = result.second;

            return {root, gt};
        }
    }

    // Function to perform an insert operation in the Treap
    Node *treapInsert(Node *root, Node *newNode)
    {
        if (root == NULL)
        {
            return newNode;
        }
        else if (newNode->priority > root->priority)
        {
            pair<Node *, Node *> result = split(root, newNode->key);
            newNode->left = result.first;
            newNode->right = result.second;

            return newNode;
        }
        else if (newNode->key < root->key)
        {
            root->left = treapInsert(root->left, newNode);
            return root;
        }
        else
        {
            root->right = treapInsert(root->right, newNode);
            return root;
        }
    }

    Node *treapMerge(Node *x, Node *y)
    {
        if (x == NULL)
        {
            return y;
        }
        else if (y == NULL)
        {
            return x;
        }
        else if (x->priority > y->priority)
        {
            x->right = treapMerge(x->right, y);
            return x;
        }
        else
        {
            y->left = treapMerge(x, y->left);
            return y;
        }
    }

    Node *treapDelete(Node *root, int key)
    {
        if (root == NULL)
        {
            return NULL;
        }
        else if (key == root->key)
        {
            return treapMerge(root->left, root->right);
        }
        else if (key < root->key)
        {
            root->left = treapDelete(root->left, key);
            return root;
        }
        else
        {
            root->right = treapDelete(root->right, key);
            return root;
        }
    }
};

// Function to print the output based on the operation type
void printOutput(char op, int key, int depth)
{
    printf("%c %d %d\n", op, key, depth);
}

// Function to simulate Treap operations
void simulateTreapOperations(ui S, int U, int B, int N, int I, int D, int Q, int P)
{
    Treap *treap = new Treap();
    rng = new RNG(S);

    // Perform burn-in insertions
    for (int i = 0; i < B; i++)
    {
        int key = (int)(rng->next() % U);
       treap->insert(key);
    }

    // Simulate N operations
    for (int opCount = B; opCount < B + N; opCount++)
    {
        int X = (int)(rng->next() % (I + D + Q));
        char op;

        if (X < I)
            op = 'I'; // Insert

        else if (X < I + D)
            op = 'D'; // Delete

        else
            op = 'Q'; // Query

        int key = (int)(rng->next() % U);
        int depth;

        // Perform Treap operation based on the selected operation type
        switch (op)
        {
        case 'I':
            depth = treap->insert(key);
            break;
        case 'Q':
            depth = treap->query(key);
            break;
        case 'D':
            depth = treap->del(key);
            break;
        }

        // Print output if required
        if (opCount % P == 0)
            printOutput(op, key, depth);
    }
}

int main()
{
    ui S;
    int U, B, N;
    int I, D, Q, P;

    // Read input
    scanf("%u %d %d %d %d %d %d %d", &S, &U, &B, &N, &I, &D, &Q, &P);

    // Simulate Treap operations
    simulateTreapOperations(S, U, B, N, I, D, Q, P);

    return 0;
}

#include <bits/stdc++.h>
using namespace std;

#define ui unsigned int

class Node
{
public:
    int key;

    struct Node *par;
    struct Node *left;
    struct Node *right;

    Node(int key, Node *par)
    {
        this->key = key;
        this->left = nullptr;
        this->right = nullptr;
        this->par = par;
    }
};

class SplayTree
{
public:
    Node *root;

    SplayTree() : root(nullptr) {}

    Node *search(Node *root, int key, int depth, bool print)
    {
        if (root == nullptr)
        {
            if (print)
                printf("Q %d -1\n", key);

            return root;
        }
        if (root->key == key)
        {
            if (print)
                printf("Q %d %d\n", key, depth);

            return root;
        }
        if (key > root->key)
            return search(root->right, key, ++depth, print);

        return search(root->left, key, ++depth, print);
    }

    Node *rotate_left(Node *node)
    {
        Node *par = node->par;
        Node *right = node->right;
        Node *rl = right->left;

        node->right = rl;

        if (rl)
            rl->par = node;

        if (par)
        {
            right->left = node;
            node->par = right;
            right->par = par;

            if (par->left && node == par->left)
                par->left = right;
            else if (par->right && node == par->right)
                par->right = right;
        }
        else
        {
            // If the rotated node is the root, update the root pointer
            right->left = node;
            node->par = right;
            right->par = nullptr;
            return right;
        }

        return right;
    }

    Node *rotate_right(Node *node)
    {
        Node *par = node->par;
        Node *left = node->left;
        Node *lr = left->right;

        node->left = lr;

        if (lr)
            lr->par = node;

        if (par)
        {
            left->right = node;
            node->par = left;
            left->par = par;

            if (par->left && node == par->left)
                par->left = left;
            else if (par->right && node == par->right)
                par->right = left;
        }
        else
        {
            // If the rotated node is the root, update the root pointer
            left->right = node;
            node->par = left;
            left->par = nullptr;
            return left;
        }

        return left;
    }

    Node *zig(Node *node)
    {
        Node *par = node->par;

        if (node == par->left)
            return rotate_right(par);

        else
            return rotate_left(par);
    }

    Node *zigzag(Node *node)
    {
        Node *par = node->par;
        Node *grandpar = par->par;

        if (par == grandpar->left)
        {
            if (node == par->left)
            {
                rotate_right(grandpar);
                return rotate_right(par);
            }
            else
            {
                rotate_left(par);
                return rotate_right(grandpar);
            }
        }
        else
        {
            if (node == par->right)
            {
                rotate_left(grandpar);
                return rotate_left(par);
            }
            else
            {
                rotate_right(par);
                return rotate_left(grandpar);
            }
        }
    }

    Node *splay(Node *node)
    {

        while (node->par != nullptr)
        {

            Node *grandpar = node->par->par;

            if (grandpar == nullptr)
                node = zig(node);

            else
                node = zigzag(node);
        }

        return node;
    }

    void query(int key, bool print)
    {
        Node *node = search(this->root, key, 0, print);

        if (node != nullptr)
            this->root = splay(node);
    }

    Node *insert(Node *root, Node *par, int key, int depth, bool print)
    {
        if (root == nullptr)
        {
            if (print)
                printf("I %d %d\n", key, depth);
            return new Node(key, par);
        }

        if (key > root->key)
            root->right = insert(root->right, root, key, ++depth, print);

        else if (key < root->key)
            root->left = insert(root->left, root, key, ++depth, print);

        else if (print)
            printf("I %d -1\n", key);

        return root;
    }
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

// GLOBALS
RNG *rng;
SplayTree *st;

int main()
{
    ui s, u, b, n, i, q, p;
    scanf("%u %u %u %u %u %u %u", &s, &u, &b, &n, &i, &q, &p);

    rng = new RNG(s);
    st = new SplayTree();

    // burn-in
    for (int i = 0; i < b; i++)
        st->root = st->insert(st->root, nullptr, (int)(rng->next() % u), 0, 0);

    char print;
    string op;
    int result;

    for (int k = 0; k < n; k++)
    {
        if ((k % p) == 0)
            print = 1;

        ui x = rng->next();
        op = (x % (i + q)) < i ? "ins" : "qry";

        x = rng->next() % u;
        if (op == "ins")
            st->root = st->insert(st->root, nullptr, (int)x, 0, print);

        else
            st->query((int)x, print);

        print = 0;
    }
    return 0;
}
#include <bits/stdc++.h>
using namespace std;

#define ui unsigned int
#define sgLog(alpha, n) (floor(log(n) / log(1 / alpha)))

// insertion return type : new root, new nodes qt, new size, rebuild?, dist to new leaf
#define insRTN tuple<Node *, int, int, int, int>

class Node
{
public:
    int key;
    struct Node *left;
    struct Node *right;

    Node(int key)
    {
        this->key = key;
        this->left = nullptr;
        this->right = nullptr;
    }
};

class ScapeGoat
{
private:
    int size;
    float alpha;

    int unbalanced(int size, int i)
    {
        return i > sgLog(this->alpha, size);
    }

    Node *flatten(Node *root, Node *head)
    {
        if (root == nullptr)
            return head;

        root->right = flatten(root->right, head);
        return flatten(root->left, root);
    }

    Node *rebuild(Node *root, int n)
    {
        Node *dummy = new Node(-1);
        Node *head = flatten(root, dummy);

        return build(head, n)->left;
    }

    Node *build(Node *head, int n)
    {
        if (n == 0)
        {
            head->left = nullptr;
            return head;
        }

        int nleft = n - 1 - ((n - 1) / 2);
        int nright = (n - 1) / 2;

        Node *mid = build(head, nleft);
        Node *handle = build(mid->right, nright);

        mid->right = handle->left;
        handle->left = mid;

        return handle;
    }

    int dfsCount(Node *root)
    {
        if (root == nullptr)
            return 0;

        // Initialize count with 1 for the current node
        int count = 1;

        count += dfsCount(root->left);
        count += dfsCount(root->right);

        return count;
    }

    insRTN SGInsert(Node *root, int depth, int n, int key)
    {
        int nroot, dist, chg, add;

        if (root == nullptr)
        {
            Node *newNode = new Node(key);
            chg = depth > sgLog(this->alpha, n + 1);

            return make_tuple(newNode, 1, 1, chg, 0);
        }

        else if (root->key == key)
            return make_tuple(root, 0, -1, 0, -1);

        else if (key < root->key)
        {
            int nleft;
            insRTN rtn = SGInsert(root->left, depth + 1, n, key);

            root->left = get<0>(rtn);
            add = get<1>(rtn);
            nleft = get<2>(rtn);
            chg = get<3>(rtn);
            dist = get<4>(rtn);

            nroot = chg ? 1 + nleft + dfsCount(root->right) : -1;
        }

        else
        {
            int nright;
            insRTN rtn = SGInsert(root->right, depth + 1, n, key);

            root->right = get<0>(rtn);
            add = get<1>(rtn);
            nright = get<2>(rtn);
            chg = get<3>(rtn);
            dist = get<4>(rtn);

            nroot = chg ? 1 + nright + dfsCount(root->left) : -1;
        }

        dist = dist + 1;

        if (chg && unbalanced(nroot, dist))
            return make_tuple(rebuild(root, nroot), add, nroot, false, dist);

        return make_tuple(root, add, nroot, chg, dist);
    }

public:
    Node *root;

    ScapeGoat(float a) : root(nullptr), size(0), alpha(a) {}

    tuple<int, int> insert(int key)
    {
        insRTN rtn = SGInsert(this->root, 0, size, key);

        this->root = get<0>(rtn);
        int added = get<1>(rtn);

        this->size = this->size + added;

        if (added)
        {
            int ordist = get<4>(rtn);
            int dist = this->query(this->root, key, 0);

            return make_tuple(ordist, dist);
        }
        else
            return make_tuple(-1, -1);
    }

    int query(Node *root, int key, int depth)
    {
        if (root == nullptr)
            return -1;

        if (root->key == key)
            return depth;

        if (key > root->key)
            return query(root->right, key, ++depth);

        return query(root->left, key, ++depth);
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
ScapeGoat *sgoat;

int main()
{
    ui s, u, b, n, i, q, p;
    float a;
    scanf("%u %u %f %u %u %u %u %u", &s, &u, &a, &b, &n, &i, &q, &p);

    rng = new RNG(s);
    sgoat = new ScapeGoat(a);

    // burn-in
    for (int i = 0; i < b; i++)
        sgoat->insert((int)(rng->next() % u));

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
        {
            auto result = sgoat->insert((int)x);

            if (print)
                printf("I %d %d %d\n", (int)x, get<0>(result), get<1>(result));
        }

        else
        {
            if (print)
            {
                int depth = sgoat->query(sgoat->root, (int)x, 0);
                printf("Q %d %d\n", (int)x, depth);
            };
        }

        print = 0;
    }
    return 0;
}
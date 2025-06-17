#include <iostream>
#include <queue>
using namespace std;

// Node structure holds pair<int,int>
struct Node
{
    pair<int, int> data;
    Node *left;
    Node *right;
    Node *parent;
    Node(pair<int, int> value) : data(value), left(nullptr), right(nullptr), parent(nullptr) {}
};

// Rotations and splay are unchanged except key type
void rotateLeft(Node *x, Node *&root)
{
    Node *y = x->right;
    x->right = y->left;
    if (y->left)
        y->left->parent = x;
    y->parent = x->parent;
    if (!x->parent)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

void rotateRight(Node *x, Node *&root)
{
    Node *y = x->left;
    x->left = y->right;
    if (y->right)
        y->right->parent = x;
    y->parent = x->parent;
    if (!x->parent)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->right = x;
    x->parent = y;
}

void splay(Node *x, Node *&root)
{
    while (x->parent)
    {
        Node *p = x->parent;
        Node *g = p->parent;
        if (!g)
        {
            // Zig step
            if (x == p->left)
                rotateRight(p, root);
            else
                rotateLeft(p, root);
        }
        else if ((x == p->left) == (p == g->left))
        {
            // Zig-Zig step
            if (x == p->left)
            {
                rotateRight(g, root);
                rotateRight(p, root);
            }
            else
            {
                rotateLeft(g, root);
                rotateLeft(p, root);
            }
        }
        else
        {
            // Zig-Zag step
            if (x == p->left)
            {
                rotateRight(p, root);
                rotateLeft(g, root);
            }
            else
            {
                rotateLeft(p, root);
                rotateRight(g, root);
            }
        }
    }
}

// BST insert (without balancing)
Node *insertInBST(Node *&root, pair<int, int> key)
{
    Node *parent = nullptr;
    Node *curr = root;
    while (curr)
    {
        parent = curr;
        if (key < curr->data)
            curr = curr->left;
        else if (curr->data < key)
            curr = curr->right;
        else
            return curr; // already exists
    }
    Node *newNode = new Node(key);
    newNode->parent = parent;
    if (!parent)
        root = newNode;
    else if (key < parent->data)
        parent->left = newNode;
    else
        parent->right = newNode;
    return newNode;
}

void insert(Node *&root, pair<int, int> key)
{
    Node *node = insertInBST(root, key);
    splay(node, root);
}

// Find without splaying
Node *contains(Node *root, pair<int, int> key)
{
    Node *curr = root;
    while (curr)
    {
        if (key < curr->data)
            curr = curr->left;
        else if (curr->data < key)
            curr = curr->right;
        else
            return curr;
    }
    return nullptr;
}

// Delete a key (if present) and rebalance
void del(Node *&root, pair<int, int> key)
{
    if (!root)
        return;
    // Splay the node (or nearest) to root
    Node *node = contains(root, key);
    if (!node || node->data != key)
        return;
    splay(node, root);
    // Now node is root
    Node *leftSub = root->left;
    Node *rightSub = root->right;
    if (leftSub)
        leftSub->parent = nullptr;
    if (rightSub)
        rightSub->parent = nullptr;
    delete root;
    root = nullptr;
    if (!leftSub && !rightSub)
        return;
    if (!leftSub)
    {
        root = rightSub;
        return;
    }
    // Splay the maximum of leftSub to attach rightSub
    Node *maxNode = leftSub;
    while (maxNode->right)
        maxNode = maxNode->right;
    splay(maxNode, leftSub);
    maxNode->right = rightSub;
    if (rightSub)
        rightSub->parent = maxNode;
    root = maxNode;
}

// Find deepest (farthest) node via BFS
Node *findFarthest(Node *root)
{
    if (!root)
        return nullptr;
    queue<Node *> q;
    q.push(root);
    Node *curr = nullptr;
    while (!q.empty())
    {
        curr = q.front();
        q.pop();
        if (curr->left)
            q.push(curr->left);
        if (curr->right)
            q.push(curr->right);
    }
    return curr;
}

// Pre-order traversal for output
void preorder(Node *root)
{
    if (!root)
        return;
    cout << "(" << root->data.first << "," << root->data.second << ") ";
    preorder(root->left);
    preorder(root->right);
}

class SplayCache
{
    Node *root;
    int capacity;
    int size;

public:
    SplayCache(int capacity) : root(nullptr), capacity(capacity), size(0) {}

    // Read returns true if hit, false if miss (and inserts on miss)
    bool Read(pair<int, int> key)
    {
        Node *temp = contains(root, key);
        if (temp)
        {
            splay(temp, root);
            return true;
        }
        if (size == capacity)
            evictDeepest();
        insert(root, key);
        size++;
        return false;
    }

    void dump()
    {
        cout << "Cache contents (pre-order): ";
        preorder(root);
        cout << "\n";
    }

private:
    void evictDeepest()
    {
        if (!root)
            return;
        Node *rem = findFarthest(root);
        del(root, rem->data);
        size--;
    }
};

int main()
{
    SplayCache cache(3);
    vector<pair<int, int>> testKeys = {
        {1, 2}, {2, 3}, {3, 4}, {1, 2}, {4, 5}, {5, 6}, {2, 3}, {6, 7}, {3, 4}};
    cout << "=== SplayCache Test (capacity=3) ===\n";
    for (auto key : testKeys)
    {
        bool hit = cache.Read(key);
        cout << (hit ? "Hit:  " : "Miss: ")
             << "(" << key.first << "," << key.second << ")\n";
        cache.dump();
        cout << "--------------------------\n";
    }
    return 0;
}

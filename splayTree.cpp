#include <iostream>
#include <queue>
using namespace std;

struct Node
{
    int data;
    Node *left;
    Node *right;
    Node *parent;

    Node(int value)
    {
        data = value;
        left = right = parent = nullptr;
    }
};

void rotateLeft(Node *x, Node *&root)
{
    Node *y = x->right;
    x->right = y->left;
    if (y->left)
        y->left->parent = x;
    y->parent = x->parent;
    if (!x->parent)
        root = y;
    else if (x->parent->left == x)
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
    else if (x->parent->left == x)
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

Node *find(int key, Node *&root)
{
    Node *curr = root;
    Node *last = nullptr;
    while (curr)
    {
        last = curr;
        if (key < curr->data)
            curr = curr->left;
        else if (key > curr->data)
            curr = curr->right;
        else
        {
            splay(curr, root);
            return curr;
        }
    }
    if (last)
        splay(last, root);
    return last;
}

Node *insertInBST(Node *&root, int key)
{
    Node *parent = nullptr;
    Node *curr = root;
    while (curr)
    {
        parent = curr;
        if (key < curr->data)
            curr = curr->left;
        else if (key > curr->data)
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

void insert(Node *&root, int key)
{
    Node *node = insertInBST(root, key);
    splay(node, root);
}

void del(Node *&root, int key)
{
    if (!root)
        return;

    Node *node = find(key, root);
    if (!node || node->data != key)
    {
        cout << "Key " << key << " is not present\n";
        return;
    }

    // Node to delete is now root
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
    // Splay max of leftSub
    Node *maxNode = leftSub;
    while (maxNode->right)
        maxNode = maxNode->right;
    splay(maxNode, leftSub);

    maxNode->right = rightSub;
    if (rightSub)
        rightSub->parent = maxNode;

    root = maxNode;
}

Node *findFarthest(Node *root)
{
    if (!root)
        return NULL;
    queue<Node *> q;
    q.push(root);
    Node *curr = NULL;
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

Node *contains(Node *root, int key)
{
    Node *curr = root;
    while (curr)
    {
        if (key < curr->data)
            curr = curr->left;
        else if (key > curr->data)
            curr = curr->right;
        else
            return curr;
    }
    return NULL;
}

void preorder(Node *root)
{
    if (!root)
        return;
    cout << root->data << " ";
    preorder(root->left);
    preorder(root->right);
}

class SplayCache
{
    Node *root;
    int capacity;
    int size;

public:
    SplayCache(int capacity)
    {
        root = NULL;
        this->capacity = capacity;
        size = 0;
    }

    bool Read(int key)
    {
        Node *temp = contains(root, key);
        if (temp)
        {
            splay(temp, root);
            return true;
        }
        if (size == capacity)
        {
            evictDeepest();
        }
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


    int testKeys[] = {1, 2, 3, 1, 4, 5, 2, 6, 3};
    
    cout << "=== SplayCache Test (capacity=3) ===\n";
    for (int key : testKeys)
    {
        bool hit = cache.Read(key);
        cout << (hit ? "Hit:  " : "Miss: ") << key << "\n";
        cache.dump();
        cout << "--------------------------\n";
    }

    return 0;
}
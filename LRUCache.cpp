#include <iostream>
#include <unordered_map>
using namespace std;

// Hash function for pair<int,int>
struct pair_hash
{
    size_t operator()(const pair<int, int> &p) const noexcept
    {
        // Combine the hashes of the two ints
        return hash<int>()(p.first) ^ (hash<int>()(p.second) << 1);
    }
};

class Node
{
public:
    pair<int, int> key;
    int val;
    Node *next;
    Node *prev;
    Node(pair<int, int> _key, int _val) : key(_key), val(_val), next(nullptr), prev(nullptr) {}
};

class LRUCache
{
public:
    Node *head = new Node({-1, -1}, -1);
    Node *tail = new Node({-1, -1}, -1);
    int cap;
    unordered_map<pair<int, int>, Node *, pair_hash> m;

    LRUCache(int capacity) : cap(capacity)
    {
        head->next = tail;
        tail->prev = head;
    }

    void insert_atHead(Node *newnode)
    {
        Node *temp = head->next;
        newnode->next = temp;
        newnode->prev = head;
        head->next = newnode;
        temp->prev = newnode;
    }

    void deleteNode(Node *delnode)
    {
        Node *delprev = delnode->prev;
        Node *delnext = delnode->next;
        delprev->next = delnext;
        delnext->prev = delprev;
    }

    int get(const pair<int, int> &key)
    {
        if (m.find(key) == m.end()) // Miss
        {
            // If at capacity, evict least recently used first
            if (m.size() == cap)
            {
                m.erase(tail->prev->key);
                deleteNode(tail->prev);
            }
            // Insert new node with fallback -1
            insert_atHead(new Node(key, 0));

            m[key] = head->next;
            return -1;
        }
        else // Hit
        {
            Node *resnode = m[key];
            int res = resnode->val;

            // Move it to the head
            deleteNode(resnode);
            insert_atHead(resnode);

            return res;
        }
    }

    // Insert or update (key,value). Evict LRU if at capacity.
    void put(const pair<int, int> &key, int value)
    {
        if (m.find(key) != m.end())
        {
            // Remove old node
            Node *existingnode = m[key];
            m.erase(key);
            deleteNode(existingnode);
        }
        if (m.size() == cap)
        {
            // Evict least-recently used (tail->prev)
            m.erase(tail->prev->key);
            deleteNode(tail->prev);
        }
        // Insert new node at head
        insert_atHead(new Node(key, value));
        m[key] = head->next;
    }
};

int main()
{
    // Example test with pair keys
    // Capacity = 2
    LRUCache cache(2);

    cache.put({1, 2}, 10);
    cache.put({2, 3}, 20);
    cout << cache.get({1, 2}) << endl; // Prints 10 (Hit)

    // This will cause eviction of (2, 3) due to LRU
    cache.put({4, 5}, 40);
    cout << cache.get({2, 3}) << endl; // Prints -1 (Miss)
    cout << cache.get({4, 5}) << endl; // Prints 40 (Hit)
    cout << cache.get({1, 2}) << endl; // Prints 10 (Hit)
    cout << cache.get({1, 2}) << endl; // Prints 10 (Hit)

    return 0;
}

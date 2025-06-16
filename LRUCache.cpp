#include <iostream>
#include <unordered_map>
using namespace std;

class Node
{
public:
    int key, val;
    Node *next;
    Node *prev;
    Node(int _key, int _val)
    {
        key = _key;
        val = _val;
        next = prev = nullptr;
    }
};

class LRUCache
{
public:
    Node *head = new Node(-1, -1);
    Node *tail = new Node(-1, -1);
    int cap;
    unordered_map<int, Node *> m;

    LRUCache(int capacity)
    {
        cap = capacity;
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

    void deletenode(Node *delnode)
    {
        Node *delprev = delnode->prev;
        Node *delnext = delnode->next;
        delprev->next = delnext;
        delnext->prev = delprev;
    }

    int get(int key)
    {
        if (m.find(key) != m.end())
        {
            Node *resnode = m[key];
            int res = resnode->val;
            m.erase(key);
            deletenode(resnode);
            insert_atHead(resnode);
            m[key] = head->next;
            return res;
        }
        return -1;
    }

    void put(int key, int value)
    {
        if (m.find(key) != m.end())
        {
            Node *existingnode = m[key];
            m.erase(key);
            deletenode(existingnode);
        }
        if (m.size() == cap)
        {
            m.erase(tail->prev->key);
            deletenode(tail->prev);
        }
        insert_atHead(new Node(key, value));
        m[key] = head->next;
    }
};

int main()
{
    LRUCache cache(2);

    cache.put(1, 1);
    cache.put(2, 2);
    cout << cache.get(1) << endl;

    cache.put(3, 3);
    cout << cache.get(2) << endl;

    cache.put(4, 4);
    cout << cache.get(1) << endl;
    cout << cache.get(3) << endl;
    cout << cache.get(4) << endl;

    return 0;
}
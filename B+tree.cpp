#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// B plus tree class
template <typename T>
class BPlusTree
{
public:
    // structure to create a linkedBlock
    struct LinkedBlock
    {
        bool isLeaf;
        vector<T> keys;
        vector<LinkedBlock *> children;
        LinkedBlock *next;

        LinkedBlock(bool leaf = false)
            : isLeaf(leaf), next(nullptr)
        {
        }
    };

    LinkedBlock *root;
    // Minimum degree (defines the range for the number of
    // keys)
    int t;

    // Function to split a child linkedBlock
    void splitChild(LinkedBlock *parent, int index, LinkedBlock *child);

    // Function to insert a key in a non-full linkedBlock
    void insertNonFull(LinkedBlock *linkedBlock, T key);

    // Function to remove a key from a linkedBlock
    void remove(LinkedBlock *linkedBlock, T key);

    // Function to borrow a key from the previous sibling
    void borrowFromPrev(LinkedBlock *linkedBlock, int index);

    // Function to borrow a key from the next sibling
    void borrowFromNext(LinkedBlock *linkedBlock, int index);

    // Function to merge two linkedBlocks
    void merge(LinkedBlock *linkedBlock, int index);

    // Function to print the tree
    void printTree(LinkedBlock *linkedBlock, int level);

public:
    BPlusTree(int degree) : root(nullptr), t(degree) {}

    void insert(T key);
    bool search(T key);
    void remove(T key);
    vector<T> rangeQuery(T lower, T upper);
    void printTree();
};

// Implementation of splitChild function
template <typename T>
void BPlusTree<T>::splitChild(LinkedBlock *parent, int index,
                              LinkedBlock *child)
{
    LinkedBlock *newChild = new LinkedBlock(child->isLeaf);
    parent->children.insert(
        parent->children.begin() + index + 1, newChild);
    parent->keys.insert(parent->keys.begin() + index,
                        child->keys[t - 1]);

    newChild->keys.assign(child->keys.begin() + t,
                          child->keys.end());
    child->keys.resize(t - 1);

    if (!child->isLeaf)
    {
        newChild->children.assign(child->children.begin() + t,
                                  child->children.end());
        child->children.resize(t);
    }

    if (child->isLeaf)
    {
        newChild->next = child->next;
        child->next = newChild;
    }
}

// Implementation of insertNonFull function
template <typename T>
void BPlusTree<T>::insertNonFull(LinkedBlock *linkedBlock, T key)
{
    if (linkedBlock->isLeaf)
    {
        linkedBlock->keys.insert(upper_bound(linkedBlock->keys.begin(),
                                             linkedBlock->keys.end(),
                                             key),
                                 key);
    }
    else
    {
        int i = linkedBlock->keys.size() - 1;
        while (i >= 0 && key < linkedBlock->keys[i])
        {
            i--;
        }
        i++;
        if (linkedBlock->children[i]->keys.size() == 2 * t - 1)
        {
            splitChild(linkedBlock, i, linkedBlock->children[i]);
            if (key > linkedBlock->keys[i])
            {
                i++;
            }
        }
        insertNonFull(linkedBlock->children[i], key);
    }
}

// Implementation of remove function
template <typename T>
void BPlusTree<T>::remove(LinkedBlock *linkedBlock, T key)
{
    // If linkedBlock is a leaf
    if (linkedBlock->isLeaf)
    {
        auto it = find(linkedBlock->keys.begin(), linkedBlock->keys.end(),
                       key);
        if (it != linkedBlock->keys.end())
        {
            linkedBlock->keys.erase(it);
        }
    }
    else
    {
        int idx = lower_bound(linkedBlock->keys.begin(),
                              linkedBlock->keys.end(), key) -
                  linkedBlock->keys.begin();
        if (idx < linkedBlock->keys.size() && linkedBlock->keys[idx] == key)
        {
            if (linkedBlock->children[idx]->keys.size() >= t)
            {
                LinkedBlock *predLinkedBlock = linkedBlock->children[idx];
                while (!predLinkedBlock->isLeaf)
                {
                    predLinkedBlock = predLinkedBlock->children.back();
                }
                T pred = predLinkedBlock->keys.back();
                linkedBlock->keys[idx] = pred;
                remove(linkedBlock->children[idx], pred);
            }
            else if (linkedBlock->children[idx + 1]->keys.size() >= t)
            {
                LinkedBlock *succLinkedBlock = linkedBlock->children[idx + 1];
                while (!succLinkedBlock->isLeaf)
                {
                    succLinkedBlock = succLinkedBlock->children.front();
                }
                T succ = succLinkedBlock->keys.front();
                linkedBlock->keys[idx] = succ;
                remove(linkedBlock->children[idx + 1], succ);
            }
            else
            {
                merge(linkedBlock, idx);
                remove(linkedBlock->children[idx], key);
            }
        }
        else
        {
            if (linkedBlock->children[idx]->keys.size() < t)
            {
                if (idx > 0 && linkedBlock->children[idx - 1]->keys.size() >= t)
                {
                    borrowFromPrev(linkedBlock, idx);
                }
                else if (idx < linkedBlock->children.size() - 1 && linkedBlock->children[idx + 1]
                                                                           ->keys.size() >= t)
                {
                    borrowFromNext(linkedBlock, idx);
                }
                else
                {
                    if (idx < linkedBlock->children.size() - 1)
                    {
                        merge(linkedBlock, idx);
                    }
                    else
                    {
                        merge(linkedBlock, idx - 1);
                    }
                }
            }
            remove(linkedBlock->children[idx], key);
        }
    }
}

// Implementation of borrowFromPrev function
template <typename T>
void BPlusTree<T>::borrowFromPrev(LinkedBlock *linkedBlock, int index)
{
    LinkedBlock *child = linkedBlock->children[index];
    LinkedBlock *sibling = linkedBlock->children[index - 1];

    child->keys.insert(child->keys.begin(),
                       linkedBlock->keys[index - 1]);
    linkedBlock->keys[index - 1] = sibling->keys.back();
    sibling->keys.pop_back();

    if (!child->isLeaf)
    {
        child->children.insert(child->children.begin(),
                               sibling->children.back());
        sibling->children.pop_back();
    }
}

// Implementation of borrowFromNext function
template <typename T>
void BPlusTree<T>::borrowFromNext(LinkedBlock *linkedBlock, int index)
{
    LinkedBlock *child = linkedBlock->children[index];
    LinkedBlock *sibling = linkedBlock->children[index + 1];

    child->keys.push_back(linkedBlock->keys[index]);
    linkedBlock->keys[index] = sibling->keys.front();
    sibling->keys.erase(sibling->keys.begin());

    if (!child->isLeaf)
    {
        child->children.push_back(
            sibling->children.front());
        sibling->children.erase(sibling->children.begin());
    }
}

// Implementation of merge function
template <typename T>
void BPlusTree<T>::merge(LinkedBlock *linkedBlock, int index)
{
    LinkedBlock *child = linkedBlock->children[index];
    LinkedBlock *sibling = linkedBlock->children[index + 1];

    child->keys.push_back(linkedBlock->keys[index]);
    child->keys.insert(child->keys.end(),
                       sibling->keys.begin(),
                       sibling->keys.end());
    if (!child->isLeaf)
    {
        child->children.insert(child->children.end(),
                               sibling->children.begin(),
                               sibling->children.end());
    }

    linkedBlock->keys.erase(linkedBlock->keys.begin() + index);
    linkedBlock->children.erase(linkedBlock->children.begin() + index + 1);

    delete sibling;
}

// Implementation of printTree function
template <typename T>
void BPlusTree<T>::printTree(LinkedBlock *linkedBlock, int level)
{
    if (linkedBlock != nullptr)
    {
        for (int i = 0; i < level; ++i)
        {
            cout << "  ";
        }
        for (const T &key : linkedBlock->keys)
        {
            cout << key << " ";
        }
        cout << endl;
        for (LinkedBlock *child : linkedBlock->children)
        {
            printTree(child, level + 1);
        }
    }
}

// Implementation of printTree wrapper function
template <typename T>
void BPlusTree<T>::printTree()
{
    printTree(root, 0);
}

// Implementation of search function
template <typename T>
bool BPlusTree<T>::search(T key)
{
    LinkedBlock *current = root;
    while (current != nullptr)
    {
        int i = 0;
        while (i < current->keys.size() && key > current->keys[i])
        {
            i++;
        }
        if (i < current->keys.size() && key == current->keys[i])
        {
            return true;
        }
        if (current->isLeaf)
        {
            return false;
        }
        current = current->children[i];
    }
    return false;
}

// Implementation of range query function
template <typename T>
vector<T> BPlusTree<T>::rangeQuery(T lower, T upper)
{
    vector<T> result;
    LinkedBlock *current = root;
    while (!current->isLeaf)
    {
        int i = 0;
        while (i < current->keys.size() && lower > current->keys[i])
        {
            i++;
        }
        current = current->children[i];
    }
    while (current != nullptr)
    {
        for (const T &key : current->keys)
        {
            if (key >= lower && key <= upper)
            {
                result.push_back(key);
            }
            if (key > upper)
            {
                return result;
            }
        }
        current = current->next;
    }
    return result;
}

// Implementation of insert function
template <typename T>
void BPlusTree<T>::insert(T key)
{
    if (root == nullptr)
    {
        root = new LinkedBlock(true);
        root->keys.push_back(key);
    }
    else
    {
        if (root->keys.size() == 2 * t - 1)
        {
            LinkedBlock *newRoot = new LinkedBlock();
            newRoot->children.push_back(root);
            splitChild(newRoot, 0, root);
            root = newRoot;
        }
        insertNonFull(root, key);
    }
}

// Implementation of remove function
template <typename T>
void BPlusTree<T>::remove(T key)
{
    if (root == nullptr)
    {
        return;
    }
    remove(root, key);
    if (root->keys.empty() && !root->isLeaf)
    {
        LinkedBlock *tmp = root;
        root = root->children[0];
        delete tmp;
    }
}
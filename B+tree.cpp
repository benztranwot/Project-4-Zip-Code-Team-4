#include <algorithm>
#include <iostream>
#include <vector>
#include "PostalRecord.h"
using namespace std;

/**
 * @brief B+ tree class template.
 *
 * Implements a basic B+ tree with insert, remove, search,
 * range query, and printing capabilities. The tree nodes
 * are represented by LinkedBlock structures.
 *
 * @tparam T Type of keys stored in the B+ tree.
 */
template <typename T>
class BPlusTree
{
public:
    /**
     * @brief Node structure representing a B+ tree block.
     *
     * Each LinkedBlock can be either an internal node or a leaf.
     * Leaf nodes are linked together using the @c next pointer to
     * support efficient range queries.
     */
    struct LinkedBlock
    {
        /// @brief Flag indicating whether this node is a leaf.
        bool isLeaf;

        /// @brief Keys stored in this node (sorted).
        vector<T> keys;

        /**
         * @brief Child pointers.
         *
         * For internal nodes, this holds pointers to child nodes.
         * For leaf nodes, this vector is typically empty.
         */
        vector<LinkedBlock *> children;

        /**
         * @brief Pointer to the next leaf node.
         *
         * Used only when this node is a leaf. Supports fast
         * traversal for range queries.
         */
        LinkedBlock *next;

        /**
         * @brief Constructs a LinkedBlock node.
         *
         * @param leaf True if the node should be a leaf, false otherwise.
         */
        LinkedBlock(bool leaf = false)
            : isLeaf(leaf), next(nullptr)
        {
        }
    };

    /// @brief Pointer to the root node of the B+ tree.
    LinkedBlock *root;

    /**
     * @brief Minimum degree of the B+ tree.
     *
     * Defines the minimum and maximum number of keys in a node.
     * Each node (except root) has at least @c t-1 keys and at most
     * @c 2*t-1 keys.
     */
    int t;

    /**
     * @brief Splits a full child node of an internal node.
     *
     * Used during insertion when a child node is full. The node
     * is split into two nodes, and a key is promoted into the parent.
     *
     * @param parent Pointer to the parent node.
     * @param index Index of the child in the parent's children vector.
     * @param child Pointer to the child node to split.
     */
    void splitChild(LinkedBlock *parent, int index, LinkedBlock *child);

    /**
     * @brief Inserts a key into a non-full node.
     *
     * Called by insert() after ensuring that the root is not full.
     *
     * @param linkedBlock Pointer to the node that is guaranteed to be non-full.
     * @param key Key to insert.
     */
    void insertNonFull(LinkedBlock *linkedBlock, T key);

    /**
     * @brief Removes a key from a subtree rooted at a given node.
     *
     * Internal recursive helper for the public remove(T key) function.
     *
     * @param linkedBlock Pointer to the current node.
     * @param key Key to remove.
     */
    void remove(LinkedBlock *linkedBlock, T key);

    /**
     * @brief Borrows a key from the previous sibling of a child.
     *
     * Used during deletion when a child has too few keys and its
     * left sibling can spare a key.
     *
     * @param linkedBlock Pointer to the parent node.
     * @param index Index of the child in the parent's children vector.
     */
    void borrowFromPrev(LinkedBlock *linkedBlock, int index);

    /**
     * @brief Borrows a key from the next sibling of a child.
     *
     * Used during deletion when a child has too few keys and its
     * right sibling can spare a key.
     *
     * @param linkedBlock Pointer to the parent node.
     * @param index Index of the child in the parent's children vector.
     */
    void borrowFromNext(LinkedBlock *linkedBlock, int index);

    /**
     * @brief Merges a child node with its right sibling.
     *
     * Used during deletion when both a child and its sibling have
     * the minimum number of keys, and they are combined into a single node.
     *
     * @param linkedBlock Pointer to the parent node.
     * @param index Index of the left child to merge with its right sibling.
     */
    void merge(LinkedBlock *linkedBlock, int index);

    /**
     * @brief Prints the keys of the subtree rooted at a given node.
     *
     * Recursive helper for the public printTree() function.
     *
     * @param linkedBlock Pointer to the current node.
     * @param level Current depth level in the tree (for indentation).
     */
    void printTree(LinkedBlock *linkedBlock, int level);

public:
    /**
     * @brief Constructs a B+ tree with a given minimum degree.
     *
     * @param degree Minimum degree (t) of the B+ tree.
     */
    BPlusTree(int degree) : root(nullptr), t(degree) {}

    /**
     * @brief Inserts a key into the B+ tree.
     *
     * If the root is full, it is split and the tree height increases.
     *
     * @param key Key to insert.
     */
    void insert(T key);

    /**
     * @brief Searches for a key in the B+ tree.
     *
     * @param key Key to search for.
     * @return true If the key is found.
     * @return false If the key is not found.
     */
    bool search(T key);

    /**
     * @brief Removes a key from the B+ tree.
     *
     * Handles root adjustment if it becomes empty after deletion.
     *
     * @param key Key to remove.
     */
    void remove(T key);

    /**
     * @brief Performs a range query on the B+ tree.
     *
     * Returns all keys in the closed interval [lower, upper].
     *
     * @param lower Lower bound of the range (inclusive).
     * @param upper Upper bound of the range (inclusive).
     * @return vector<T> Collection of keys in the specified range.
     */
    vector<T> rangeQuery(T lower, T upper);

    /**
     * @brief Prints the entire B+ tree to standard output.
     *
     * Wrapper around the recursive printTree(LinkedBlock*, int) function.
     */
    void printTree();
};

// Implementation of splitChild function
/**
 * @brief Splits a full child node of an internal node.
 *
 * See BPlusTree::splitChild for detailed description.
 */
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
/**
 * @brief Inserts a key into a node that is guaranteed to be non-full.
 *
 * See BPlusTree::insertNonFull for detailed description.
 */
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

// Implementation of remove function (internal helper)
/**
 * @brief Internal recursive remove helper.
 *
 * See BPlusTree::remove(LinkedBlock*, T) for detailed description.
 */
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
/**
 * @brief Borrows a key from the previous sibling of a child node.
 *
 * See BPlusTree::borrowFromPrev for detailed description.
 */
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
/**
 * @brief Borrows a key from the next sibling of a child node.
 *
 * See BPlusTree::borrowFromNext for detailed description.
 */
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
/**
 * @brief Merges a child node with its right sibling.
 *
 * See BPlusTree::merge for detailed description.
 */
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
/**
 * @brief Recursively prints the subtree rooted at a given node.
 *
 * See BPlusTree::printTree(LinkedBlock*, int) for detailed description.
 */
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
/**
 * @brief Prints the entire B+ tree starting from the root.
 */
template <typename T>
void BPlusTree<T>::printTree()
{
    printTree(root, 0);
}

// Implementation of search function
/**
 * @brief Searches for a key starting from the root node.
 *
 * See BPlusTree::search for detailed description.
 */
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
/**
 * @brief Executes a range query on the B+ tree.
 *
 * Starts from the leaf node where the lower bound would
 * be located and traverses forward using leaf links.
 *
 * See BPlusTree::rangeQuery for detailed description.
 */
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
/**
 * @brief Inserts a key into the B+ tree, handling root splitting if necessary.
 *
 * See BPlusTree::insert for detailed description.
 */
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

// Implementation of remove function (public)
/**
 * @brief Removes a key from the B+ tree, adjusting the root if needed.
 *
 * See BPlusTree::remove(T) for detailed description.
 */
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

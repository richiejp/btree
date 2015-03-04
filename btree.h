#include <QString>
#include <QVariant>
#ifndef BTREE_H
#define BTREE_H

enum EType {
    KeyType,
    NodeType,
    ElementType
};

class BNode;

class BNodeElement
{
    friend class BTree;
public:
    virtual EType type() const;

protected:
    BNode *m_node;
};

class BNodePtr;

class BKey : BNodeElement
{
    friend class Btree_test;
    friend class BTree;

public:
    EType type() const;

private:
    int m_key;
    BNodePtr *m_next;

};

class BNodePtr : BNodeElement
{
    friend class Btree_test;
    friend class BTree;

public:
    EType type() const;

private:
    BNode *m_ptr = NULL;
    BKey *m_next = NULL;
};

class BNode
{
    friend class Btree_test;
    friend class BTree;

public:

private:
    unsigned int m_degree = 0;
    BNode *m_parent = NULL;
    BNodePtr *m_firstPtr;
};

class BTree
{
    friend class Btree_test;

public:
    BTree(unsigned int degree, int key);

    BNodeElement *find(const int key) const;
    bool contains(const int key) const;

    bool add(const int key);

private:
    unsigned int m_degree;
    BNodePtr m_root;

    bool isRoot(const BNode *n) const;
    bool isFull(const BNode *n) const;
    BNodeElement *nextElement(BNodeElement *e) const;
    BNodePtr* median(BNode *n) const;

    void initNode(BNode *n);
    BNode *newRootNode(BKey *newRootKey);
    BNode *newSplitNode(BNodePtr *firstPtr);
    BKey *newKPPair(const int key);
    void newPtr(BKey *prevKey);
    BNodePtr *nearest(const BKey *k, BNode *n);
    void insert(BKey *key, BNodePtr *nearest);
    void bringup(BKey *key);
    void split(BNode *node);
};


#endif // BTREE_H

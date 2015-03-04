#include "btree.h"

EType BNodeElement::type() const {
    return ElementType;
}

EType BKey::type() const {
    return KeyType;
}

EType BNodePtr::type() const {
    return NodeType;
}

BTree::BTree(unsigned int degree, int key) {
    m_degree = degree;
    m_root.m_node = newRootNode(newKPPair(key));
}

bool BTree::isRoot(const BNode *n) const {
    return m_root.m_ptr == n;
}

bool BTree::isFull(const BNode *n) const {
    return n->m_degree >= m_degree * 2;
}

BNodeElement *BTree::nextElement(BNodeElement *e) const {
    if(e->type() == KeyType) {
        return ((BKey *)e)->m_next;
    }
    return ((BNodePtr *)e)->m_next;
}

BNodePtr *BTree::median(BNode *n) const {
    BNodeElement *e = n->m_firstPtr->m_next;
    for(unsigned int c = 2;
        c < n->m_degree - 1;
        c++)
    {
        e = nextElement(e);
    }
    Q_ASSERT(e->type() == NodeType);
    return (BNodePtr *)e;
}

void BTree::initNode(BNode *n) {
    BNodeElement *e = n->m_firstPtr;
    Q_ASSERT(e != NULL);

    int c = 1;
    while(e != NULL) {
        e->m_node = n;
        switch(e->type()){
        case KeyType:
            e = ((BKey *)e)->m_next;
            break;
        case NodeType:
            c++;
            e = ((BNodePtr *)e)->m_next;
            break;
        }
    }
    n->m_degree = c;
}

BNode *BTree::newRootNode(BKey *newRootKey) {
    Q_ASSERT(newRootKey->m_next != NULL);

    BNode *n = new BNode();
    n->m_firstPtr = new BNodePtr();
    n->m_firstPtr->m_next = newRootKey;
    n->m_firstPtr->m_ptr = m_root.m_ptr;
    initNode(n);
    m_root.m_ptr = n;
    return n;
}

BNode *BTree::newSplitNode(BNodePtr *firstPtr) {
    BNode *n = new BNode();
    n->m_firstPtr = firstPtr;
    n->m_parent = firstPtr->m_node->m_parent;
    initNode(n);
    return n;
}

BKey *BTree::newKPPair(const int key) {
    BKey *k = new BKey();
    k->m_key = key;
    newPtr(k);
    return k;
}

void BTree::newPtr(BKey *prevKey) {
    BNodePtr *ptr = new BNodePtr();
    ptr->m_node = prevKey->m_node;
    prevKey->m_next = ptr;
}

BNodePtr *BTree::nearest(const BKey *k, BNode *n) {
    BNodePtr *np = n->m_firstPtr;
    for(;np->m_next != NULL;
        np = np->m_next->m_next)
    {
        /*if(np->m_next->m_key > k->m_key) {
            break;
        }*/
        if(np->m_ptr == k->m_node) {
            break;
        }
    }

    return np;
}

void BTree::insert(BKey *key, BNodePtr *nearest) {
    BNode *n = nearest->m_node;

    if(isFull(n)) {
        split(n);
        n = nearest->m_node;
    }

    // n{ ..., nearest, key, np, nearest->next, ...}
    BNodePtr *np = key->m_next;
    np->m_next = nearest->m_next;
    nearest->m_next = key;
    key->m_node = n;
    np->m_node = n;
    n->m_degree++;
}

void BTree::bringup(BKey *key) {
    Q_ASSERT(key->m_next != NULL);

    if(isRoot(key->m_node)) {
        newRootNode(key);
    } else {
        insert(key, nearest(key, key->m_node->m_parent));
    }
}

void BTree::split(BNode *n) {
    BNodePtr *mp = median(n);
    BKey *mk = mp->m_next;
    BNodePtr *rp = mk->m_next;
    mp->m_next = NULL;
    initNode(n);

    newPtr(mk);
    BNode *nn = newSplitNode(rp);
    mk->m_next->m_ptr = nn;
    bringup(mk);

    n->m_parent = mk->m_node;
    nn->m_parent = n->m_parent;
}

BNodeElement *BTree::find(const int key) const {
    BNodePtr *np = m_root.m_ptr->m_firstPtr;
    forever {
        BKey *k = np->m_next;
        if(k == NULL || k->m_key > key) {
            if(np->m_ptr != NULL) {
                np = np->m_ptr->m_firstPtr;
            } else {
                break;
            }
        } else if(key > k->m_key) {
            np = k->m_next;
        } else {
            return k;
        }
    }
    return np;
}

bool BTree::contains(const int key) const {
    return find(key)->type() == KeyType;
}

bool BTree::add(const int key) {
    BNodeElement *e = find(key);
    switch(e->type()) {
    case KeyType:
        return false;
    case NodeType:
        insert(newKPPair(key), (BNodePtr *)e);
    }
    return true;
}

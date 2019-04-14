#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include "Queue.h"
#include "KaryTree.h"

//////////////////////////////////////////////////
//  private
KTN_t *createNodeKT(int value, int K, void *element);
KTN_t *findNodeOnKT(KTN_t *R, int keyValue, KT_OPTION_e option);
KTN_t *findLeftmostLeefNodeOnKT(KTN_t *R);
void *insertElementOnKTslave(KTN_t *R, void *child);
void *destroyNodeKTslave(KTN_t *R, void *option);
void *findNodeOnKTslave(KTN_t *R, void *option);

//////////////////////////////////////////////////
//  public
bool destroyKT(KTN_t *R, KT_OPTION_e option) {
    // Block illegal parameters
    if (R == NULL) return false;
    
    // Detach from the tree.
    if (R->parent != NULL) {
        if (R->bigBrother == NULL) {    // Eldest son
            R->parent->child = R->littleBrother;
        }
        else {
            R->bigBrother->littleBrother = R->littleBrother;
            R->littleBrother->bigBrother = R->bigBrother;
        }
        R->parent->numSpawning++;
    }

    preOrderTraversalOnKT(R, destroyNodeKTslave, &option);
    
    return true;
}

KTN_t *insertElementOnKT(KTN_t *R, int K, int keyValue, void *element) {
    // Block illegal parameters.
    if (element == NULL) return NULL;
    
    KTN_t *node = createNodeKT(keyValue, K, element);
    if (node == NULL) return NULL;
    if (R == NULL) return node;
    levelOrderTraversalOnKT(R, insertElementOnKTslave, node);
    
    return R;
}

bool deleteElementOnKT(KTN_t *R, int keyValue) {
    // Block illegal parameters.
    if (R == NULL) return false;
    
    //    Consider a subtree rooted at the node to be deleted.
    //    Replace the deletion node with the leftmost leaf of the subtree.
    KTN_t *target = findNodeOnKT(R, keyValue, KT_OPTION_BREADTH_FIRST_SEARCH);
    if (target == NULL) return false;
    KTN_t *leftmost = findLeftmostLeefNodeOnKT(target);
    
    // change a value and element of target node. then delete a leftmost node.
    target->keyValue = leftmost->keyValue;
    free(target->element);
    target->element = leftmost->element;
    if ((leftmost->parent != NULL) &&
        (leftmost->parent->child == leftmost)) {
        leftmost->parent->child = leftmost->littleBrother;
    }
    destroyKT(leftmost, KT_OPTION_NONE);
    
    return true;
}

void *findElementOnKT(KTN_t *R, int keyValue, KT_OPTION_e option) {
    KTN_t *findNode = findNodeOnKT(R, keyValue, option);
    if (findNode == NULL) return NULL;
    return findNode->element;
}

void *levelOrderTraversalOnKT(KTN_t *R, void *(*func)(KTN_t*, void*), void *parameter) {
    // Block illegal paramaters.
    if (R == NULL) return false;
    
    QUEUE_t *Q = createQueue();
    enQueue(Q, R);
    while (true) {
        KTN_t *node = deQueue(Q);
        if (node == NULL) break;
        
        void *p = func(node, parameter);
        if (p != NULL) {
            destroyQueue(Q, QUEUE_OPTION_NONE);
            return p;
        }
        KTN_t *child = node->child;
        while (child != NULL) {
            enQueue(Q, child);
            child = child->littleBrother;
        }
    }
    destroyQueue(Q, QUEUE_OPTION_NONE);
    return NULL;
}

void *preOrderTraversalOnKT(KTN_t *R, void *(*func)(KTN_t*, void*), void *parameter) {
    if (R == NULL) return NULL;
    
    void *p = NULL;
    p = func(R, parameter);
    if (p != NULL) return p;
    KTN_t *child = R->child;
    while (child != NULL) {
        p = preOrderTraversalOnKT(child, func, parameter);
        if (p != NULL) return p;
        child = child->littleBrother;
    }
    
    return NULL;
}

void *postOrderTraversalOnKT(KTN_t *R, void *(*func)(KTN_t*, void*), void *parameter) {
    if (R == NULL) return NULL;
    
    void *p = NULL;
    KTN_t *child = R->child;
    while (child != NULL) {
        p = postOrderTraversalOnKT(child, func, parameter);
        if (p != NULL) return p;
        child = child->littleBrother;
    }
    p = func(R, parameter);
    if (p != NULL) return p;
    
    return NULL;
}

void *getElementOnKT(KTN_t *R) {
    // Block illegal parameters.
    if (R == NULL) return NULL;
    
    return R->element;
}

//////////////////////////////////////////////////
//  private
KTN_t *createNodeKT(int keyValue, int K, void *element) {
    // Block illegal parameters.
    if (element == NULL) return NULL;
    
    KTN_t *node = malloc(sizeof(KTN_t));
    if (node == NULL) return NULL;
    node->keyValue = keyValue;
    node->element = element;
    node->numSpawning = K;
    node->parent = NULL;
    node->bigBrother = NULL;
    node->littleBrother = NULL;
    node->child = NULL;
    
    return node;
}

KTN_t *findNodeOnKT(KTN_t *R, int keyValue, KT_OPTION_e option) {
    KTN_t *node = NULL;
    switch (option) {
        case KT_OPTION_BREADTH_FIRST_SEARCH:
            node = levelOrderTraversalOnKT(R, findNodeOnKTslave, &keyValue);
            if (node != NULL) return node;
            break;
        case KT_OPTION_DEPTH_FIRST_SEARCH:
            node = preOrderTraversalOnKT(R, findNodeOnKTslave, &keyValue);
            if (node != NULL) return node;
            break;
        default:
            break;
    }
    return NULL;
}

KTN_t *findLeftmostLeefNodeOnKT(KTN_t *R) {
    KTN_t *parent = NULL;
    KTN_t *leftmost = R;
    while (true) {
        if (leftmost->child != NULL) {
            parent = leftmost;
            leftmost = leftmost->child;
            continue;
        }
        break;
    }
    return leftmost;
}

void *insertElementOnKTslave(KTN_t *R, void *parameter) {
    if (R == NULL) return NULL;
    if (R->numSpawning <= 0) return NULL;
    
    KTN_t *parent = R;
    KTN_t *child = parameter;
    KTN_t *son = parent->child;
    if (son == NULL) {
        child->parent = parent;
        parent->child = child;
    }
    else {
        KTN_t *lastBrother = son;
        while (son != NULL) {
            lastBrother = son;
            son = son->littleBrother;
        }
        child->parent = parent;
        child->bigBrother = lastBrother;
        lastBrother->littleBrother = child;
    }
    parent->numSpawning--;
    
    return child;
}

void *destroyNodeKTslave(KTN_t *R, void *parameter) {
    // Block illegal parameters.
    if (R == NULL) return NULL;
    
    int option = *((int *)parameter);
    if ((option == KT_OPTION_WITH_ELEMENT) &&
        (R->element != NULL)) {
        free(R->element);
    }
    free(R);
    
    return NULL;
}

void *findNodeOnKTslave(KTN_t *R, void *parameter) {
    // Block illegal parameters.
    if (R == NULL) return NULL;
    
    if (R->keyValue == *((int*)parameter)) return R;
    return NULL;
}

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include "Queue.h"
#include "KaryTree.h"

//////////////////////////////////////////////////
KTN_t *createNode(int keyValue, void *element) {
    // Block illegal parameters.
    if (element == NULL) return NULL;
    
    KTN_t *node = malloc(sizeof(KTN_t));
    if (node == NULL) return NULL;
    node->keyValue = keyValue;
    node->element = element;
    node->numOfChild = 0;
    node->parent = NULL;
    node->bigBrother = NULL;
    node->littleBrother = NULL;
    node->child = NULL;
    
    return node;
}

bool destroyKT(KTN_t *R, KT_OPTION_e option) {
    // Block illegal parameters
    if (R == NULL) return false;
    
    // post-order traversal.
    destroyKT(R->child, option);
    destroyKT(R->littleBrother, option);
    if ((option == KT_OPTION_WITH_ELEMENT) &&
        (R->element != NULL)) {
        free(R->element);
    }
    free(R);
    
    return true;
}

KTN_t *insertElementIntoKT(KTN_t *R, int K, int keyValue, void *element) {
    KTN_t *target = createNode(keyValue, element);
    if (target == NULL) return NULL;
    if (R == NULL) return target;
    
    //　level-order traversal.
    QUEUE_t *Q = createQueue();
    enQueue(Q, R);
    while (!isEmptyQueue(Q)) {
        KTN_t *parent = deQueue(Q);
        
        if (parent->numOfChild >= K) {
            // enqueue all brothers.
            KTN_t *littleBrother = parent->child;
            while (littleBrother != NULL) {
                enQueue(Q, littleBrother);
                littleBrother = littleBrother->littleBrother;
            }
        }
        else {
            // search empty child. then register a node.
            KTN_t *bigBrother = NULL;
            KTN_t *littleBrother = parent->child;
            while (true) {
                if (littleBrother == NULL) break;
                enQueue(Q, littleBrother);
                bigBrother = littleBrother;
                littleBrother = bigBrother->littleBrother;
            }
            parent->numOfChild++;
            target->parent = parent;
            if (bigBrother == NULL) {
                parent->child = target;
            }
            else {
                bigBrother->littleBrother = target;
                target->bigBrother = bigBrother;
            }
            break;
        }
    }
    destroyQueue(Q, QUEUE_OPTION_NONE);
    
    return R;
}

bool deleteElementOnKT(KTN_t *R, int (*comp)(void*, void*), void *element) {
    // Block illegal parameters.
    if (R == NULL) return false;
    
    //    Consider a subtree rooted at the node to be deleted.
    //    Replace the deletion node with the leftmost leaf of the subtree.
    KTN_t *target = findNodeOnKT(R, comp, element, KT_OPTION_BREADTH_FIRST_SEARCH);
    if (target == NULL) return false;
    
    // find leftmost-leaf
    KTN_t *parent = NULL;
    KTN_t *leftmost = target;
    while (true) {
        if (leftmost->child != NULL) {
            parent = leftmost;
            leftmost = leftmost->child;
            continue;
        }
        break;
    }
    
    // change a value of target node. then delete a leftmost node.
    target->keyValue = leftmost->keyValue;
    if (leftmost->parent != NULL) {
        leftmost->parent->numOfChild--;
        if (leftmost == target) {
            if (target->bigBrother == NULL) {
                target->parent->child = target->littleBrother;
                if (target->littleBrother != NULL) {
                    target->littleBrother->bigBrother = NULL;
                }
            }
            else {
                target->bigBrother->littleBrother = target->littleBrother;
                if (target->littleBrother != NULL) {
                    target->littleBrother->bigBrother = target->bigBrother;
                }
            }
        }
        else {
            leftmost->parent->child = leftmost->littleBrother;
            if (leftmost->littleBrother != NULL) {
                leftmost->littleBrother->bigBrother = NULL;
            }
        }
    }
    leftmost->littleBrother = NULL; // for destroy.
    destroyKT(leftmost, KT_OPTION_WITH_ELEMENT);
    
    return true;
}

int findElementOnKT(KTN_t *R, int (*comp)(void*, void*), void *element, KT_OPTION_e option) {
    KTN_t *findNode = findNodeOnKT(R, comp, element, option);
    if (findNode == NULL) return -1;
    return findNode->keyValue;
}

KTN_t *findNodeOnKT(KTN_t *R, int (*comp)(void*, void*), void *element, KT_OPTION_e option) {
    switch (option) {
        case KT_OPTION_BREADTH_FIRST_SEARCH:
            return breadthFirstFindNodeOnKT(R, comp, element);
        case KT_OPTION_DEPTH_FIRST_SEARCH:
            return depthFirstFindNodeOnKT(R, comp, element);
        default:
            break;
    }
    return NULL;
}

int breadthFirstFindElementOnKT(KTN_t *R, int (*comp)(void*, void*), void *element) {
    KTN_t *findNode = breadthFirstFindNodeOnKT(R, comp, element);
    if (findNode == NULL) return -1;
    return findNode->keyValue;
}

KTN_t *breadthFirstFindNodeOnKT(KTN_t *R, int (*comp)(void*, void*), void *element) {
    // Block illegal parameters.
    if (R == NULL) return NULL;
    //　level-order traversal.
    KTN_t *findNode = NULL;
    QUEUE_t *Q = createQueue();
    enQueue(Q, R);
    while (!isEmptyQueue(Q)) {
        KTN_t * node = deQueue(Q);
        if (comp(node, element) == 0) {
            findNode = node;
            break;
        }
        
        // enqueue all childes.
        KTN_t *child = node->child;
        while (child != NULL) {
            enQueue(Q, child);
            child = child->littleBrother;
        }
    }
    destroyQueue(Q, QUEUE_OPTION_NONE);
    return findNode;
}
int depthFirstFindElementOnKT(KTN_t *R, int (*comp)(void*, void*), void *element) {
    KTN_t *findNode = depthFirstFindNodeOnKT(R, comp, element);
    if (findNode == NULL) return -1;
    return findNode->keyValue;
}

KTN_t *depthFirstFindNodeOnKT(KTN_t *R, int (*comp)(void*, void*), void *element) {
    // Block illegal parameters.
    if (R == NULL) return NULL;
    
    if (comp(R->element, element) == 0) return R;
    KTN_t *child = R->child;
    while (child != NULL) {
        KTN_t *node = depthFirstFindNodeOnKT(child, comp, element);
        if (node != NULL) return node;
        child = child->littleBrother;
    }
    return NULL;
}

void levelOrderTraversalOnKT(KTN_t *R) {
    QUEUE_t *Q = createQueue();
    enQueue(Q, R);
    while (!isEmptyQueue(Q)) {
        KTN_t * node = deQueue(Q);
        printf("level-order traversal : %d\n", node->keyValue);
        
        // enqueue all childs.
        KTN_t *child = node->child;
        while (child != NULL) {
            enQueue(Q, child);
            child = child->littleBrother;
        }
    }
    destroyQueue(Q, QUEUE_OPTION_WITH_ELEMENT);
}

void preOrderTraversalOnKT(KTN_t *R) {
    if (R == NULL) return;
    
    printf("pre-order traversal : %d\n", R->keyValue);
    KTN_t *child = R->child;
    while (child != NULL) {
        preOrderTraversalOnKT(child);
        child = child->littleBrother;
    }
}

void postOrderTraversalOnKT(KTN_t *R) {
    if (R == NULL) return;
    
    KTN_t *child = R->child;
    while (child != NULL) {
        postOrderTraversalOnKT(child);
        child = child->littleBrother;
    }
    printf("post-order traversal : %d\n", R->keyValue);
}

#ifndef KaryTree_h
#define KaryTree_h

#include <stdbool.h>

//////////////////////////////////////////////////
typedef enum KTOption {
    KT_OPTION_NONE,
    KT_OPTION_WITH_ELEMENT,
    KT_OPTION_BREADTH_FIRST_SEARCH = 10,
    KT_OPTION_DEPTH_FIRST_SEARCH
} KT_OPTION_e;

typedef struct KaryTree_Node {
    int keyValue;
    void *element;
    int numOfChild;
    struct KaryTree_Node *parent;       // for Breadth-First Find
    struct KaryTree_Node *bigBrother;
    struct KaryTree_Node *littleBrother;
    struct KaryTree_Node *child;
} KTN_t;

//////////////////////////////////////////////////
KTN_t *createNode(int value, void *element);
bool destroyKT(KTN_t *R, KT_OPTION_e option);
KTN_t *insertElementIntoKT(KTN_t *R, int K, int keyValue, void *element);
bool deleteElementOnKT(KTN_t *R, int (*comp)(void*, void*), void *element);
int findElementOnKT(KTN_t *R, int (*comp)(void*, void*), void *element, KT_OPTION_e option);
KTN_t *findNodeOnKT(KTN_t *R, int (*comp)(void*, void*), void *element, KT_OPTION_e option);
int breadthFirstFindElementOnKT(KTN_t *R, int (*comp)(void*, void*), void *element);
KTN_t *breadthFirstFindNodeOnKT(KTN_t *R, int (*comp)(void*, void*), void *element);
int depthFirstFindElementOnKT(KTN_t *R, int (*comp)(void*, void*), void *element);
KTN_t *depthFirstFindNodeOnKT(KTN_t *R, int (*comp)(void*, void*), void *element);
void levelOrderTraversalOnKT(KTN_t *R);
void preOrderTraversalOnKT(KTN_t *R);
void postOrderTraversalOnKT(KTN_t *R);

#endif

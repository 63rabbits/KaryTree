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
    int numSpawning;
    struct KaryTree_Node *parent;       // for Breadth-First Find
    struct KaryTree_Node *bigBrother;
    struct KaryTree_Node *littleBrother;
    struct KaryTree_Node *child;
} KTN_t;

//////////////////////////////////////////////////
bool destroyKT(KTN_t *R, KT_OPTION_e option);
KTN_t *insertElementOnKT(KTN_t *R, int K, int keyValue, void *element);
bool deleteElementOnKT(KTN_t *R, int keyValue);
void *findElementOnKT(KTN_t *R, int keyValue, KT_OPTION_e option);
void *levelOrderTraversalOnKT(KTN_t *R, void *(*func)(KTN_t*, void*), void *parameter);
void *preOrderTraversalOnKT(KTN_t *R, void *(*func)(KTN_t*, void*), void *parameter);
void *postOrderTraversalOnKT(KTN_t *R, void *(*func)(KTN_t*, void*), void *parameter);
void *getElementOnKT(KTN_t *R);

#endif

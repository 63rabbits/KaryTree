#include <unistd.h>
//#define checkMemoryLeak

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include "KaryTree.h"

//////////////////////////////////////////////////
typedef struct Element {
    int value;
} Element_t;

//////////////////////////////////////////////////
void test(void);
Element_t *createElement(int value);
bool destroyElement(Element_t *element);
void *levelOrderTraversalOnKTslave(KTN_t *R, void *parameter);
void *preOrderTraversalOnKTslave(KTN_t *R, void *parameter);
void *postOrderTraversalOnKTslave(KTN_t *R, void *parameter);

//////////////////////////////////////////////////
int main(int argc, const char * argv[]) {
    
#ifdef checkMemoryLeak
    while (true) {
        test();
        sleep(1);
    }
#else
    test();
#endif
    
    return EXIT_SUCCESS;
}

void test() {
    int K = 3;
    int numOfNode = 10;
    int findKey = 5;
    int deleteKey = 2;
    
    Element_t *element = NULL;
    KTN_t *root = NULL;
    for (int i=0; i<numOfNode; i++) {
        element = createElement(i);
        root = insertElementOnKT(root, K, element->value, element);
        if (root == NULL) {
            printf("error [%s] : could not insert %d.\n", __func__, i);
            return;
        }
    }
    
    printf("*** level-order traversal ***\n");
    levelOrderTraversalOnKT(root, levelOrderTraversalOnKTslave, NULL);
    
    printf("*** pre-order traversal ***\n");
    preOrderTraversalOnKT(root, preOrderTraversalOnKTslave, NULL);
    
    printf("*** post-order traversal ***\n");
    postOrderTraversalOnKT(root, postOrderTraversalOnKTslave, NULL);
    
    printf("*** breadth first find ***\n");
    Element_t *element1 = findElementOnKT(root, findKey, KT_OPTION_BREADTH_FIRST_SEARCH);
    printf("breadth first find keyValue %d [%s] : found value = %d\n", findKey, __func__, element1->value);
    
    printf("*** depth first find ***\n");
    Element_t *element2 = findElementOnKT(root, findKey, KT_OPTION_DEPTH_FIRST_SEARCH);
    printf("depth first find keyValue %d [%s] : found value = %d\n", findKey, __func__, element2->value);
    
    printf("*** delete ***\n");
    bool check = deleteElementOnKT(root, deleteKey);
    if (check) {
        printf("delete keyValue %d [%s] : success.\n", deleteKey, __func__);
    }
    else {
        printf("error [%s] : could not delete keyValue %d.\n", __func__, deleteKey);
    }
    
    printf("*** level-order traversal ***\n");
    levelOrderTraversalOnKT(root, levelOrderTraversalOnKTslave, NULL);
    
    destroyKT(root, KT_OPTION_WITH_ELEMENT);
}

Element_t *createElement(int value) {
    Element_t *element = malloc(sizeof(Element_t));
    if (element == NULL) return NULL;
    element->value = value;
    return element;
}

bool destroyElement(Element_t *element) {
    // Block illegal parameters.
    if (element == NULL) return false;
    
    free(element);
    return true;
}

void *levelOrderTraversalOnKTslave(KTN_t *R, void *parameter) {
    Element_t *element = getElementOnKT(R);
    
    printf("level-order traversal : %d\n", element->value);
    return NULL;    // none stop.
}

void *preOrderTraversalOnKTslave(KTN_t *R, void *parameter) {
    Element_t *element = getElementOnKT(R);
    
    printf("pre-order traversal : %d\n", element->value);
    return NULL;    // none stop.
}

void *postOrderTraversalOnKTslave(KTN_t *R, void *parameter) {
    Element_t *element = getElementOnKT(R);
    
    printf("post-order traversal : %d\n", element->value);
    return NULL;    // none stop.
}


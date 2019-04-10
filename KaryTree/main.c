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
int comp(void *elementA, void *elementB);

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
    int findValue = 5;
    int deleteValue = 2;
    
    Element_t *element = NULL;
    KTN_t *root = NULL;
    for (int i=0; i<numOfNode; i++) {
        element = createElement(i);
        root = insertElementIntoKT(root, K, element->value, element);
        if (root == NULL) {
            printf("error [%s] : could not insert %d.\n", __func__, i);
            return;
        }
    }
    
    printf("*** level-order traversal ***\n");
    levelOrderTraversalOnKT(root);

    printf("*** pre-order traversal ***\n");
    preOrderTraversalOnKT(root);

    printf("*** post-order traversal ***\n");
    postOrderTraversalOnKT(root);

    printf("*** breadth first find ***\n");
    element = createElement(findValue);
    int value = findElementOnKT(root, comp, element, KT_OPTION_BREADTH_FIRST_SEARCH);
    printf("breadth first find %d [%s] : found value = %d\n", element->value, __func__, value);
    destroyElement(element);

    printf("*** depth first find ***\n");
    element = createElement(findValue);
    value = findElementOnKT(root, comp, element, KT_OPTION_DEPTH_FIRST_SEARCH);
    printf("depth first find %d [%s] : found value = %d\n", element->value, __func__, value);
    destroyElement(element);

    printf("*** delete ***\n");
    element = createElement(deleteValue);
    bool check = deleteElementOnKT(root, comp, element);
    if (check) {
        printf("delete value %d [%s] : success.\n", deleteValue, __func__);
    }
    else {
        printf("error [%s] : could not delete value %d.\n", __func__, deleteValue);
    }
    destroyElement(element);

    printf("*** level-order traversal ***\n");
    levelOrderTraversalOnKT(root);
    
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

int comp(void *elementA, void *elementB) {
    int valueA = ((Element_t *)elementA)->value;
    int valueB = ((Element_t *)elementB)->value;
    return valueA - valueB;
}

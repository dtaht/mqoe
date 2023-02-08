#include <stdio.h>
#include <string.h>
 
#include <avltree.h>
 
int main(void)
{
    avltree * tree;
    const char * result;
    unsigned int e;
    char * elements[] = {"orange", "apple", "pear", "grapefruit", "cherry", "plum"};
    const unsigned int n = sizeof(elements) / sizeof(const char*);
 
    tree = avltree_create((avltree_cmpfn)strcmp);
    for (e = 0; e < n; e++) {
        avltree_add(tree, elements[e]);
    }
    avltree_for_each(tree, (avltree_forfn)puts);
    for (e = 0; e < n; e++) {
        result = avltree_find(tree, elements[e]);
        if (result) {
            printf("Found: %s\n", result); 
        }
        else {
            printf("Couldn't find %s\n", elements[e]);
        }
    }
    for (e = 0; e < n; e++) {
        result = avltree_remove(tree, elements[e]);
        if (result) {
            printf("Removed: %s\n", result);
        }
        else {
            printf("Couldn't remove %s\n", elements[e]);
        }
    }
    avltree_delete(tree);
 
    return 0;
}

#ifndef AVLTREE_H
#define AVLTREE_H
 
#include <stdlib.h>
 
struct avltreenode
{
    struct avltreenode * left;
    struct avltreenode * right;
    struct avltreenode * parent;
    unsigned int leftheight;
    unsigned int rightheight;
    void * data;
};
 
typedef struct avltreenode avltreenode;
 
typedef int (*avltree_cmpfn)(const void*, const void*);
typedef void (*avltree_forfn)(void*);
 
struct avltree {
    avltreenode * root;
    size_t count;
    avltree_cmpfn compare;
};
 
typedef struct avltree avltree;
 
avltree * avltree_create(avltree_cmpfn compare);
void avltree_delete(avltree * tree);
void avltree_for_each(const avltree * tree, avltree_forfn fun);
void* avltree_add(avltree * tree, void * data);
void* avltree_find(const avltree * tree, const void* data);
void* avltree_remove(avltree * tree, const void* data);
void avltree_empty(avltree * tree);
size_t avltree_get_count(const avltree *tree);
 
#endif /* AVLTREE_H */


#include <stdlib.h>
#include <avltree.h>
 
avltree * avltree_create(avltree_cmpfn compare)
{
    avltree * tree = malloc(sizeof(avltree));
    if (tree != NULL) {
        tree->root = NULL;
        tree->compare = compare;
        tree->count = 0;
    }
    return tree;
}
 
static void avltreenode_delete(avltreenode *node)
{
    free(node);
}
 
static void avltree_empty_recursive(avltreenode * root)
{
    if (root->left) {
        avltree_empty_recursive(root->left);
    }
    if (root->right) {
        avltree_empty_recursive(root->right);
    }
    avltreenode_delete(root);
}
 
void avltree_empty(avltree * tree)
{
    if (tree->root) {
        avltree_empty_recursive(tree->root);
        tree->root = NULL;
        tree->count = 0;
    }
}
 
void avltree_delete(avltree * tree)
{
    if (tree) {
        avltree_empty(tree);
        free(tree);
    }
}
 
static void avltree_for_each_recursive(const avltreenode * root, avltree_forfn fun)
{
    if (root->left != NULL) {
        avltree_for_each_recursive(root->left, fun);
    }
    fun(root->data);
    if (root->right != NULL) {
        avltree_for_each_recursive(root->right, fun);
    }
}
 
void avltree_for_each(const avltree * tree, avltree_forfn fun)
{
    if (tree->root) {
        avltree_for_each_recursive(tree->root, fun);
    }
}
 
struct avlsearchresult
{
    avltreenode *node;
    avltreenode *parent;
};
typedef struct avlsearchresult avlsearchresult;
 
static int avltree_search(const avltree *tree, avlsearchresult *result, const void *data)
{
    int found = 0;
 
    result->node = tree->root;
    while (!found && result->node != NULL) {
        int rv = tree->compare(result->node->data, data);
        if (rv == 0) {
            found = 1;
        }
        else {
            result->parent = result->node;
            if (rv > 0) {
                result->node = result->node->left;
            }
            else if (rv < 0) {
                result->node = result->node->right;
            }
        }
    }
    return found;
}
 
static avltreenode * avltreenode_create(void * data)
{
    avltreenode * node = malloc(sizeof(avltreenode));
    if (node) {
        node->left = NULL;
        node->right = NULL;
        node->parent = NULL;
        node->leftheight = 0;
        node->rightheight = 0;
        node->data = data;
    }
    return node;
}
 
static int avltreenode_get_max_height(const avltreenode *node)
{
    int height;
    if (node->leftheight > node->rightheight) {
        height = node->leftheight;
    }
    else {
        height = node->rightheight;
    }
    return height;
}
 
static void avltreenode_fix_height(avltreenode *node)
{
    node->leftheight = 0;
    node->rightheight = 0;
    if (node->left) {
        node->leftheight = avltreenode_get_max_height(node->left) + 1;
    }
    if (node->right) {
        node->rightheight = avltreenode_get_max_height(node->right) + 1;
    }
}
 
static void avltree_rotate_left(avltree *tree, avltreenode *node)
{
    avltreenode *right = node->right;
    if (node == tree->root) {
        tree->root = right;
    }
    else if (node == node->parent->left) {
        node->parent->left = right;
    }
    else {
        node->parent->right = right;
    }
    right->parent = node->parent;
    if (right->left) {
        node->right = right->left;
        node->right->parent = node;
    }
    else {
        node->right = NULL;
    }
    right->left = node;
    node->parent = right;
    avltreenode_fix_height(node);
    avltreenode_fix_height(right);
}
 
static void avltree_rotate_right(avltree *tree, avltreenode *node)
{
    avltreenode *left = node->left;
    if (node == tree->root) {
        tree->root = left;
    }
    else if (node == node->parent->left) {
        node->parent->left = left;
    }
    else {
        node->parent->right = left;
    }
    left->parent = node->parent;
    if (left->right) {
        node->left = left->right;
        node->left->parent = node;
    }
    else {
        node->left = NULL;
    }
    left->right = node;
    node->parent = left;
    avltreenode_fix_height(node);
    avltreenode_fix_height(left);
}
 
static int avltreenode_get_balance_factor(const avltreenode *node)
{
    return node->leftheight - node->rightheight;
}
 
static void avltree_rebalance(avltree *tree, avltreenode *node)
{
    avltreenode *current = node;
    while (current != NULL) {
        avltreenode *parent = current->parent;
        int balance;
        avltreenode_fix_height(current);
        balance = avltreenode_get_balance_factor(current);
        if (balance == -2) {
            /* Right heavy */
            const int rightbalance = avltreenode_get_balance_factor(current->right);
            if (rightbalance < 0) {
                avltree_rotate_left(tree, current);
            }
            else {
                avltree_rotate_right(tree, current->right);
                avltree_rotate_left(tree, current);
            }
        }
        else if (balance == 2) {
            /* Left heavy */
            const int leftbalance = avltreenode_get_balance_factor(current->left);
            if (leftbalance > 0) {
                avltree_rotate_right(tree, current);
            }
            else {
                avltree_rotate_left(tree, current->left);
                avltree_rotate_right(tree, current);
            }
        }
        current = parent;
    }
}
 
void* avltree_add(avltree * tree, void * data)
{
    void *temp = NULL;
    avlsearchresult result;
    result.node = NULL;
    result.parent = NULL;
 
    if (avltree_search(tree, &result, data)) {
        temp = result.node->data;
        result.node->data = data;
    }
    else {
        avltreenode *node = avltreenode_create(data);
        if (result.node == tree->root) {
            tree->root = node;
        }
        else {
            int rv = tree->compare(data, result.parent->data);
            if (rv < 0) {
                result.parent->left = node;
            }
            else {
                result.parent->right = node;
            }
            node->parent = result.parent;
            avltree_rebalance(tree, node);
        }
        tree->count++;
    }
     
    return temp;
}
 
void* avltree_find(const avltree * tree, const void* data)
{
    void *temp = NULL;
    avlsearchresult result;
    result.node = NULL;
    result.parent = NULL;
 
    if (avltree_search(tree, &result, data)) {
        temp = result.node->data;
    }
    return temp;
}
 
static avltreenode *avltreenode_find_min(avltreenode *node)
{
    avltreenode *current = node;
 
    while (current->left) {
        current = current->left;
    }
    return current;
}
 
static void avltree_remove_node(avltree *tree, avltreenode *node)
{
    if (node->left && node->right) {
        /* Node with 2 children */
        avltreenode *successor = avltreenode_find_min(node->right);
        node->data = successor->data;
        avltree_remove_node(tree, successor);
    }
    else {
        avltreenode *parent = node->parent;
        if (node->left) {
            /* Node with only left child */
            if (node->parent) {
                if (node == node->parent->left) {
                    node->parent->left = node->left;
                    node->parent->left->parent = node->parent;
                }
                else {
                    node->parent->right = node->left;
                    node->parent->right->parent = node->parent;
                }
            }
            else {
                tree->root = node->left;
                tree->root->parent = NULL;
            }
        }
        else if (node->right) {
            /* Node with only right child */
            if (node->parent) {
                if (node == node->parent->left) {
                    node->parent->left = node->right;
                    node->parent->left->parent = node->parent;
                }
                else {
                    node->parent->right = node->right;
                    node->parent->right->parent = node->parent;
                }
            }
            else {
                tree->root = node->right;
                tree->root->parent = NULL;
            }
        }
        else {
            /* Node with no children */
            if (node->parent) {
                if (node == node->parent->left) {
                    node->parent->left = NULL;
                }
                else {
                    node->parent->right = NULL;
                }
            }
            else {
                tree->root = NULL;
            }
        }
        avltreenode_delete(node);
        avltree_rebalance(tree, parent);
        tree->count--;
    }
}
 
void* avltree_remove(avltree * tree, const void* data)
{
    void *temp = NULL;
    avlsearchresult result;
    result.node = NULL;
    result.parent = NULL;
 
    if (avltree_search(tree, &result, data)) {
        temp = result.node->data;
        avltree_remove_node(tree, result.node);
    }
    return temp;
}
 
size_t avltree_get_count(const avltree *tree)
{
    return tree->count;
}

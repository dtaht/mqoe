#include <stdio.h>
#include <binpack.h>

void *avltree_first_fit(const avltree *tree, const void *data)
{
    void *found = NULL;
 
    avltreenode *node = tree->root;
    while (!found && node != NULL) {
        int rv = tree->compare(node->data, data);
        if (rv >= 0) {
            found = node->data;
        }
        else {
            node = node->right;
        }
    }
    return found;
}

bin *find_first_bin(const avltree *tree, unsigned int size)
{
    bin b = {0, size};
    return avltree_first_fit(tree, &b);
}
 
unsigned int first_fit(unsigned int binsize, unsigned int *sizes, unsigned int *bins,
        unsigned int numitems)
{
    unsigned int bins_used = 0;
    unsigned int item;
    avltree *tree = avltree_create((avltree_cmpfn)bin_compare);
    if (tree == NULL) {
        return 0;
    }
    for (item = 0; item < numitems; item++) {
        bin *b = find_first_bin(tree, sizes[item]);
        if (b) {
            /* Add to this bin */
            avltree_remove(tree, b);
            bins[item] = b->id;
            bin_use(b, sizes[item]);
            avltree_add(tree, b);
        }
        else {
            /* Create a new bin and add to it */
            b = bin_create(bins_used, binsize);
            bins[item] = bins_used;
            bin_use(b, sizes[item]);
            avltree_add(tree, b);
            bins_used++;
        }
    }
    avltree_for_each(tree, (avltree_forfn)bin_delete);
    avltree_delete(tree);
    return bins_used;
}

void *avltree_best_fit(const avltree *tree, const void *data)
{
    void *best = NULL;
    unsigned int found = 0;
    avltreenode *node = tree->root;
    while (node != NULL && !found) {
        int result = tree->compare(node->data, data);
        if (result > 0) {
            if (best == NULL || tree->compare(node->data, best) < 0) {
                best = node->data;
            }
            node = node->left;
        }
        else if (result < 0) {
            node = node->right;
        }
        else {
            best = node->data;
            found = 1;
        }
    }
    return best;
}

bin *find_best_bin(const avltree *tree, unsigned int size)
{
    bin b = {0, size};
    return avltree_best_fit(tree, &b);
}
 
unsigned int best_fit(unsigned int binsize, unsigned int *sizes, unsigned int *bins,
        unsigned int numitems)
{
    unsigned int bins_used = 0;
    unsigned int item;
    avltree *tree = avltree_create((avltree_cmpfn)bin_compare);
    if (tree == NULL) {
        return 0;
    }
    for (item = 0; item < numitems; item++) {
        bin *b = find_best_bin(tree, sizes[item]);
        if (b) {
            /* Add to this bin */
            avltree_remove(tree, b);
            bins[item] = b->id;
            bin_use(b, sizes[item]);
            avltree_add(tree, b);
        }
        else {
            /* Create a new bin and add to it */
            b = bin_create(bins_used, binsize);
            bins[item] = b->id;
            bin_use(b, sizes[item]);
            avltree_add(tree, b);
            bins_used++;
        }
    }
    avltree_for_each(tree, (avltree_forfn)bin_delete);
    avltree_delete(tree);
    return bins_used;
}

unsigned int worst_fit(unsigned int binsize, unsigned int *sizes, unsigned int *bins,
        unsigned int numitems)
{
    unsigned int bins_used = 0;
    unsigned int item;
    maxheap *heap = maxheap_create();
    if (heap == NULL) {
        return 0;
    }
    for (item = 0; item < numitems; item++) {
        bin *b = maxheap_remove_max(heap);
        if (b && b->capacity < sizes[item]) {
            /* Too small; put b back */
            maxheap_add(heap, b, b->capacity);
            b = NULL;
        }
        if (!b) {
            /* Create a new bin */
            b = bin_create(bins_used, binsize);
            bins_used++;
        }
        bins[item] = b->id;
        bin_use(b, sizes[item]);
        maxheap_add(heap, b, b->capacity);
    }
    maxheap_for_each(heap, (maxheap_forfn)bin_delete);
    maxheap_delete(heap);
    return bins_used;
}

int compare_uints_decreasing(const void *v1, const void *v2)
{
    const unsigned int *p1 = v1;
    const unsigned int *p2 = v2;
    if (*p1 > *p2) {
        return -1;
    }
    if (*p1 < *p2) {
        return 1;
    }
    return 0;
}
 
void sort_uints_decreasing(unsigned int *sizes, unsigned int numitems)
{
    qsort(sizes, numitems, sizeof(unsigned int), compare_uints_decreasing);
}

unsigned int first_fit_decreasing(unsigned int binsize, unsigned int *sizes, unsigned int *bins,
        unsigned int numitems)
{
    sort_uints_decreasing(sizes, numitems);
    return first_fit(binsize, sizes, bins, numitems);
}

unsigned int best_fit_decreasing(unsigned int binsize, unsigned int *sizes, unsigned int *bins,
        unsigned int numitems)
{
    sort_uints_decreasing(sizes, numitems);
    return best_fit(binsize, sizes, bins, numitems);
}

unsigned int worst_fit_decreasing(unsigned int binsize, unsigned int *sizes, unsigned int *bins,
        unsigned int numitems)
{
    sort_uints_decreasing(sizes, numitems);
    return worst_fit(binsize, sizes, bins, numitems);
}

unsigned int next_fit(unsigned int binsize, unsigned int *sizes, unsigned int *bins,
        unsigned int numitems)
{
    unsigned int bin = 0;
    unsigned int capacity = binsize;
    unsigned int item;
    for (item = 0; item < numitems; item++) {
        if (sizes[item] > capacity) {
            /* Start a new bin */
            bin++;
            capacity = binsize;
        }
        /* Put item in bin */
        capacity -= sizes[item];
        bins[item] = bin;
    }
    return bin + 1;
}
 
int main(void)
{
    unsigned int sizes[] = {1, 4, 9, 4, 1, 5, 8, 3, 2, 5, 7, 3, 2, 6};
    const unsigned int numitems = 14;
    unsigned int bins[numitems];
    const unsigned int binsize = 10;
    unsigned int item;
    unsigned int bins_used = fit(binsize, sizes, bins, numitems);
    printf("%d bins were used\n", bins_used);
    for (item = 0; item < numitems; item++) {
        printf("Item #%d (size %d) is in bin %d\n", item, sizes[item], bins[item]);
    }
    return 0;
}

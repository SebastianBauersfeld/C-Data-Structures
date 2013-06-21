/*
 * author: Sebastian Bauersfeld
 */
#include "binary_search_tree.h"

#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

typedef enum { RBT_RED, RBT_BLACK } rbt_color;

/* this set of functions is not complete, use the binary search tree
 * functions for additional functionality */
extern void rbt_insert(bst_tree* tree, int val);
extern void rbt_assert(bst_tree* tree);
extern int rbt_remove(bst_tree* tree, int val);
extern void rbt_test();


#endif RED_BLACK_TREE_H

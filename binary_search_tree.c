/*
 * author: Sebastian Bauersfeld
 */

#include "binary_search_tree.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>

/* construct a new binary search tree */
bst_tree* bst_create(){
    bst_tree* ret = (bst_tree*) malloc(sizeof(bst_tree));
    ret->root = 0;
    return ret;
}

/* construct a new binary tree node */
bst_node* bst_create_node(bst_node* parent, bst_node* left, bst_node* right, int val){
    bst_node* ret = (bst_node*) malloc(sizeof(bst_node));
    ret->parent = parent;
    ret->left = left;
    ret->right = right;
    ret->val = val;
    return ret;
}

void bst_node_insert(bst_node* node, int val){
    if(!node)
        return;
    if(val == node-> val)
        return;
    else if(val > node->val){
        if(node->right)
            bst_node_insert(node->right, val);
        else
            node->right = bst_create_node(node, 0, 0, val);
    }else{
        if(node->left)
            bst_node_insert(node->left, val);
        else
            node->left = bst_create_node(node, 0, 0, val);
    }
}

/* insert a value into a binary tree */
void bst_insert(bst_tree* tree, int val){
    if(tree->root)
        bst_node_insert(tree->root, val);
    else
        tree->root = bst_create_node(0, 0, 0, val);
}


void bst_print_node(bst_node* node, int indent){
    if(!node)
        return;
    for(int i = 0; i < indent; i++)
        printf(" ");
    printf("%d\n", node->val);
    bst_print_node(node->left, indent + 1);
    bst_print_node(node->right, indent + 1);
}

/* print readable presentation of tree to stdout */
void bst_print(bst_tree* tree){
    if(!tree->root)
        printf("Empty tree.\n");
    bst_print_node(tree->root, 0);
}

int bst_node_contains(bst_node* node, int val){
    if(!node)
        return 0;
    if(node->val == val)
        return 1;
    return bst_node_contains(node->left, val) || bst_node_contains(node->right, val);
}

/* searches for val and determines whether it is contained within the tree */
int bst_contains(bst_tree* tree, int val){
    return bst_node_contains(tree->root, val);
}

void bst_node_print_order(bst_node* node, bst_order print_order){
    if(!node)
        return;

    switch(print_order){
        case BST_INORDER: 
            bst_node_print_order(node->left, print_order);
            printf("%d  ", node->val);
            bst_node_print_order(node->right, print_order);
            break;
        case BST_PREORDER: 
            printf("%d  ", node->val);
            bst_node_print_order(node->left, print_order);
            bst_node_print_order(node->right, print_order);
            break;
        case BST_POSTORDER: 
            bst_node_print_order(node->left, print_order);
            bst_node_print_order(node->right, print_order);
            printf("%d  ", node->val);
            break;
    }
}

/* prints the elements of the tree either in pre-order, in-order or post-order */
void bst_print_order(bst_tree* tree, bst_order print_order){
    bst_node_print_order(tree->root, print_order);
    printf("\n");
}

void bst_node_assert(bst_node* node, int lower, int upper){
    if(!node) return;
    assert(node->val >= lower && node->val <= upper);
    bst_node_assert(node->left, lower, node->val - 1);
    bst_node_assert(node->right, node->val + 1, upper);
}

/* verifies the search property of a bst */
void bst_assert(bst_tree* tree){
    assert(tree);
    bst_node_assert(tree->root, INT_MIN, INT_MAX);
}

/* remove a node which has at max one child */
void bst_splice_node(bst_node* node){
    assert(node);
    assert(node->parent);
    assert(!(node->left && node->right)); /* nodes with two children can't be spliced! */
    
    bst_node* offspring = node->left ? node->left : node->right;

    if(node == node->parent->left) /* this node is the left child of its parent */
        node->parent->left = offspring;
    else /* it is the right child */
        node->parent->right = offspring;

    if(offspring)
        offspring->parent = node->parent;
    free(node);
}

/* free a bst node */
void bst_node_delete(bst_node* node){
    if(!node)
        return;
    bst_node_delete(node->left);
    bst_node_delete(node->right);
    free(node);
}

/* free a bst tree */
void bst_delete(bst_tree* tree){
    bst_node_delete(tree->root);
    free(tree);
}

/* a small test suite for the bst implementation */
int test_suite(){
    bst_tree* tree = bst_create();
    bst_print(tree);
    assert(!bst_contains(tree, 0));

    bst_insert(tree, 0);
    assert(bst_contains(tree, 0));

    bst_insert(tree, 1);
    bst_insert(tree, 2);
    bst_insert(tree, 100);
    bst_insert(tree, -100);
    assert(bst_contains(tree, -100));
    assert(bst_contains(tree, 1));
    bst_assert(tree);

    printf("in-order: ");
    bst_print_order(tree, BST_INORDER);

    printf("pre-order: ");
    bst_print_order(tree, BST_PREORDER);

    printf("post-order: ");
    bst_print_order(tree, BST_POSTORDER);

    bst_print(tree);

    bst_delete(tree);
    return 0;
}

int main(int argc, char** args){
    if(test_suite() < 0)
        printf("\n===\nTest suite failed!\n");
    else
        printf("\n===\nAll tests succeeded!\n");
    return 0;
}

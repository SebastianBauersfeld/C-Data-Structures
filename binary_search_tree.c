/*
 * author: Sebastian Bauersfeld
 */

#include "binary_search_tree.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>

/* prototypes */
bst_node* bst_find_node(bst_node* start, int val);


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

/* searches for val and determines whether it is contained within the tree */
int bst_contains(bst_tree* tree, int val){
    return bst_find_node(tree->root, val) ? 1 : 0;
}

void bst_node_print(bst_node* node, bst_order print_order){
    if(!node)
        return;

    switch(print_order){
        case BST_INORDER: 
            bst_node_print(node->left, print_order);
            printf("%d  ", node->val);
            bst_node_print(node->right, print_order);
            break;
        case BST_PREORDER: 
            printf("%d  ", node->val);
            bst_node_print(node->left, print_order);
            bst_node_print(node->right, print_order);
            break;
        case BST_POSTORDER: 
            bst_node_print(node->left, print_order);
            bst_node_print(node->right, print_order);
            printf("%d  ", node->val);
            break;
        default:
            assert(0);
    }
}

/* prints the elements of the tree either in pre-order, in-order or post-order */
void bst_print(bst_tree* tree, bst_order print_order){
    bst_node_print(tree->root, print_order);
    printf("\n");
}

void bst_node_assert(bst_node* node, int lower, int upper){
    if(!node) return;
    if(node->parent)
        assert(node->parent->left == node || node->parent->right == node);
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
bst_node* bst_splice_node(bst_node* node){
    assert(node);
    assert(!(node->left && node->right)); /* nodes with two children can't be spliced! */
    
    bst_node* child = node->left ? node->left : node->right;
    bst_node* father = node->parent;

    if(father){
        if(node == father->left) /* this node is the left child of its parent */
            father->left = child;
        else /* it is the right child */
            father->right = child;
    }

    if(child)
        child->parent = father;
    return child;
}

bst_node* bst_find_node(bst_node* start, int val){
    if(!start)
        return 0;
    if(val == start->val)
        return start;
    else if(val < start->val)
        return bst_find_node(start->left, val);
    else
        return bst_find_node(start->right, val);
}

/* returns the node with the min value which is or is below the given node */
bst_node* bst_node_min(bst_node* node){
    assert(node);
    if(node->left)
        return bst_node_min(node->left);
    else
        return node;
}

bst_node* bst_remove_node(bst_node* node){
    assert(node);
    if(!(node->left && node->right)){ /* node has at max one child, so we can splice it out */
        bst_node* ret = bst_splice_node(node);
        free(node);
        return ret;
    }

    /* node has two children, ergo we search the successor in the right sub-tree */
    bst_node* succ = bst_node_min(node->right); 

    /* the successor has no left child!! and at max a right child so we can splice it out... */
    bst_splice_node(succ);

    /* ...and plug it into the position of our deleted node */
    succ->left = node->left;
    succ->right = node->right;
    succ->parent = node->parent;

    if(succ->left)
        succ->left->parent = succ;
    if(succ->right)
        succ->right->parent = succ;
    if(succ->parent){
        if(succ->parent->left == succ)
            succ->parent->left = succ;
        else
            succ->parent->right = succ;
    }

    free(node);
    return succ;
}

/* searches for a node with the given value and deletes it if it is contained within the tree
 * returns 1 if it removed it or 0 in case a node with this value does not exist */
int bst_remove(bst_tree* tree, int val){
    bst_node* node = bst_find_node(tree->root, val);
    if(!node)
        return 0;

    int isroot = node == tree->root;
    bst_node* replaced = bst_remove_node(node);
    if(isroot)
        tree->root = replaced;

    return 1;
}

/* free a bst node */
void bst_node_free(bst_node* node){
    if(!node)
        return;
    bst_node_free(node->left);
    bst_node_free(node->right);
    free(node);
}

/* free a bst tree */
void bst_free(bst_tree* tree){
    bst_node_free(tree->root);
    free(tree);
}

/* a small test suite for the bst implementation */
void bst_test(){
    printf("testing binary search trees...\n");

    /* empty tree */
    bst_tree* tree = bst_create();
    assert(!bst_contains(tree, 0));
    assert(!bst_remove(tree, 0));

    /* remove single root */
    bst_insert(tree, 0);
    assert(bst_contains(tree, 0));
    assert(bst_remove(tree, 0));
    bst_assert(tree);
    assert(!bst_contains(tree, 0));
    bst_insert(tree, 0);
    assert(bst_contains(tree, 0));
    bst_assert(tree);

    /* insert some stuff and check the tree's search property */
    bst_insert(tree, 1);
    bst_insert(tree, 2);
    bst_insert(tree, 5);
    bst_insert(tree, 4);
    bst_insert(tree, 3);
    bst_insert(tree, 100);
    bst_insert(tree, 105);
    bst_insert(tree, 102);
    bst_insert(tree, 103);
    bst_insert(tree, 104);
    bst_insert(tree, -100);
    assert(bst_contains(tree, -100));
    assert(bst_contains(tree, 1));
    bst_assert(tree);

    /* print out the tree in-order, pre-order and post-order */
    printf("in-order: ");
    bst_print(tree, BST_INORDER);

    printf("pre-order: ");
    bst_print(tree, BST_PREORDER);

    printf("post-order: ");
    bst_print(tree, BST_POSTORDER);

    /* remove some stuff */
    bst_assert(tree);
    assert(bst_remove(tree, 0));
    bst_assert(tree);
    assert(!bst_remove(tree, 0));
    bst_assert(tree);
    assert(!bst_contains(tree, 0));
    bst_assert(tree);
    assert(bst_remove(tree, -100));
    assert(bst_remove(tree, 100));
    assert(!bst_contains(tree, 100));
    assert(bst_contains(tree, 104));

    bst_assert(tree);
    assert(!bst_remove(tree, -99999));


    /* create degenerated tree */
    const int NUM = 90000;
    for(int i = 0; i < NUM; i++)
        bst_insert(tree, i);

    bst_assert(tree);

    /* this should be slow */
    for(int i = 0; i < NUM; i++)
        assert(bst_contains(tree, i));

    /* this should be slow as well */
    for(int i = 0; i < NUM; i++)
        assert(bst_remove(tree, i));

    bst_assert(tree);
    bst_free(tree);
}

/*
 * author: Sebastian Bauersfeld
 */

#include "red_black_tree.h"
#include "binary_search_tree.h"
#include <assert.h>
#include <stdio.h>

/* verify integrity of a red-black tree node and its sub-nodes */
int rbt_node_assert(bst_node* node){
    if(!node)
        return 1;

    int myLen = 0;

    switch(node->color){
        case RBT_RED:
            /* root needs to be black (at least in our implementation) */
            assert(node->parent);

            /* red nodes cannot have a red parent */
            assert(node->parent->color == RBT_BLACK);
            break;
        case RBT_BLACK:
            myLen = 1;
            break;
        default:
            assert(0);  /* invalid color */
    }
    int lenLeft = rbt_node_assert(node->left);
    int lenRight = rbt_node_assert(node->right);
    assert(lenLeft == lenRight);
    return lenLeft + myLen;
}

/* verify integrity of a red-black tree */
void rbt_assert(bst_tree* tree){
    bst_assert(tree);
    rbt_node_assert(tree->root);    
}

int red_conflict(bst_node* node){
    return node &&
           node->color == RBT_RED &&
           node->parent &&
           node->parent->color == RBT_RED;
}

int is_left(bst_node* node){
    assert(node);
    return node->parent && node->parent->left == node;
    // WARNING: roots are neither left nor right!
}

int is_right(bst_node* node){
    assert(node);
    return node->parent && node->parent->right == node;
    // WARNING: roots are neither left nor right!
}

bst_node* grandparent(bst_node* node){
    return (node && node->parent) ? node->parent->parent : 0;
}

bst_node* sibling(bst_node* node){
    if(node && node->parent)
        return (node->parent->left != node) ? node->parent->left : node->parent->right;
    return 0;
}

bst_node* uncle(bst_node* node){
    return node ? sibling(node->parent) : 0;
}

/* right-rotate around node */
void right_rotate(bst_node* node){
    assert(node);
    assert(node->left);

    bst_node* a = node;
    bst_node* b = a->left;
    bst_node* c = b->right;
    bst_node* p = a->parent;

    // put b up top
    if(is_left(a))
        p->left = b;
    else if(is_right(a))
        p->right = b;
    b->parent = p;

    //reconnect a
    a->parent = b;
    a->left = c;
    if(c) c->parent = a;
    b->right = a;
}

/* left-rotate around node */
void left_rotate(bst_node* node){
    assert(node);
    assert(node->right);

    bst_node* a = node;
    bst_node* b = a->right;
    bst_node* c = b->left;
    bst_node* p = a->parent;

    // put b up top
    if(is_left(a))
        p->left = b;
    else if(is_right(a))
        p->right = b;
    b->parent = p;

    //reconnect a
    a->parent = b;
    a->right = c;
    if(c) c->parent = a;
    b->left = a;
}

void rbt_repair(bst_node* node){
    assert(node);

    if(node->parent){
        if (red_conflict(node)){

            /* case 1: parent and uncle are red ==> color parent and uncle black and grandparent red */
            bst_node* u = uncle(node);
            if(u && u->color == RBT_RED){
                node->parent->color = RBT_BLACK;
                u->color = RBT_BLACK;
                bst_node* gp = grandparent(node);
                gp->color = RBT_RED;
                rbt_repair(gp);

            /* case 2.1 */
            }else if(is_right(node) && is_left(node->parent)){
                left_rotate(node->parent);
                rbt_repair(node->left);

            /* case 2.2 */
            }else if(is_left(node) && is_right(node->parent)){
                right_rotate(node->parent);
                rbt_repair(node->right);

            /* case 3.1 */
            }else if(is_left(node) && is_left(node->parent)){
                bst_node* gp = grandparent(node);
                bst_node* p = node->parent;
                right_rotate(gp);
                gp->color = RBT_RED;
                p->color = RBT_BLACK;
                
            /* case 3.2 */
            }else if(is_right(node) && is_right(node->parent)){
                bst_node* gp = grandparent(node);
                bst_node* p = node->parent;
                left_rotate(gp);
                gp->color = RBT_RED;
                p->color = RBT_BLACK;
            }
        }
    }else{  // it doesn't have a parent --> it is the root --> make it black
        node->color = RBT_BLACK;
    }
}

void rbt_insert_node(bst_node* node, int val){
    assert(node);
    if(node->val == val) // node already exists, we don't need to do anything
        return;

    if(val < node->val){
        if(node->left){
            rbt_insert_node(node->left, val);
        }else{
            bst_node* new = bst_create_node(node, 0, 0, val);
            new->color = RBT_RED;
            node->left = new;
            rbt_repair(new);
        }
    }else{
        if(node->right){
            rbt_insert_node(node->right, val);
        }else{
            bst_node* new = bst_create_node(node, 0, 0, val);
            new->color = RBT_RED;
            node->right = new;
            rbt_repair(new);
        }
    }
}


/* insert value into tree and restructure tree if needed
 * (rotation, color-switching) */
void rbt_insert(bst_tree* tree, int val){
    if(tree->root){
        rbt_insert_node(tree->root, val);
    }else{
        tree->root = bst_create_node(0, 0, 0, val);
        tree->root->color = RBT_BLACK;
    }
    while(tree->root->parent)
        tree->root = tree->root->parent;
}


/* a small test suite for the rbt implementation */
void rbt_test(){
    printf("testing red black trees...\n");
    bst_tree* tree = bst_create();
    assert(!bst_contains(tree, 0));

    rbt_insert(tree, 5);
    rbt_insert(tree, 1);
    rbt_insert(tree, 2);

    rbt_insert(tree, 4);
    rbt_insert(tree, 5);
    rbt_insert(tree, 5);
    rbt_insert(tree, -9999);
    rbt_assert(tree);

    assert(!bst_contains(tree, -500));
    assert(!bst_contains(tree, 3));
    assert(bst_contains(tree, -9999));
    assert(bst_contains(tree, 5));


    const int COUNT = 90000;
    for(int i = 0; i < COUNT; i++)
        rbt_insert(tree, i);

    for(int i = 0; i < COUNT; i++){
        assert(bst_contains(tree, i));
        assert(!bst_contains(tree, i + COUNT));
    }

    //bst_print(tree, BST_INORDER);
    rbt_assert(tree);
    bst_free(tree);
}

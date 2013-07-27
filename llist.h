/*
 * Implementation of singly linked integer lists with sentinel node
 * authors: Sebastian Bauersfeld, Javier Olaecha
 */

#ifndef LLIST_H
#define LLIST_H

typedef struct ll_node{
    int data;
    struct ll_node* next;
} ll_node;

typedef struct ll_list{
    ll_node* sent;
} ll_list;

ll_node* ll_create_node(int value, ll_node* next);
ll_list* ll_create_list();
int ll_is_empty(ll_list* l);
void ll_insert(ll_node* n, int value);
void ll_prepend(ll_list* l, int value);
void ll_append(ll_list* l, int value);
int ll_count(ll_list* l);
void ll_print(ll_list* l);
int ll_delete_first_of(ll_list* l, int value);
ll_node* reverse(ll_list* l, ll_node* n, ll_node* acc);
void ll_reverse_iter(ll_list* l);
void ll_reverse(ll_list* l);

#endif LLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "llist.h"

ll_node* ll_create_node(int value, ll_node* next){
    ll_node* ret = (ll_node*) malloc(sizeof(ll_node));
    ret->data = value;
    ret->next = next;
    return ret;
}

ll_list* ll_create_list(){
    ll_list* ret = (ll_list*) malloc(sizeof(ll_list));
    ret->sent = ll_create_node(0, 0);
    ret->sent->next = ret->sent;
    return ret;
}

int ll_is_empty(ll_list* l){ return l->sent->next == l->sent; }

void ll_insert(ll_node* n, int value){
    n->next = ll_create_node(value, n->next);
}

void ll_prepend(ll_list* l, int value){ ll_insert(l->sent, value); }

void ll_append(ll_list* l, int value){
    ll_node* current = l->sent;
    while(current->next != l->sent)
        current = current->next;
    ll_insert(current, value);
}

int ll_count(ll_list* l){
    ll_node* current = l->sent;
    int ret = 0;
    while(current->next != l->sent){
        current = current->next;
        ret++;
    }
    return ret;
}

void ll_print(ll_list* l){
    ll_node* current = l->sent->next;
    printf("[");
    while(current != l->sent){
        printf("%d ", current->data);
        current = current->next;
    }
    printf("]\n");
}

int ll_delete_first_of(ll_list* l, int value){
    ll_node* current = l->sent;
    while(current->next != l->sent){
        if(current->next->data == value){
            ll_node* tmp = current->next->next;
            free(current->next);
            current->next = tmp;
            return 1;
        }
        current = current->next;
    }
    return 0;
}

ll_node* reverse(ll_list* l, ll_node* n, ll_node* acc){
    if(n == l->sent)
        return acc;
    ll_node* rest = n->next;
    n->next = acc;
    return reverse(l, rest, n);
}

void ll_reverse_iter(ll_list* l){
    ll_node* rev = l->sent;
    ll_node* curr = l->sent->next;

    while(curr != l->sent){
        ll_node* rest = curr->next;
        curr->next = rev;
        rev = curr;
        curr = rest;
    }
    l->sent->next = rev;
}

void ll_reverse(ll_list* l){
    //l->sent->next = reverse(l, l->sent->next, l->sent);
    ll_reverse_iter(l);
}

int main(int argc, char** args){
    ll_list* l = ll_create_list();
    ll_print(l);
    ll_reverse(l);
    ll_print(l);
    assert(ll_is_empty(l));
    assert(ll_count(l) == 0);
    ll_append(l, 5);
    ll_print(l);
    ll_reverse(l);
    ll_print(l);
    assert(!ll_is_empty(l));
    assert(ll_count(l) == 1);

    for(int i = 0; i < 10; i++)
        ll_append(l, i);

    assert(ll_count(l) == 11);
    ll_prepend(l, 99);
    ll_prepend(l, 100);
    ll_prepend(l, 101);
    ll_print(l);
    assert(ll_count(l) == 14);
    assert(ll_delete_first_of(l, 4));
    assert(ll_count(l) == 13);
    ll_print(l);
    assert(!ll_delete_first_of(l, 4));
    assert(ll_delete_first_of(l, 9));
    assert(!ll_delete_first_of(l, 9));
    assert(ll_count(l) == 12);
    ll_print(l);
    ll_reverse(l);
    ll_print(l);
    ll_reverse(l);
    ll_print(l);
    return 0;
}

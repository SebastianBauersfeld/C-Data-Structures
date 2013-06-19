#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"

linked_list* ll_create(){
  linked_list* ll = (linked_list *)malloc(sizeof(linked_list));
  ll->head = NULL;
  return ll;
};

void ll_print(linked_list* ll){
  ll_node* node = ll->head;
  while(node) {
    printf("%d", node->value);
    if (ll_get_next_node(node) != NULL){
      printf(", ");
    } else {
      printf("\n");
    };
    node = ll_get_next_node(node);
  };
};

ll_node* ll_get_next_node(ll_node* node){
  return node->next;
};

void ll_prepend(linked_list* linked_list, int value){
  ll_node* new_head = node_create(value, linked_list->head);
  linked_list->head = new_head;
};

ll_node* node_create(int value, ll_node* next){
  ll_node* node = (ll_node*)malloc(sizeof(ll_node));
  node->value = value;
  node->next = next;
  return node;
};

ll_node* nth_node(linked_list* ll, int position){
  ll_node* curr_node = ll->head;
  for(int i = 0; i < position; i++){
    if(curr_node == NULL) return NULL;
    curr_node = ll_get_next_node(curr_node);
  }
  return curr_node;
};

void insert_ll(ll_node* new_node, ll_node* prev_node){
  new_node->next = prev_node->next;
  prev_node->next = new_node;
};

int main (int argc, char **args) {
  linked_list* ll = ll_create();
  ll_prepend(ll, 3);
  ll_print(ll);
  ll_prepend(ll, 5);
  ll_prepend(ll, 11);
  ll_print(ll);
  insert_ll(node_create(1, NULL), nth_node(ll, 1));
  ll_print(ll);
  return 0;
}

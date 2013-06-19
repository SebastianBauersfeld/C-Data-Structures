#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"

ll_node* ll_create(int value){
  ll_node* result = (ll_node *)malloc(sizeof(ll_node));
  result->value = value;
  result->next = NULL;

  return result;
};

void ll_print(ll_node* node){
  do {
    printf("%d", node->value);
    if (ll_get_next_node(node) != NULL){
      printf(", ");
    } else {
      printf("\n");
    };
    node = ll_get_next_node(node);
  } while(node);
};

ll_node* ll_get_next_node(ll_node* node){
  return node->next;
};

int main (int argc, char **args) {
  ll_node* linked_list = ll_create(3);
  ll_print(linked_list);
  /* prepend_ll(linked_list, 5); */
  /* prepend_ll(linked_list, 11); */
  /* ll_print(linked_list); */
  return 0;
}

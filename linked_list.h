typedef struct ll_node {
  int value;
  struct ll_node * next;

} ll_node;

typedef struct linked_list {
  ll_node* head;
} linked_list;

linked_list* ll_create();
ll_node* node_create(int value, ll_node* next);
ll_node* ll_get_next_node(ll_node*);
void ll_print(linked_list* linked_list);
void ll_prepend(linked_list* linked_list, int value);
int ll_size(ll_node*);
void insert_ll(ll_node* new_node, ll_node* prev_node);
ll_node* nth_node(linked_list* ll, int position);
/* ll_node* find_value(linked_list* ll, int value); */

/* append_ll */

/* delete_ll */

typedef struct ll_node {
  int value;
  struct ll_node * next;

} ll_node;

ll_node* ll_create(int value);
ll_node* ll_get_next_node(ll_node*);
void ll_print(ll_node* linked_list);
ll_node* prepend_ll(ll_node* linked_list, int value);
int ll_size(ll_node*);

/* append_ll */
/* insert_ll */
/* delete_ll */
/* find_value */

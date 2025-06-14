#ifndef LINKED_LIST
#define LINKED_LIST

typedef struct linked_list_node_struct{
	struct linked_list_node_struct *next;
	int data;

}linked_list_node;

void linked_list_append(linked_list_node **, linked_list_node *);
linked_list_node *make_node(const int);
int linked_list_remove_instance(linked_list_node **, const int);
void linked_list_free(linked_list_node *);
int linked_list_insert_after(linked_list_node **, linked_list_node *, const int);
linked_list_node *linked_list_clone(const linked_list_node *);
#endif

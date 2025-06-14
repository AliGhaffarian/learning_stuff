#include "linked_list.h"
#include <stdlib.h>

void linked_list_append(linked_list_node **head_addr, linked_list_node *append_node){
	linked_list_node **ptr_of_interest = head_addr;

	while(*ptr_of_interest != (linked_list_node *)NULL) ptr_of_interest = &(*ptr_of_interest)->next;

	*(ptr_of_interest) = append_node;
}

linked_list_node *make_node(const int data){
	linked_list_node *tmp_node = malloc(sizeof(linked_list_node));
	if (! tmp_node) return (linked_list_node *)NULL;

	tmp_node->next = (linked_list_node *)NULL;
	tmp_node->data = data;
	return tmp_node;
}

int linked_list_remove_instance(linked_list_node **head_addr, int data){
	linked_list_node **ptr_to_current = head_addr;

	while((*ptr_to_current)->data != data){

		if ((*ptr_to_current)->next == (linked_list_node *)NULL) return -1;

		ptr_to_current = &(*ptr_to_current)->next;
	}
	linked_list_node *ptr_to_free = *ptr_to_current;
	(*ptr_to_current) = (*ptr_to_current)->next;
	free((void *)ptr_to_free);

	return 0;
}

int linked_list_insert_after(linked_list_node **head_addr, linked_list_node *insert_node, const int data){
	linked_list_node **ptr_to_current = head_addr;

	while((*ptr_to_current)->data != data){
		if ((*ptr_to_current)->next == (linked_list_node *)NULL) return -1;

		ptr_to_current = &(*ptr_to_current)->next;
	}
	linked_list_node *next = (*ptr_to_current)->next;
	(*ptr_to_current)->next = insert_node;
	insert_node->next = next;

	return 0;
}

void linked_list_free(linked_list_node *head){
	linked_list_node *current = head;
	linked_list_node *next = current->next;
	while(current != (linked_list_node *)NULL){
		next = current->next;
		free(current);
		current = next;
	}
}

linked_list_node *linked_list_clone(const linked_list_node *head){
	const linked_list_node *current = head;

	linked_list_node *clone_head = (linked_list_node *)NULL;
	linked_list_node **ptr_to_clone_current = &clone_head;

	while(current != (linked_list_node *)NULL){
		*ptr_to_clone_current = make_node(current->data);
		if (*ptr_to_clone_current == (linked_list_node *)NULL){
			linked_list_free(clone_head);
			return (linked_list_node *)NULL;
		}
		current = current->next;
		ptr_to_clone_current = &(*ptr_to_clone_current)->next;
	}
	return clone_head;
}

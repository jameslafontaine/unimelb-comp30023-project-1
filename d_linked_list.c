#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "d_linked_list.h"

// MAY NEED TO REFACTOR FUNCTIONS TO TAKE POINTER TO THE POINTER TO THE HEAD SO THAT IT ALTERS THE LIST IN PLACE INSTEAD OF A LOCAL VERSION CREATED WITHIN THE FUNCTION SCOPE

ListNode* insert_at_head(ListNode* head, void* element_ptr) {
	ListNode* new_head;
	ListNode* old_head;

	// if head is NULL then the list needs to be initialised
	if (!head) {
		//printf("head==NULL\n");
		new_head = (ListNode *)  malloc(sizeof(ListNode));
		assert(new_head);
		new_head->element = element_ptr;
		new_head->next = NULL;
		new_head->prev = NULL;
		return new_head;
	}
	// otherwise we insert a new node at the head
	else {
		//printf("head!=NULL\n");
		old_head = head;
		new_head = (ListNode *) malloc(sizeof(ListNode));
		assert(head);
		new_head->element = element_ptr;
		new_head->next = old_head;
		new_head->prev = NULL;
		old_head->prev = new_head;

		return new_head;
	}
}

ListNode* insert_at_tail(ListNode* head, void* element_ptr) {
	ListNode* new_head;
	ListNode* new_tail;
	ListNode* old_tail;

	// if head is NULL then the list needs to be initialised
	if (!head) {
		new_head = (ListNode *) malloc(sizeof(ListNode));
		assert(new_head);
		new_head->element = element_ptr;
		new_head->next = NULL;
		new_head->prev = NULL;
		return new_head;
	}
	// otherwise we find the tail of the list and insert a new node there
	else {
		old_tail = retrieve_tail(head);
		new_tail = (ListNode *) malloc(sizeof(ListNode));
		assert(new_tail);
		new_tail->element = element_ptr;
		new_tail->next = NULL;
		new_tail->prev = old_tail;
		old_tail->next = new_tail;

		return head;
	}
}

ListNode* remove_head(ListNode* head) {
	ListNode* new_head;
	// if head is NULL then do nothing
	if (!head) {
		return head;
	}
	// if the head is the only element then we can simply remove it
	else if (!(head->next)) {
		free_node(head);
		return head;
	}
	// otherwise we free the current head and point to the next node as the new
	// head
	else {
		new_head = head->next;
		new_head->prev = NULL;
		free_node(head);
	}

	return new_head;
}

ListNode* remove_tail(ListNode* head) {
	ListNode* old_tail;
	ListNode* new_tail;

	// if head is NULL then do nothing
	if (!head) {
		return head;
	}
	// if the head is the tail then remove it
	else if (!(head->next)) {
		free_node(head);
		return head;
		// otherwise we find the tail of the list and remove it
	} else {
		old_tail = retrieve_tail(head);
		new_tail = old_tail->prev;
		new_tail->next = NULL;
		free_node(old_tail);

		return head;
	}
}

ListNode* retrieve_tail(ListNode* head) {
	ListNode* current;

	// keep traversing the list until the tail is found and return it
	current = head;
	while (current->next) {
		current = current->next;
	}
	return current;
}

void swap_nodes(ListNode** node1, ListNode** node2) {
	ListNode* tmp_prev;

	tmp_prev = (*node1)->prev;
   (*node1)->next = (*node2)->next;
   (*node1)->prev = (*node2);
   (*node2)->next = (*node1);
   (*node2)->prev = tmp_prev;
}

// Sorts the provided linked list using the provided comparison function and insertion sort
void ins_sort_list(ListNode** head, int (*cmp_func)(ListNode**, ListNode**)) {
	ListNode** currenti = head;
	ListNode** currentj;

	if (*currenti == NULL) {
		return;
	}
    while ((*currenti)->next != NULL) {
		currenti = &((*currenti)->next);
		currentj = currenti;
    	while ((*currentj)->prev != NULL && cmp_func(currentj, &((*currentj)->prev)) == GREATER_THAN) {
        	swap_nodes(&((*currentj)->prev), currentj);
        	currentj = &((*currentj)->prev);
		}
	}
		
}


ListNode* enqueue(ListNode* queue_head, void* elem_ptr) {
    return insert_at_tail(queue_head, elem_ptr);
}

ListNode* dequeue(ListNode* queue_head) {
    return remove_head(queue_head);
}







void free_node(ListNode* node) {
	free(node->element);
	free(node);
	node = NULL;
	assert(!node);
}

void free_list(ListNode* head) {
	ListNode* current = head;
	ListNode* next;

  	// iterate through list until the end of the list (NULL) is reached
	for (next = current; current != NULL; current = next) {
    	// store next pointer before we free
    	next = current->next;
    	free_node(current);
	}
}

/*
int get_list_length(ListNode* head) {
	int i=0;
	ListNode* current;

	current = head;
	while(current) {
		i++;
		current = current->next;
	}

	return i;
}
*/
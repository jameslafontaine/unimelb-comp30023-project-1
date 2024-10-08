#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "d_linked_list.h"

/* Adds the element pointer to the head of the linked list pointed to by the provided pointer */
ListNode* insert_at_head(ListNode* head, void* element_ptr) {
	ListNode* new_head;
	ListNode* old_head;

	// If head is NULL then the list needs to be initialised
	if (!head) {
		new_head = (ListNode *)  malloc(sizeof(ListNode));
		if (new_head == NULL) {
        	fprintf(stderr, "Malloc failure\n");
        	exit(EXIT_FAILURE);
    	}
		new_head->element = element_ptr;
		new_head->next = NULL;
		new_head->prev = NULL;
		return new_head;
	}
	// Otherwise we insert a new node at the head
	else {
		old_head = head;
		new_head = (ListNode *) malloc(sizeof(ListNode));
		if (new_head == NULL) {
        	fprintf(stderr, "Malloc failure\n");
        	exit(EXIT_FAILURE);
    	}
		new_head->element = element_ptr;
		new_head->next = old_head;
		new_head->prev = NULL;
		old_head->prev = new_head;

		return new_head;
	}
}

/* Adds the element pointer to the tail of the linked list pointed to by the provided pointer */
ListNode* insert_at_tail(ListNode* head, void* element_ptr) {
	ListNode* new_head;
	ListNode* new_tail;
	ListNode* old_tail;

	// If head is NULL then the list needs to be initialised
	if (!head) {
		new_head = (ListNode *) malloc(sizeof(ListNode));
		if (new_head == NULL) {
        	fprintf(stderr, "Malloc failure\n");
        	exit(EXIT_FAILURE);
    	}
		new_head->element = element_ptr;
		new_head->next = NULL;
		new_head->prev = NULL;
		return new_head;
	}
	// Otherwise we find the tail of the list and insert a new node there
	else {
		old_tail = retrieve_tail(head);
		new_tail = (ListNode *) malloc(sizeof(ListNode));
		if (new_tail == NULL) {
        	fprintf(stderr, "Malloc failure\n");
        	exit(EXIT_FAILURE);
    	}
		new_tail->element = element_ptr;
		new_tail->next = NULL;
		new_tail->prev = old_tail;
		old_tail->next = new_tail;

		return head;
	}
}

/* Adds the element pointer to the node next to the provided node */
ListNode* insert_after_node(ListNode* node, void* element_ptr) {
	ListNode* new_node = NULL;

	// Return the the new node if the provided node doesn't exist (i.e. the list is empty)
	if (node == NULL) {
		new_node = (ListNode *) malloc(sizeof(ListNode));
		if (new_node== NULL) {
        	fprintf(stderr, "Malloc failure\n");
        	exit(EXIT_FAILURE);
    	}
		new_node->element = element_ptr;
		new_node->next = NULL;
		new_node->prev = NULL;
		return new_node;
	}
	// Otherwise insert the new node after the provided node
	else {
		new_node = (ListNode *) malloc(sizeof(ListNode));
		if (new_node== NULL) {
        	fprintf(stderr, "Malloc failure\n");
        	exit(EXIT_FAILURE);
    	}
		new_node->element = element_ptr;
		new_node->next = node->next;
		new_node->prev = node;
		node->next = new_node;
		if (new_node->next != NULL) {
			(new_node->next)->prev = new_node;
		}

		return node;
	}
	
}

/* Removes the node at the head of the linked list and returns the new head pointer */
ListNode* remove_head(ListNode* head) {
	ListNode* new_head;
	// If head is NULL then do nothing
	if (!head) {
		return head;
	}
	// If the head is the only element then we can simply remove it
	else if (!(head->next)) {
		free_node(&head);
		return head;
	}
	// Otherwise we free the current head and point to the next node as the new
	// head
	else {
		new_head = head->next;
		new_head->prev = NULL;
		free_node(&head);
		head = NULL;
		return new_head;
	}
}

/* Removes the node at the tail of the linked list and returns the new head pointer */
ListNode* remove_tail(ListNode* head) {
	ListNode* old_tail;
	ListNode* new_tail;

	// If head is NULL then do nothing
	if (!head) {
		return head;
	}
	// If the head is the tail then remove it
	else if (!(head->next)) {
		free_node(&head);
		return head;
	// Otherwise we find the tail of the list and remove it
	} else {
		old_tail = retrieve_tail(head);
		new_tail = old_tail->prev;
		new_tail->next = NULL;
		free_node(&old_tail);
		old_tail = NULL;
		return head;
	}
}

/* Retrieves a pointer to the head node of the list */
ListNode* retrieve_head(ListNode* node) {
	ListNode* current;

	// Keep traversing the list until the head is found and return it
	current = node;
	while(current->prev) {
		current = current->prev;
	}
	return current;
}

/* Retrieves a pointer to the tail node of the list */
ListNode* retrieve_tail(ListNode* node) {
	ListNode* current;

	// Keep traversing the list until the tail is found and return it
	current = node;
	while (current->next) {
		current = current->next;
	}
	return current;
}

/* Swaps the data of two adjacent nodes in a linked list */
void swap_nodes(ListNode* node1, ListNode* node2) {
	void* tmp;

	tmp = node1->element;
    node1->element = node2->element;
	node2->element = tmp;
}

/* Sorts the provided linked list using the provided comparison function and insertion sort */
ListNode* ins_sort_list(ListNode* head, int (*cmp_func)(ListNode*, ListNode*)) {
	ListNode* currenti = head;
	ListNode* currentj;

	if (currenti == NULL) {
		return head;
	}
    while (currenti->next != NULL) {
		currenti = currenti->next;
		currentj = currenti;
    	while (currentj->prev != NULL && cmp_func(currentj->prev, currentj) == GREATER_THAN) {
        	swap_nodes(currentj->prev, currentj);
        	currentj = currentj->prev;
		}
	}

	return head;
}

/* Place a process at the end of the provided 'queue' */
ListNode* enqueue(ListNode* queue_head, void* elem_ptr) {
    return insert_at_tail(queue_head, elem_ptr);
}

/* Remove a process from the start of the provided 'queue' (doesn't return the process) */
ListNode* dequeue(ListNode* queue_head) {
    return remove_head(queue_head);
}

/* Displays the connections of the linked list (prev, current and next addresses) */
void print_list(ListNode* head) {
	ListNode* current = head;

	printf("###################################################################\n");
	while (current) {
		printf("current = %p\n", current);
		printf("current->prev = %p\n", current->prev);
		printf("current->next = %p\n", current->next);
		current = current->next;
	}
	printf("###################################################################\n");
}


/* Frees allocated memory for the given node */
void free_node(ListNode** node) {
	free((*node)->element);
	(*node)->element = NULL;
	free(*node);
	*node = NULL;

} 

/* Frees an entire linked list given the head */
void free_list(ListNode** head) {
	ListNode* current = *head;
	ListNode* next;

  	// Iterate through list until the end of the list (NULL) is reached
	for (next = current; current != NULL; current = next) {
    	// Store next pointer before we free
    	next = current->next;
    	free_node(&current);
		current = NULL;
	}
}

/*
void transfer_node(ListNode** head1, ListNode** head2) {
}
*/

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
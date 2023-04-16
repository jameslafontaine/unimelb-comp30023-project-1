#ifndef DLL_INCLUDED
#define DLL_INCLUDED


#define LESS_THAN -1
#define EQUAL 0
#define GREATER_THAN 1

/*
   Provides various tools for manipulating doubly linked lists
*/

typedef struct node ListNode;

struct node {
	void* element;
	ListNode* next;
	ListNode* prev;
};

/* Adds the element pointer to the head of the linked list pointed to by the provided pointer */
ListNode* insert_at_head(ListNode* head, void* element_ptr);

/* Adds the element pointer to the tail of the linked list pointed to by the provided pointer */
ListNode* insert_at_tail(ListNode* head, void* element_ptr);

/* Adds the element pointer to the node next to the provided node */
ListNode* insert_after_node(ListNode* node, void* element_ptr);

/* Removes the node at the head of the linked list and returns the new head pointer */
ListNode* remove_head(ListNode* head);

/* Removes the node at the tail of the linked list and returns the new head pointer */
ListNode* remove_tail(ListNode* head);

/* Retrieves a pointer to the head node of the list */
ListNode* retrieve_head(ListNode* node);

/* Retrieves a pointer to the tail node of the list */
ListNode* retrieve_tail(ListNode* node);

/* Swaps the data of two adjacent nodes in a linked list */
void swap_nodes(ListNode* node1, ListNode* node2);

/* Sorts the provided linked list using the provided comparison function and insertion sort */
ListNode* ins_sort_list(ListNode* head, int (*cmp_func)(ListNode*, ListNode*));

/* Place a process at the end of the provided 'queue' */
ListNode* enqueue(ListNode* queue_head, void* elem_ptr);

/* Remove a process from the start of the provided 'queue' (doesn't return the process) */
ListNode* dequeue(ListNode* queue_head);

/* Displays the connections of the linked list (prev, current and next addresses) */
void print_list(ListNode* head);

/* Frees allocated memory for the given node */
void free_node(ListNode** node);

/* Frees an entire linked list given the head */
void free_list(ListNode** head);

/* Transfers the head of list1 to the tail of list2
void transfer_node(ListNode** head1, ListNode** head2);
*/

/* Calculates the length of the list starting from the supplied head */
// int get_list_length(ListNode* head);



#endif

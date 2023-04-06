/**
 * Provides various tools for manipulating doubly linked lists
 */

typedef struct node ListNode;

struct node {
	void* element;
	ListNode* next;
	ListNode* prev;
};

/*
   adds the element pointer to the head of the linked list pointed to by the
   provided pointer
 */
ListNode* insert_at_head(ListNode* head, void* element_ptr);

/*
   adds the element pointer to the tail of the linked list pointed to by the
   provided pointer
 */
ListNode* insert_at_tail(ListNode* head, void* element_ptr);

/*
   removes the node at the head of the linked list and returns the new head
   pointer
 */
ListNode* remove_head(ListNode* head);

/*
   removes the node at the tail of the linked list and returns the new head
   pointer
 */
ListNode* remove_tail(ListNode* head);

/*
   retrieves a pointer to the tail node of the list
 */
ListNode* retrieve_tail(ListNode* head);

/*
   calculates the length of the list starting from the supplied head
 */
// int get_list_length(ListNode* head);

/*
   frees allocated memory for the given node and double checks
 */
void free_node(ListNode* node);

/*
   frees an entire linked list and double checks at each node
*/
void free_list(ListNode* head);

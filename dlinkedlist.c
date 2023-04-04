#include <stdlib.h>
#include "linkedlist.h"

ListNode* insert_at_head(ListNode* head, void* element_ptr) {
    ListNode* old_head;

    // if head is NULL then the list needs to be initialised
    if (!head) {
        head = malloc(sizeof(ListNode));
        assert(head);
        head->element = element_ptr;
        head->next = NULL;
        head->prev = NULL;
        return head;
    // otherwise we insert a new node at the head
    } else {
        old_head = head;
        head = malloc(sizeof(ListNode));
        assert(head);
        head->next = old_head;
        head->prev = NULL;
        old_head->prev = head;

    return head;
    }
}

ListNode* insert_at_tail(ListNode* head, void* element_ptr) {
    ListNode* new;
    ListNode* tail;

    // if head is NULL then the list needs to be initialised
    if (!head) {
        head = malloc(sizeof(ListNode));
        assert(head);
        head->element = element_ptr;
        head->next = NULL;
        head->prev = NULL;
        return head;
    // otherwise we find the tail of the list and insert a new node there
    } else {
        tail = retrieve_tail(head);
        new = malloc(sizeof(ListNode));
        assert(new);
        new->element = element_ptr;
        new->next = NULL;
        new->prev = tail;
        tail->next = new;

    return head;
    }
}

ListNode* remove_head(ListNode* head) {
    ListNode* new_head;
    // if head is NULL then do nothing
    if (!head) {
        return head;
    // if the head is the only element then we can simply remove it
    if (!(head->next)) {
        free_node(head);
    } 
    // otherwise we free the current head and point to the next node as the new head
    } else {
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
    // if the head is the tail then remove it
    } else if (!(head->next)) {
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

void free_node(ListNode* node) {
    free(node);
    node = NULL;
    assert(!node);
}
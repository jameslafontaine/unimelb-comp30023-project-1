#include "d_linked_list.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define HEAD_TEST
#define TAIL_TEST

int test_list[5] = {1, 2, 3, 4, 5};

ListNode* test_head;

int main() {

#ifdef HEAD_TEST

	printf("\n#################################################################"
		   "###################################\n");
	for (int i = 0; i < 5; i++) {
		printf("i = %d\n", i);
		int* element_ptr = test_list + i;
		printf("test_list + i = %d\n", test_list[i]);
		test_head = insert_at_head(test_head, element_ptr);
		printf("inserted %d at head\n", test_list[i]);
		printf("test_head->element = %d\n", *((int*)(test_head->element)));
		if (i > 0) {
			printf("test_head->next->element = %d\n",
				   *((int*)(test_head->next->element)));
			printf("test_head->next->prev->element = %d\n",
				   *((int*)(test_head->next->prev->element)));
		}
	}

	for (int i = 0; i < 5; i++) {
		printf("i = %d\n", i);
		test_head = remove_head(test_head);
		printf("removed head\n");
	}

	printf("\n#################################################################"
		   "###################################\n");

	// free_list(test_head);
	test_head = NULL;

#endif

#ifdef TAIL_TEST

	for (int i = 0; i < 5; i++) {
		printf("i = %d\n", i);
		int* element_ptr = test_list + i;
		printf("test_list + i = %d\n", test_list[i]);
		test_head = insert_at_tail(test_head, element_ptr);
		printf("inserted %d at tail\n", test_list[i]);
		printf("test_tail->element = %d\n",
			   *((int*)(retrieve_tail(test_head)->element)));
		if (i > 0) {
			printf("test_head->next->element = %d\n",
				   *((int*)(test_head->next->element)));
			printf("test_head->next->prev->element = %d\n",
				   *((int*)(test_head->next->prev->element)));
		}
	}

	for (int i = 0; i < 5; i++) {
		printf("i = %d\n", i);
		test_head = remove_tail(test_head);
		printf("removed tail\n");
	}

	printf("\n#################################################################"
		   "###################################\n");

	// free_list(test_head);
	test_head = NULL;

#endif

	return 0;
}

/*#include "linkedList.h"

void createLinkedList(node_t * head, char * ip) {
  head = malloc(sizeof(node_t));
  if (head == NULL) {
    return;
  }

  strcpy(head->ip, ip);
  head->next = NULL;
}

void push(node_t * head, char * ip) {
    node_t * current = head;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = malloc(sizeof(node_t));
    strcpy(current->next->ip, ip);
    current->next->next = NULL;
}

void popFirst(node_t ** head) {
    node_t * next_node = NULL;

    if (*head == NULL) {
        return;
    }

    next_node = (*head)->next;
    free(*head);
    *head = next_node;
}

void popLast(node_t * head) {
    node_t * current;

    if (head->next == NULL) {
        free(head);
        head = NULL;
        return;
    }

    current = head;

    while (current->next->next != NULL) {
        current = current->next;
    }
}

void popByIndex(node_t ** head, int index) {
    int i = 0;
    node_t * current = *head;
    node_t * temp_node = NULL;

    if (index == 0) {
       popFirst(head);
       return;
    }

    for (int i = 0; i < index-1; i++) {
        if (current->next == NULL) {
            return;
        }
        current = current->next;
    }

    temp_node = current->next;
    current->next = temp_node->next;
    free(temp_node);
}

void clearLinkedList(node_t ** head) {
  node_t * current = * head;
  node_t * previous;
  while (current->next != NULL) {
    previous = current;
    current = current->next;
    free(current);
  }
  free(current);
}

void printAllAddresses(node_t * head) {
  node_t * current = head;
  while(current->next != NULL) {
    printf("%s\n", current->ip);
    current = current->next;
  }
  printf("%s\n", current->ip);
}
*/

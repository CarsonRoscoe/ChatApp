/*#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
  char * ip;
  struct node *next;
};

typedef struct node node_t;

void createLinkedList(node_t * head, char * ip);
void push(node_t * head, char * ip);
void popFirst(node_t ** head);
void popLast(node_t * head);
void popByIndex(node_t ** head, int index);
void clearLinkedList(node_t ** head);
void printAllAddresses(node_t * head);

#endif //LINKEDLIST_H
*/

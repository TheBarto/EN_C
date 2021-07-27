#ifndef LINKEDLIST_H
#define LINKEDLIST_H 

#include <stdlib.h>
#include <stdio.h>
#include "defines.h"

struct Node
{
	void* data;
	struct Node* next;
	struct Node* previous;
};

/* Nota aclaratoria. Ahora la lista enlazada consta de una
 * sola estructura. El Node que hace de LinkedList tiene un
 * un puntero al primer elemento(next), un puntero al ultimo
 * elemento(previous), y el numero de elementos en data */

typedef struct Node Node;
typedef struct Node LinkedList;

typedef void* (userCopyFunction)(void *data);

/* ---- NODE DEFINES ---- */
#define NEXT(n) n->next
#define PREVIOUS(n) n->previous
#define DATA(n) n->data

/* ---- LIST DEFINES ---- */
#define FIRST(l) NEXT(l)
#define LAST(l) PREVIOUS(l)
#define INFO(l) DATA(l)


/* ---- GET NODE FUNCTIONS ---- */
#define getDataNode(node) (DATA(node))
#define getPreviousNode(node) (PREVIOUS(node))
#define getNextNode(node) (NEXT(node))

#define increaseCounter(l) ((*(uint32_t *)INFO(l))++)
#define decreaseCounter(l) ((*(uint32_t *)INFO(l))--)

#define getNElems(l) ((*(uint32_t *)INFO(l)))
#define resetNElems(l) ((*(uint32_t *)INFO(l)) = 0)

Node* createNode(void* data);
Node* createNodeCopyData(void* data, Types type);
Node* createNodeUserCopyData(void* data, userCopyFunction function);
void addNewNode(Node* node, Node* newNode);
void* deleteNode(Node* node);
//void swapValueNodes(int* n1, int* n2);
//void swapNodes(Node* n1, Node* n2);
void freeNodeLinkedList(Node* node);
void freeDeepCopyNodeLinkedList(Node* node);
//void moveNode(Node* n1, Node* n2);

LinkedList* createLinkedList();
void appendElem(LinkedList* list, void* data);
void appendElemIndex(LinkedList* list, void* data, uint32_t pos);
void appendElemDeepCopy(LinkedList* list, void* data, userCopyFunction function);
void appendElemIndexDeepCopy(LinkedList* list, void* data, uint32_t pos, userCopyFunction function);
void* popElem(LinkedList* list);
void* popElemIndex(LinkedList* list, uint32_t posElem);
bool isEmpty(LinkedList* list);
void* getLinkedListElem(LinkedList* list, uint32_t position);
void swapLinkedListNodes(LinkedList* list, uint32_t swapPosition1, uint32_t swapPosition2);
void swapLinkedListValueNodes(LinkedList* list, uint32_t swapPosition1, uint32_t swapPosition2);
LinkedList* duplicateLinkedList(LinkedList* list);
LinkedList* duplicateDeepCopyLinkedList(LinkedList* list, userCopyFunction function);
void deleteLinkedList(LinkedList* list);
void deleteDeepCopyLinkedList(LinkedList* list);
void changePositionNodeLinkedList(LinkedList* list, uint32_t nodePosition, uint32_t newPosition);

void* deepCopyUInt8Value(void* data);
void* deepCopyInt32Value(void* data);
uint32_t getUInt32ArrayWithListElements(LinkedList* list, uint32_t* array);
uint8_t getUInt8ArrayWithListElements(LinkedList* list, uint8_t** array);
#endif 

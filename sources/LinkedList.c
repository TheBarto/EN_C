#include "../headers/LinkedList.h"

Node* createNode(void* data)
{
	Node *node = (Node *)malloc(sizeof(Node));
	PREVIOUS(node) = NULL;
	NEXT(node) = NULL;
	DATA(node) = data;	
	
	return node;
}

Node* createNodeDeepCopy(void* data, userCopyFunction function)
{
	Node *node = (Node *)malloc(sizeof(Node));
	PREVIOUS(node) = NULL;
	NEXT(node) = NULL;
	DATA(node) = function(data);

	return node;
}

void addNewNode(Node* node, Node* newNode)
{
	NEXT(newNode) = NEXT(node);
	
	PREVIOUS(newNode) = (struct Node*)node;

	if(NEXT(newNode))
		PREVIOUS(NEXT(newNode)) = (struct Node*)newNode;
	NEXT(node) = (struct Node*)newNode;

	return;
}

/* ELIMINA UN NODO DE LA LISTA Y REAJUSTA PUNTEROS */
void* deleteNode(Node* node)
{
	if(NEXT(node) != NULL)
		PREVIOUS(NEXT(node)) = PREVIOUS(node);
	if(PREVIOUS(node) != NULL)
		NEXT(PREVIOUS(node)) = NEXT(node);
	
	PREVIOUS(node) = NULL;
	NEXT(node) = NULL;
	void* data = DATA(node);
	free(node);
	node = NULL;

	return data;
}

/* ELIMINA UN NODO LIBERANDOLO */
void freeNodeLinkedList(Node* node)
{
	NEXT(node) = NULL;
	PREVIOUS(node) = NULL;

	free(node);
	node = NULL;	
}

/* ELIMINA UN NODO LIBERANDOLO */
void freeDeepCopyNodeLinkedList(Node* node)
{
	NEXT(node) = NULL;
	PREVIOUS(node) = NULL;

	free(DATA(node));
	DATA(node) = NULL;
	free(node);
	node = NULL;	
}


/* CREA UNA LISTA */
LinkedList* createLinkedList()
{
	LinkedList* l = (LinkedList *)malloc(sizeof(LinkedList));
	INFO(l) = (uint32_t *)malloc(sizeof(uint32_t));
	resetNElems(l);
	//NELEMS(l) = 0;
	FIRST(l) = NULL;
	LAST(l) = NULL;

	return l;
}

/* ANADE UN ELEMENTO A LA LISTA */
void appendElem(LinkedList* list, void* data)
{
	if(!LAST(list))
	{
		FIRST(list) = createNode(data);
		LAST(list) = FIRST(list);
	}
	else
	{
		Node* node = createNode(data);
		addNewNode(LAST(list), node);
		LAST(list) = node;
	}

	increaseCounter(list);
	return;
}

/* ANADE UN ELEMENTO A LA LISTA EN LA POSICION INDICADA */
void appendElemIndex(LinkedList* list, void* data, uint32_t pos)
{
	uint32_t i = DFT_CERO_U32;
	Node* node = FIRST(list);
	for(; i < (pos-1); i++, node = getNextNode(node));
	
	addNewNode(node, createNode(data));
	increaseCounter(list);

	return;
}

/* AÑADE UN ELEMENTO A LA LISTA COPIANDO EL ELEMENTO */
void appendElemDeepCopy(LinkedList* list, void* data, userCopyFunction function)
{
	if(!LAST(list))
	{
		printf("appendElemDeepCopy > vacia y guardar: %d\n", *(uint32_t *)data);
		FIRST(list) = createNodeDeepCopy(data, function);
		LAST(list) = FIRST(list);
	}
	else
	{
		printf("appendElemDeepCopy > NO vacia y guardar: %d\n", *(uint32_t *)data);
		Node* node = createNodeDeepCopy(data, function);
		addNewNode(LAST(list), node);
		LAST(list) = node;
	}

	increaseCounter(list);
	return;
}

/* ANADE UN ELEMENTO A LA LISTA EN LA POSICION INDICADA COPIANDO EL ELEMENTO */
void appendElemIndexDeepCopy(LinkedList* list, void* data, uint32_t pos, userCopyFunction function)
{
	
	if(getNElems(list) == 0)
	{
		appendElem(list, data);
	}
	else
	{
		uint32_t i = DFT_CERO_U32;
		Node* node = FIRST(list);

		while((node) && (i < (pos-1))) //Mientras el valor este en la lista y no sea nulo
		{
			i++;
			node = getNextNode(node);
		}
		
		addNewNode(node, createNodeDeepCopy(data, function));		
	}

	increaseCounter(list);

	return;
}

/* EXTRAE UN ELEMENTO */
void* popElem(LinkedList* list)
{
	decreaseCounter(list);
	void *data = NULL;

	if(getNElems(list) == 0)
	{
		data = deleteNode(LAST(list));
		LAST(list) = NULL;
		FIRST(list) = NULL;
	}
	else
	{
		LAST(list) = PREVIOUS(LAST(list));
		data = deleteNode((NEXT(LAST(list))));
	}
	
	return data;
}

/* EXTRAE UN ELEMENTO EN LA POSICION INDICADA */
void* popElemIndex(LinkedList* list, uint32_t posElem)
{
	decreaseCounter(list);
	uint32_t i = DFT_CERO_U32;
	Node* node = NULL;
	for(node = FIRST(list); i < (posElem); i++, node = getNextNode(node));

	if(posElem == 0)
		FIRST(list) = NEXT(FIRST(list));
	if(posElem == getNElems(list))
		LAST(list) = PREVIOUS(LAST(list));
	
	return deleteNode(node);
}

/* COMPRUEBA SI ESTA VACIA */
bool isEmpty(LinkedList* list)
{
	if(getNElems(list) == DFT_CERO_U32)
		return TRUE;

	return FALSE;
}

/* DEVUELVE UN ELEMENTO DE LA LISTA */
void* getLinkedListElem(LinkedList* list, uint32_t position)
{
	uint32_t i = DFT_CERO_U32;
	Node* n = FIRST(list);
	for(; i < position; i++, n = getNextNode(n));

	return getDataNode(n);
}

/* INTERCAMBIA LOS NODOS */
void swapLinkedListNodes(LinkedList* list, uint32_t swapPosition1, uint32_t swapPosition2)
{
	uint32_t i = 0;
	Node* n1 = FIRST(list);
	Node* n2 = FIRST(list);

	for(;i<swapPosition1;i++,n1=NEXT(n1));
	for(i=0;i<swapPosition2;i++,n2=NEXT(n2));

	PREVIOUS(NEXT(n2)) = n1;
	NEXT(PREVIOUS(n2)) = n1;
	PREVIOUS(NEXT(n1)) = n2;
	NEXT(PREVIOUS(n1)) = n2;

	Node* n = PREVIOUS(n2);
	PREVIOUS(n2) = PREVIOUS(n1);
	PREVIOUS(n1) = n;

	n = NEXT(n2);
	NEXT(n2) = NEXT(n1);
	NEXT(n1) = n;
	
	return;
}

/* INTERCAMBIA EL CONTENIDO DE LOS NODOS */
void swapLinkedListValueNodes(LinkedList* list, uint32_t swapPosition1, uint32_t swapPosition2)
{
	uint32_t i = 0;
	Node* n1 = FIRST(list);
	Node* n2 = FIRST(list);

	for(;i<swapPosition1;i++,n1=NEXT(n1));
	for(i=0;i<swapPosition2;i++,n2=NEXT(n2));

	void* p = DATA(n1);
	DATA(n1) = DATA(n2);
	DATA(n2) = p;
	
	return;
}

/* CREA UNA LISTA CON PUNTEROS SHALLOW, NO DEEP */
LinkedList* duplicateLinkedList(LinkedList* list)
{
	LinkedList* listNew = createLinkedList();
	uint32_t i = 0;	

	for(; i < getNElems(list); i++){
		void* value = getLinkedListElem(list,i);
		appendElem(listNew, value);
	}
	
	return listNew;
}

LinkedList* duplicateDeepCopyLinkedList(LinkedList* list, userCopyFunction function)
{
	LinkedList* listNew = createLinkedList();
	uint32_t i = 0;	

	for(; i < getNElems(list); i++){
		void* value = getLinkedListElem(list,i);
		appendElemDeepCopy(listNew, value, function);
	}
	
	return listNew;
}

/* ELIMINA LA LISTA */
void deleteLinkedList(LinkedList* list)
{
	Node* n = FIRST(list);
	Node* n2 = NEXT(FIRST(list));	

	while(n2)
	{
		freeNodeLinkedList(n);
		n = n2;
		n2 = NEXT(n2);
	}	

	freeNodeLinkedList(n);

	free(INFO(list));
	INFO(list) = NULL;
	free(list);
	list = NULL;
}

void deleteDeepCopyLinkedList(LinkedList* list)
{
	Node* n = FIRST(list);
	Node* n2 = NEXT(FIRST(list));	

	while(n2)
	{
		freeDeepCopyNodeLinkedList(n);
		n = n2;
		n2 = NEXT(n2);
	}	

	freeDeepCopyNodeLinkedList(n);

	free(INFO(list));
	INFO(list) = NULL;
	free(list);
	list = NULL;
}

/* CAMBIA LA POSICION DE UN NODO */
void changePositionNodeLinkedList(LinkedList* list, uint32_t nodePosition, uint32_t newPosition)
{
	uint32_t i = 0;
	Node* n1 = FIRST(list);
	Node* n2 = FIRST(list);

	for(;i<nodePosition;i++,n1=NEXT(n1));
	for(i=0;i<newPosition;i++,n2=NEXT(n2));
        
    if(NEXT(n1) == NULL)
        LAST(list) = n2;
        
    if(PREVIOUS(n1) == NULL)
        FIRST(list) = n2;
        
    if(NEXT(n2) != NULL)
        PREVIOUS(NEXT(n2)) = PREVIOUS(n2);
    
    if(PREVIOUS(n2) != NULL)
        NEXT(PREVIOUS(n2)) = NEXT(n2);
    
     if(PREVIOUS(n1) != NULL)
        NEXT(PREVIOUS(n1)) = n2;
    
    PREVIOUS(n2) = PREVIOUS(n1);
    PREVIOUS(n1) = n2;
    
    NEXT(n2) = n1;
        
    return;
}

void* deepCopyInt32Value(void* data)
{
	int32_t* value = (int32_t*)malloc(sizeof(int32_t));
	*value = *(int32_t *)data;

	return value;
}

void* deepCopyUInt8Value(void* data)
{
	uint8_t* value = (uint8_t*)malloc(sizeof(uint8_t));
	*value = *(uint8_t *)data;

	printf("deepCopyUInt8Value> valor a copiar: %d\n", *(uint8_t *)value);

	return value;
}

uint32_t getUInt32ArrayWithListElements(LinkedList* list, uint32_t* array)
{
	array = (uint32_t*)malloc(*(uint32_t *)INFO(list)*sizeof(uint32_t));
	if(!array)
		return 0xFFFFFFFF;

	uint32_t pos = 0;
	for(Node* n = FIRST(list); !n; n = NEXT(n), pos++)
	{
		*(array+pos) = *(uint32_t*)DATA(n);
	}

	return *(uint32_t*)INFO(list);
}

uint8_t getUInt8ArrayWithListElements(LinkedList* list, uint8_t** array)
{
	printf("Total de elementos INFO(list): %d\n",*(uint8_t *)INFO(list));
	*array = (uint8_t*)malloc(*(uint8_t *)INFO(list)*sizeof(uint8_t));
	if(!*array)
		return 0xFF;

	uint8_t* aux_array = *array;

	printf("FIRST(list): %p, %d, %p\n",FIRST(list), *(uint8_t*)DATA(list), NEXT(list));
	uint8_t pos = 0;
	for(Node* n = FIRST(list); n; n = NEXT(n), pos++)
	{
		printf("Valor de pos: %d\n",pos);
		*(aux_array + pos) = *(uint8_t*)DATA(n);
		//*(*(array+pos)) = *(uint8_t*)DATA(n);
		printf("Valor de DATA(n): %d\n",*(uint8_t*)DATA(n));
		printf("Valor de *(array+pos): %d\n",*(aux_array+pos));
		
	}

	return *(uint8_t*)INFO(list);
}
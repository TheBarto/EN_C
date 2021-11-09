#include "../headers/Stack.h"

Stack* createStack()
{
	return createLinkedList();
}

void push(Stack* stack, void* data)
{
	appendElem(stack, data);
}

void pushDeepCopy(Stack* stack, void* data, userCopyFunction function)
{

#ifdef DEBUG_GRADO3
	printf("pushDeepCopy > Valor a guardar: %d\n",*(uint8_t *)data);
#endif

	appendElemDeepCopy(stack, data, function);
}

void* pop(Stack* stack)
{
	return popElem(stack);
}

bool isEmptyStack(Stack* stack)
{
	return isEmpty(stack);
}

Stack* duplicateStack(Stack* stack)
{
	return duplicateLinkedList(stack);
}

Stack* duplicateDeepCopyStack(Stack* stack, userCopyFunction function)
{
	return duplicateDeepCopyLinkedList(stack, function);
}

void deleteStack(Stack* stack)
{
	return deleteLinkedList(stack);
}

void deleteDeepCopyStack(Stack* stack)
{
	return deleteDeepCopyLinkedList(stack);
}

uint32_t getUInt32ArrayWithStackElements(Stack* stack, uint32_t* array)
{
	return getUInt32ArrayWithListElements(stack, array);
}

uint8_t getUInt8ArrayWithStackElements(Stack* stack, uint8_t** array)
{

	uint8_t total = getUInt8ArrayWithListElements(stack, array);

#ifdef DEBUG_GRADO3
	printf("getUInt8ArrayWithStackElements > El total de elementos es: %d\n",total);
	for(int i = 0; i < total; i++)
		printf("%d ", array[0][i]);

	printf("\n");
#endif

	return total;
}

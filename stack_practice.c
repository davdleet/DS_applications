#include <stdio.h>
#include <stdlib.h>


typedef struct {
	int key;
}element;
void stackFull();
element stackEmpty();

element *stack;
int top = -1;
int capacity = 1;
void push(element item)
{
	if (top >= capacity - 1)
		stackFull();
	stack[++top] = item;
}

element pop()
{
	if (top == -1)
		return stackEmpty();
	return stack[top--];
}

void stackFull()
{
	stack = (element*)realloc(stack, sizeof(element) * 2 * capacity);
	capacity *= 2;
}

element stackEmpty()
{
	fprintf(stderr, "Stack is empty, cannot pop element");
	return * stack;
}

int main(void)
{
	stack = (element*)malloc(sizeof(element) * capacity);
	int i,j;
	element elone;
	elone.key = 0;
	while (elone.key >= 0)
	{
		printf("enter a key to add: ");
		scanf_s("%d", &elone.key);
		push(elone);
	}
	pop();
	for (i = 0; i < 1; i++)
	{
		printf("current stack: \n");
		for (j = 0; j < top + 1; j++)
		{
			printf("%d ", stack[j].key);
		}
		printf("\n");
	}
	pop();
	for (i = 0; i < 1; i++)
	{
		printf("current stack: \n");
		for (j = 0; j < top + 1; j++)
		{
			printf("%d ", stack[j].key);
		}
		printf("\n");
	}
	//free(stack);
}
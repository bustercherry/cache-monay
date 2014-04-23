#include "lru.h"
#include "stdlib.h"
#include "stdio.h"

int main(int argv, char** argc)
{
	lru_t* lru = init_lru(10);
	int i;
	
	for(i = 0; i < 50; i++)
	{
		append_data(lru, i);
	}
	
	print_lru(lru);
	printf("%d \n", lru->current_size);
	
	
	for(i = 0; i < 60; i++)
	{
		remove_head(lru);
	}
	
	print_lru(lru);
	
	printf("%d \n", lru->current_size);
	printf("Contains 20: %d \n", contains(lru, 20));
	printf("Contains 1230: %d \n", contains(lru, 1230));

	destroy_lru(lru);
	
	return 0;
}


void print_lru(lru_t* lru)
{
	node_t* current = lru->head;
	
	while(current != NULL)
	{
		printf("%Lx ", current->value);
		current = (node_t*) current->next;
	}
	
	printf("\n");
}

node_t* init_node(unsigned long long value)
{
	node_t* node = malloc(sizeof *node);
	
	node->value = value;
	node->next = NULL;
	
	return node;
}

lru_t* init_lru(int max_size)
{
	lru_t* lru = (lru_t*) malloc(sizeof *lru);
	lru->head = NULL;
	lru->max_size = max_size;
	lru->current_size = 0;
	
	return lru;
}

void destroy_lru(lru_t* lru)
{
	node_t* current = lru->head;
	
	while(current != NULL)
	{
		free(current);
		current = current->next;
	}
	
	free(current);
	free(lru);
}

void append_data(lru_t* lru, unsigned long long data)
{
	node_t* new_node = init_node(data);
	node_t* current = lru->head;
	
	if(contains(lru, data)) 
		return;
	
	if(lru->head == NULL)
	{
		lru->head = new_node;
		lru->current_size++;
	}
	else
	{
		while(current != NULL)
		{
			if(current->next == NULL)
			{
				current->next = new_node;
				lru->current_size++;
				return;
			}
			else
			{
				current = current->next;
			}
		}
	}
	
}

void append_node(lru_t* lru, node_t* node)
{
	node_t* current = lru->head;
	
	if(contains(lru, node->value)) 
		return;
	
	if(lru->head == NULL)
	{
		lru->head = node;
		lru->current_size++;
	}
	else
	{
		while(current != NULL)
		{
			if(current->next == NULL)
			{
				current->next = node;
				lru->current_size++;
				return;
			}
			else
			{
				current = current->next;
			}
		}
	}
}

node_t clone_node(node_t* node)
{
	node_t new_node;
	
	if(node != NULL)
	{
		new_node.value = node->value;
		new_node.next = node->next;
		
		free(node);
	}	
	
	return new_node;
}

node_t remove_head(lru_t* lru)
{
	node_t current = clone_node(lru->head);	
	
	if(lru->head != NULL) 
	{
		lru->current_size--;
		lru->head = lru->head->next;
	}
	
	return current;
}

int contains(lru_t* lru, int value)
{
	node_t* current = lru->head;
	
	while(current != NULL)
	{
		if(current->value == value)
		{
			return 1;
		}
		else
		{
			current = current->next;
		}
	}
	
	return 0;
}

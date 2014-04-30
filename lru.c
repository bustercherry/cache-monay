#include "lru.h"
#include "stdlib.h"
#include "stdio.h"

/*
int main(void)
{
	lru_t* lru = init_lru(10);
	int i;
	
	append_data(lru, 0);
  append_data(lru, 1);
  append_data(lru, 2);
  append_data(lru, 3);
  append_data(lru, 1);
  append_data(lru, 1);
  append_data(lru, 0);
  append_data(lru, 2);
  
	print_lru(lru);
	printf("Current size: %d \n", lru->current_size);
	printf("Contains 20: %d \n", contains(lru, 20));
	printf("Contains 1230: %d \n", contains(lru, 1230));

	destroy_lru(lru);
	
	return 0;
}
*/

void print_lru(lru_t* lru)
{
	node_t* current = lru->head;
  
	while(current != NULL)
	{
		printf("%d ", current->value);
		current = current->next;
	}
	
	printf("\n");
}

node_t* init_node(int value)
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

void append_data(lru_t* lru, int data)
{
	node_t* new_node = init_node(data);
	node_t* current = lru->head;
	
	int ret = contains(lru, data);
  if(ret != -1)
  {
    remove_node(lru, ret);
  }
	
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

int remove_head(lru_t* lru)
{
  int value = 0;
	
	if(lru->head != NULL) 
	{
    value = lru->head->value;
		lru->current_size--;
		lru->head = lru->head->next;
	}
	
	return value;
}

int contains(lru_t* lru, int value)
{
	node_t* current = lru->head;
  int numTimes = 0;
	
	while(current != NULL)
	{
		if(current->value == value)
		{
			return numTimes;
		}
		else
		{
			current = current->next;
      numTimes++;
		}
	}
	
	return -1;
}

void remove_node(lru_t* lru, int offset)
{
    node_t* current = lru->head;
    int numTimes = 0;
    
    if(offset == 0)
    {
      remove_head(lru);
      return;
    }
    
    while(current != NULL)
    {
      if(numTimes == offset - 1)
      {
        current->next = current->next->next;
        lru->current_size--;
        break;
      }
      else
      {
        current = current->next;
        numTimes++;
      }
      
    }
    
}


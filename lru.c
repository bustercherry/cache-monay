#include "lru.h"
#include "stdlib.h"
#include "stdio.h"

/*
int main(void)
{
	lru_t* lru = init_lru(10);
  
	push_data(lru, 0);
  push_data(lru, 1);
  push_data(lru, 2);
  push_data(lru, 3);
  push_data(lru, 0);
  push_data(lru, 2);
  push_data(lru, 2);
  push_data(lru, 3);
  push_data(lru, 0);
  push_data(lru, 1);
  push_data(lru, 0);
  
  
	printf("Current size: %d \n", lru->current_size);
	printf("Contains 20: %d \n", contains(lru, 20));
	printf("Contains 1230: %d \n", contains(lru, 1230));

	destroy_lru(lru);
	
	return 0;
}
*/


void print_lru(lru_t* lru)
{
	node_t* current = lru->head->next;
  
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
	lru->head = init_node(0xFFFFFFFF);
	lru->max_size = max_size;
	lru->current_size = 0;
	
	return lru;
}

void destroy_lru(lru_t* lru)
{
	node_t* temp;
	
	while(lru->head->next != NULL)
	{
    temp = lru->head->next;
		lru->head->next = lru->head->next->next;
    free(temp);
	}
	
	free(lru);
}

void push_data(lru_t* lru, int value)
{
  node_t* current = lru->head->next;
  node_t* prev = lru->head;
  
  while(current != NULL)
  {
    if(current->value == value)
    {
      prev->next = current->next;
      current->next = lru->head->next;
      lru->head->next = current;
      
      return;
    }
    
    prev = current;
    current = current->next;
    
  }
  
  node_t* new_node = init_node(value);
  
  new_node->next = lru->head->next;
  lru->head->next = new_node;
  lru->current_size++;
  
}

int find_least(lru_t* lru)
{
  node_t* current = lru->head;
  
  while(current->next != NULL)
  {
    current = current->next;
  }
	
	return current->value;
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
      //remove_head(lru);
      return;
    }
    
    while(current != NULL)
    {
      if(numTimes == offset - 1)
      {
        node_t *temp = current->next;
        current->next = current->next->next;
        lru->current_size--;
        free(temp);
        break;
      }
      else
      {
        current = current->next;
        numTimes++;
      }
      
    }
    
}


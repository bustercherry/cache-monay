/***************************
 * File: lru.h
 * Created by: Aaron Mertz
 * 4/17/14
 ***************************/
 
 typedef struct node_t node_t;
 
 struct node_t
 {
	 int value;
	 node_t* next;
 };
 
 
 typedef struct
 {
	 node_t* head;
	 int max_size;
	 int current_size;
	 
 } lru_t;

/********************************************************************
 * Takes a given lru pointer and prints it element by element, using
 * %Lx as the printf format.
 ********************************************************************/
void print_lru(lru_t* lru);

/********************************************************************
 * Returns a pointer to a node that is constructed using the given
 * data. The node's 'next' element is left as NULL.
 ********************************************************************/
node_t* init_node(int data);

/********************************************************************
 * Initializes an lru with a max_size. The lru head is set to NULL,
 * and current_size is set to 0.
 ********************************************************************/
lru_t* init_lru(int max_size);

/********************************************************************
 * Loops through all nodes in an lru and frees them, before freeing
 * the lru itself.
 ********************************************************************/
void destroy_lru(lru_t* lru);

/********************************************************************
 * Appends the given data to the lru by constructing a new node and 
 * updating the lru head pointer.
 ********************************************************************/
void push_data(lru_t* lru, int value);

/********************************************************************
 * Removes and returns the head of the lru in order to determine which
 * block should be replaced next.
 ********************************************************************/
int find_least(lru_t* lru);

/********************************************************************
 * Loops through the lru and checks to see if it contains the given
 * value. Returns 0 if the value isn't found and 1 if the value is
 * found.
 ********************************************************************/
int contains(lru_t* lru, int value);

/**********************************************************************
 * Removes a node at the given offset.
 *********************************************************************/
void remove_node(lru_t* lru, int offset);


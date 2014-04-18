/***************************
 * File: lru.h
 * Created by: Aaron Mertz
 * 4/17/14
 ***************************/
 
 
 typedef struct
 {
	 unsigned long long value;
	 struct node_t* next;
	 
 } node_t;
 
 
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
node_t* init_node(unsigned long long data);


/********************************************************************
 * Exactly clones a given node pointer and returns just a node.
 * This is used to get the value of the head while still being able to
 * free it, instead of freeing before the value can be read.
 ********************************************************************/
node_t clone_node(node_t* node);

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
void append_data(lru_t* lru, unsigned long long data);

/********************************************************************
 * Appends the given node to the lru by updating the lru head pointer.
 ********************************************************************/
void append_node(lru_t* lru, node_t* node);

/********************************************************************
 * Removes and returns the head of the lru in order to determine which
 * block should be replaced next.
 ********************************************************************/
node_t remove_head(lru_t* lru);

/********************************************************************
 * Loops through the lru and checks to see if it contains the given
 * value. Returns 0 if the value isn't found and 1 if the value is
 * found.
 ********************************************************************/
int contains(lru_t* lru, int value);


# data_structures_C
Important and useful (frequently used) datastructures written in C

## stack
Just your normal stack.

`stack *initialize_stack()` - Initializes the stack and returns a struct pointer with typedefed type "stack"

`void push(void *object, stack *stack)` - Pushes "object" as void pointer onto the given stack

`void *pop(stack *stack)` - Returns the last element (LIFO) as a void pointer

`void *peek_stack(stack *stack)` - Returns the last element (LIFO) as a void pointer without removing it from the stack

`char is_empty_stack(stack *stack)` - Returns a 1 if the stack is empty, 0 otherwise (careful!)

Note that the "stack" type is actually just a wrapper that contains a pointer to the actual stack, to permit using more than one in your script. Also note that popping from an empty stack results in a NULL pointer, the program won't immediately crash, but you should check if you didn't check for emptiness.

## queue
Just your normal queue.

`queue *initialize_queue()` - Initializes the queue and returns a struct pointer with the typedefed type "queue"

`void enqueue(void *object, queue *queue)` - Enqueues "object" as void pointer in the queue

`void *dequeue(queue *queue)` - Returns the first element (FIFO) as a void pointer

`void *peek_queue(queue *queue)` - Returns the first element (FIFO) as a void pointer without removing it from the queue

`char is_empty_queue(queue *queue)` - Returns a 1 if the queue is empty, 0 otherwise (careful!)

Note that the "queue" type is actually just a wrapper that contains a pointer to the actual stack, to permit using more than one in your script. Also note that dequeueing from an empty queue results in a NULL pointer, the program won't immediately crash, but you should check if you didn't check for emptiness.

## hash
A simple and highly customizable hashmap

The hash uses a simple overflow avoidance mechanism by simply saving pointers of a linked list in its array to minimize lookup time.

Since a hashmap, especially in C, has to serve very different needs, this is why this file is very customizable. First of all, you can edit the source file to your needs, if you need a bigger hashmap or just want more uniform spread. Furthermore, the current hash function is nothing more than a better placeholder, so if you ever want to handle strings as well, you would need to update it as well. Lastly, the actual hashmap node can be edited as well, to better suit the individual needs, for example char pointer or another struct.

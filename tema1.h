// Resuli Rrapo 321CD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MR "MOVE_RIGHT"
#define ML "MOVE_LEFT"
#define SR "SEARCH_RIGHT"
#define SL "SEARCH_LEFT"
#define WRITE_X "WRITE"
#define INSL "INSERT_LEFT"
#define INSR "INSERT_RIGHT"
#define SHOW "SHOW"
#define SHOW_C "SHOW_CURRENT"
#define SWITCH "SWITCH"
#define SEARCH "SEARCH"
#define CC "CLEAR_CELL"
#define CA "CLEAR_ALL"
#define EXECUTE "EXECUTE"


typedef struct node{
    char value;
    struct node* prev;
    struct node* next;
} Node;

typedef struct Train {
	Node* sant; //head of the list
    Node* current;  //current position of the mechanic
} Train;

typedef struct QueueNode{
	char* elem;
	struct QueueNode *next;
} QueueNode;

typedef struct Queue{
	QueueNode *front;
	QueueNode *rear;
	long size;
}Queue;

// function to create a new queue
Queue* createQueue(){
	Queue * coada = (Queue *) malloc(sizeof(Queue));
	coada->front = NULL;
	coada->rear = NULL;
	coada->size = 0;
	return coada;
} 

// function to check if the queue is empty
int isQueueEmpty(Queue *q){
	if (q->rear == NULL)
		return 1;
	return 0;
}

// function to add a new element to the queue
void enqueue(Queue *q, char* command){
	QueueNode* node = (QueueNode *) malloc(sizeof(QueueNode));
	node->next = NULL;
	if(isQueueEmpty(q)) {

		q->rear = node;
		q->front = node;

	} else {

		q->rear->next = node;
		q->rear = node;

	}
	q->rear->elem = (char *) malloc(20*sizeof(char));
	strcpy(node->elem,command);
	node->elem[strlen(command) - 1] = '\0';
	q->size++;
}

// function to remove an element from the queue
void dequeue(Queue* q){
	if (isQueueEmpty(q)) {
		return;
	}
		QueueNode* temp = q->front;

		// if the queue has only one element
		if (q->front == q->rear) {
			free(q->rear->elem);
			free(q->rear);
			q->front = q->rear = NULL;
		} else 
		{
			// if the queue has more than one element
			q->front = q->front->next;
			free(temp->elem);
			free(temp);
		}
		q->size--;
}

// function to free the queue
void destroyQueue(Queue *q){
	while (isQueueEmpty(q)) {
		dequeue(q);
	}
	free(q);
}

// function to initialize the train
Train* init_train() {
    Train  *train = (Train  *)malloc(sizeof(struct Train));
	train->sant = (Node *) malloc(sizeof(Node));
    train->sant->prev = NULL;

	train->current = (Node *) malloc(sizeof(Node));
    train->current->value = '#';
    train->current->prev = train->sant;
    train->current->next = NULL;
    train->sant->next = train->current;

    return train;
}

// function to free the train
void freetrain(Train* train) {
	Node* temp = train->sant;
	Node* iter = train->sant;
	while (iter != NULL) {
		temp = iter;
		iter = iter->next;
		free(temp);
	}
	free(train);
}
// move the mechanic to the left
int move_mechanic_left(Train* mechanic) {
    if (mechanic->current != mechanic->sant->next) {
        mechanic->current = mechanic->current->prev;
	}
		while (mechanic->current == mechanic->sant->next) {
			break;
		}
}

// move the mechanic to the right
void move_mechanic_right(Train* mechanic) {
    if (mechanic->current->next == NULL) {
        mechanic->current->next = (Node *) malloc(sizeof(Node));
        mechanic->current->next->prev = mechanic->current;
        mechanic->current->next->next = NULL;
		mechanic->current->next->value = '#';
    }
    mechanic->current = mechanic->current->next;
}

// clear the current vagon
void clear_cell(Train* train) {
	// return to initial state if there is only one vagon
    if (train->current == train->sant->next && train->current->next == NULL) {
        free(train->current);
        train->current = train->sant;
        train->sant->next = NULL;
        return;
    }

    Node* temp= train->current;

    // check if the current is the first vagon
    if (train->current == train->sant->next) {
        while (train->current->next != NULL) {
            train->current = train->current->next;
        }
    } else {
        train->current = train->current->prev;
    }
    // reconnect the previous and next vagons
    if (temp->prev) {
        temp->prev->next = temp->next;
    }
    if (temp->next) {
        temp->next->prev = temp->prev;
    }
    free(temp);
}

// clear all the vagons except the first one
void clear_all(Train* train) {
    Node* current = train->sant->next;

    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    
    // reset the train to its initial state
    train->current = (Node *) malloc(sizeof(Node));
    train->current->value = '#';
    train->current->prev = train->sant;
    train->current->next = NULL;
    train->sant->next = train->current;
}

// search for a string in the train
void search(Train* train, char* str, FILE* fp) {
    Node* start = train->current;
    Node* temp = start;
    int found = 0;

    do {
        Node* checker = temp; // node to check the string
        char* s = str; // pointer to iterate through the string
        found = 1;

        // check if the characters match the string
        while (*s != '\0') {
            if (checker == NULL || checker->value != *s) {
                found = 0;
                break;
            }
            checker = checker->next;
            s++; // move to the next character in the string

			// if we meet the end , connec twith the first vagon
            if (checker == NULL) {
                checker = train->sant->next;
            }
        }

        if (found) {
            train->current = temp;
            break;
        }

        // move to the next carriage and connect with the first vagon if we reach the end
        temp = temp->next;
        if (temp == NULL) {
            temp = train->sant->next;
        }

    } while (temp != start); // end the search if we re back at the start

    if (!found) {
        fprintf(fp, "ERROR\n");
    }
}

// search for a string to the left of the current vagon
void search_left(Train* train, char* str, FILE* fp) {
    Node* temp = train->current;
	char* s = str;
    int found = 0;

    while (temp != train->sant->next) {
        if (temp->value == *str) {
			found = 1;
			*s++;
			while (*s != '\0')
			{
				// move to the previous vagon
				temp = temp->prev;
				if (temp->value != *s) {
					found = 0;
					break;
				}
				*s++;
			}
            break;
        }
        temp = temp->prev;
    }

    if (found) {
        train->current = temp;
    } else {
        fprintf(fp, "ERROR\n");
    }
}

// search for a string in the right direction
void search_right(Train* train, char* str, FILE* fp) {
    Node* temp = train->current;
	char* s = str;
    int found = 0;

    while (temp != train->sant->prev) {
        if (temp->value == *str) {
			found = 1;
			*s++;
			while (*s != '\0')
			{
				// move to teh next vagon
				temp = temp->next;
				if (temp->value != *s) {
					found = 0;
					break;
				}
				*s++;
			}
            break;
        }
        temp = temp->next;
    }

    if (found) {
        train->current = temp;
    } else {
        fprintf(fp, "ERROR\n");
    }
}

// write a character to the current vagon
void write_char(Train* mechanic, char value) {
    mechanic->current->value = value;
}

// insert a vagon to the left of the current vagon
void insert_left(Train* mechanic, char value, FILE* fp) {
	if(mechanic->current->prev == mechanic->sant) {
			fprintf(fp,"ERROR\n");
	} else {
		// create a new node
		Node* newnode2 = (Node* )malloc(sizeof(Node));
		newnode2->next = mechanic->current;
		newnode2->prev = mechanic->current->prev;

		// update the mechanic's position
		mechanic->current->prev->next = newnode2;
		mechanic->current->prev = newnode2;
		newnode2->value = value;
		mechanic->current = mechanic->current->prev;
	}
}

void insert_right(Train* mechanic, char value) {

	Node* temp = mechanic->current;
    Node* newnode3 = (Node*) malloc(sizeof(Node));

	// insert if we are not at the end of the train
	if (temp->next != NULL) {
		newnode3->next = temp->next;
		temp->next = newnode3;
		temp->next->next->prev = newnode3;
		newnode3->prev = temp;
	} 
	// insert if we are at the end of the train
	else if (temp->next == mechanic->sant) {
		temp->next = newnode3;
		newnode3->prev = temp;
		newnode3->next = mechanic->sant;
	} 
	// insert if we are at the beginning of the train
	else {
		newnode3->next = NULL;
		newnode3->prev = temp;
		temp->next = newnode3;
	}
	newnode3->value = value;
	mechanic->current = newnode3;
}

void show_current(Train* train, FILE *fp) {
	fprintf(fp,"%c\n", train->current->value);
}


void show(Train* train, FILE *fp) {
    Node* temp = train->sant->next;
    while (temp != NULL) {
        if (temp == train->current) {
            fprintf(fp,"|%c|", temp->value);
        } else {
            fprintf(fp,"%c", temp->value);
        }
        temp = temp->next;
    }
    fprintf(fp,"\n");
}

void Switch(Queue* q) {
    if (q == NULL || q->front == NULL || q->front == q->rear) {
        return;
    }

    QueueNode* prev = NULL;
    QueueNode* current = q->front;
    QueueNode* next = NULL;
    q->rear = q->front; // the front will now be the rear

    // traverse the list and reverse the direction of each link
    while (current != NULL) {
        next = current->next; // store next node
        current->next = prev; // reverse current node's pointer
        prev = current; // move pointers one position ahead for next iteration
        current = next;
    }

    q->front = prev; // the last node processed becomes the front
}
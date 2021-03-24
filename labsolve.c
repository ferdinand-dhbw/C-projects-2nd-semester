#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#define MAXX 255
#define MAXY 255

//structs
typedef struct labyrinth {
	char lab[MAXX+1][MAXY];
	bool visited[MAXX][MAXY];
	int start[2];
	int treasure[2];
} LabCell, *Lab_p;

struct path {
	int point[2];
	struct path* next;
};
typedef struct path Path, *Path_p;

struct queue {
	int point[2];
	struct queue* next;
	Path_p path;
};
typedef struct queue Queue, *Queue_p;

//functions
Lab_p readLab(FILE*);
Lab_p checkLab(Lab_p);
void setCoords(int*, int, int);
void printLab(Lab_p);
void solveLab(Lab_p);
Queue_p push(Queue_p, int, int, Path_p);
Path_p pushPath(Path_p, int, int);
Queue_p drop(Queue_p);
bool notVisitedAndAccessible(Lab_p, int, int);
void addSolution(Lab_p, Path_p);
Path_p duplicatePath(Path_p);


int main(int argc, char* argv[]) {
	FILE* in = stdin;
	if (argc > 2) {
		fprintf(stderr, "Usage: %s [<file>]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	else if (argc == 2) {
		fopen_s(&in, argv[1], "r");
		if (!in) {
			perror(argv[0]);
			exit(EXIT_FAILURE);
		}
	}
	Lab_p lab;
	if (!(lab = checkLab(readLab(in)))) exit(EXIT_FAILURE); //reads and checks the lab.
	solveLab(lab);
	printLab(lab);
	free(lab);
	exit(EXIT_SUCCESS);
}

Lab_p readLab(FILE* file) {
	//reads the lab
	int count = 0;
	Lab_p p = malloc(sizeof(LabCell)); //allocate mem for struct
	setCoords(&p->start[0], -1, -1);
	setCoords(&p->treasure[0], -1, -1);

	while (fgets(p->lab[count], MAXX, file) && count < MAXY) { //read all lines line by line and stay under the limit
		printf(p->lab[count]);
		for (int i = 0; i < MAXY; i++) {
			// set X and S
			if (p->lab[count][i] == 'S') {
				if (p->start[0] != -1) return NULL; //second S is catched
				setCoords(&(p->start[0]), count, i);
			}
			else if (p->lab[count][i] == 'X') {
				if (p->treasure[0] != -1) return NULL; //second X
				setCoords(&(p->treasure[0]), count, i);
			}
		}
		count++;
	}
	if(count < MAXX) p->lab[count][0] = 'T'; //as a terminator

	for (int i = 0; i < MAXX; i++) //set visited matrix to false
		for (int j = 0; j < MAXY; j++)
			p->visited[i][j] = false;
	return p;
}

Lab_p checkLab(Lab_p lab) {
	//checks if lab is valid
	//return 0 if not.
	if (!lab) return NULL; //in prevous function smth went wrong
	if (lab->start[0] == -1 || lab->treasure[0] == -1) return NULL;
	return lab;
}

void setCoords(int* t, int x, int y) {
	*t = x;
	t++;
	*t = y;
}

void printLab(Lab_p lab) {
	printf("\n");
	for (int x = 0; x < MAXY; x++) {
		if (lab->lab[x][0] == 'T') break;
		printf("%s", lab->lab[x]);
	}
	printf("\n");
}

void solveLab(Lab_p lab) {
	//Dijsktra
	//push S to queue
	Queue_p q=push(NULL, lab->start[0], lab->start[1], NULL);
	Path_p p = NULL;

	while (q) { //while queue is not empty
		p = duplicatePath(q->path);//kopieren und nicht verlinken!
		//get current point
		int x = q->point[0];
		int y = q->point[1];
		//set visited
		lab->visited[x][y] = true;

		if (x == lab->treasure[0] && y == lab->treasure[1]) break; //termination criterion; fastet path is first element of queue;
		//drop
		q = drop(q);

		if (notVisitedAndAccessible(lab, x + 1, y))//right
			q=push(q, x + 1, y, pushPath(duplicatePath(p), x, y));
		if (notVisitedAndAccessible(lab, x, y + 1))//up
			q = push(q, x, y + 1, pushPath(duplicatePath(p), x, y));
		if (notVisitedAndAccessible(lab, x - 1, y))//left
			q=push(q, x - 1, y, pushPath(duplicatePath(p), x, y));
		if (notVisitedAndAccessible(lab, x, y - 1))//down
			q=push(q, x, y - 1, pushPath(duplicatePath(p), x, y));
		if (p) free(p); //free p
	}
	if (!q) {
		printf("There's no solution!\n");
	}
	else {
		addSolution(lab, q->path);
	}
	while (q=drop(q)); //free queue;
}

Queue_p push(Queue_p queue, int x, int y, Path_p path) {
	//pushes element on the queue
	//create element
	Queue_p new = malloc(sizeof(Queue));
	setCoords(&new->point, x, y);
	new->path = path;
	new->next = NULL;

	//add to queue
	if (!queue) { //if queue is empty
		return new;
	}
	else {
		Queue_p current = queue;
		while (current->next) {
			current = current->next;
		}	//go to last element
		current->next = new;
		return queue;
	}
}

Path_p pushPath(Path_p path, int x, int y) {
	//pushes the path to go to a element 
	//create element

	Path_p new = malloc(sizeof(Path));
	if(new) setCoords(&new->point, x, y);
	new->next = NULL;

	//add to queue
	if (!path) { //if queue is empty
		return new;
	}
	else {
		Path_p current = path;
		while (current->next) {
			current = current->next;
		}//go to last element
		current->next = new;
		return path;
	}
}

Queue_p drop(Queue_p queue_ptr) {
	//drops an element from queue and frees path
	if (!queue_ptr) return NULL;
	Queue_p ret = queue_ptr->next;

	Path_p current = queue_ptr->path, waste; //free the whole path
	while (current) {
		waste = current;
		current = current->next;
		free(waste);
	}
	free(queue_ptr); //finally free the element
	return ret; //return the rest
}

bool notVisitedAndAccessible(Lab_p lab, int x, int y) {
	return (lab->lab[x][y] != '#' && !(lab->visited[x][y]));
}

void addSolution(Lab_p lab, Path_p path) {
	//adds a solution to the lab
	int x, y;
	while (path) {
		x = path->point[0];
		y = path->point[1];
		if (lab->lab[x][y] != 'S' && lab->lab[x][y] != 'X') lab->lab[x][y] = '.';
		path = path->next;
	}
}

Path_p duplicatePath(Path_p p) {
	//dublicates a whole path (singly linked lists); return value is the new list
	if (!p) return NULL; //empty path results in an empty path

	Path_p new, current, first; //new will be the new created, copied element
								//current will be the current element in the new list
								//first is the first element and will be given back

	first = malloc(sizeof(Path));
	setCoords(&first->point, p->point[0], p->point[1]);
	first->next = NULL;
	current = first;
	p = p->next;

	while (p) {
		//copy one element
		new = malloc(sizeof(Path));
		setCoords(&new->point, p->point[0], p->point[1]);
		new->next = NULL;

		current->next = new; //link it

		//go to the next one
		current = current->next;
		p = p->next;
	}
	return first;
}

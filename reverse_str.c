#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXITEMS 10000
#define MAXL 255

struct data { //objects for stack
	char str[MAXL];
	struct data * prev;
};

int main(int argc, char* argv[])
{
	struct data *p=NULL, *pre=NULL, *d;
	char* temp[MAXL];
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

	//read each line and push into stack
	int count = 0;
	while (fgets(temp, MAXL, in) && count <= MAXITEMS) { //read all lines line by line and stay under the limit
		p = malloc(sizeof(struct data)); //allocate mem for struct
		if (p) { //malloc may not have worked
			strcpy_s(&p->str, MAXL, temp); //cpy str 
			p->prev = pre; //link to previous
			pre = p; //set next previous
		}
		count++; //count the number of items
	}

	if (in!= stdin) fclose(in); //close file

	int lb = 1; //linebreak
	while (p) { // while stack is not empty
		printf("%s", p->str); //print str

		d = p;
		p = p->prev; //go to prev
		free(d); //free mem
	}

	exit(EXIT_SUCCESS);
}
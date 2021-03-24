#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<string.h>

//structs
struct treeElement {
	int key;
	char value[32]; //C: int a[x]; / Java: int[] a; \n a = new int[x];
	struct treeElement * left, * right;
};
typedef struct treeElement Element, * Element_ptr, ** Element_ptr_ptr;

//function prototypes
bool insert(Element_ptr_ptr, int, char*);
Element_ptr insert_rec(Element_ptr, int, char*);
Element_ptr create(int, char*);
void print(Element_ptr);
Element_ptr find(Element_ptr, int);
void printUsage(bool);

//functions
int main() {
	Element_ptr tree = NULL;
	printf("Binary Search Tree - CLI by Ferdinand Koenig.\nType \"H:\" for help.\n");
	/*
	insert(&tree, 7, "Sieben");
	insert(&tree, 6, "Sechs");
	insert(&tree, 5, "Fuenf");
	insert(&tree, 11, "Elf");
	*/

	char buffer[64];
	while (1) {
		printf("\n-tree>");
		fgets(buffer, 64, stdin);

		if (!(buffer[1] == ':') && !(buffer[2]==' ')) {
			printUsage(0);
		}
		else {
			char * value, *rest, * key, *next, *next2;
			Element_ptr result;

			switch (buffer[0]) {
				case 'I':
				case 'i':
					next = next2 = NULL;
					key = strtok_s(buffer, ",", &next);
					rest = strtok_s(NULL, ",", &next);
					value = strtok_s(rest, " ", &next2);
					value[strlen(value) - 1] = '\0';
					insert(&tree, atoi((char *) (key + 3)), value);
					break;

				case 'P':
				case 'p':
					print(tree);
					break;

				case 'F':
				case 'f':
					next = NULL;
					key = strtok_s(buffer, " ", &next);
					key = strtok_s(NULL, " ", &next);
					if (result = find(tree, atoi(key))) {
						printf("Found: (%d, %s)\n", result->key, result->value);		
					}
					else
						printf("Not found\n");
					
					break;

				case 'E':
				case 'e':
					exit(0);

				case 'H':
				case 'h':
					printUsage(1);
					break;

				default:
					printUsage(0);
					break;
			}
		}
	}
}


bool insert(Element_ptr_ptr tree_ptr, int key, char* Value) {
	*tree_ptr=insert_rec(*tree_ptr, key, Value);
	return true;
}

Element_ptr insert_rec(Element_ptr tree, int key, char* Value) {
	if (!tree) return create(key, Value);
	if (key < tree->key) {
		tree->left = insert_rec(tree->left, key, Value);
	}
	else if (key > tree->key){
		tree->right = insert_rec(tree->right, key, Value);
	}
	else {
		Element_ptr newElement = create(key, Value);
		newElement->left = tree->left;
		newElement->right = tree->right;
		free(tree);
		return newElement;
	}
	return tree;
}

Element_ptr create(int key, char* Value) {
	Element_ptr p = malloc(sizeof(Element)); //allocate mem for struct
	if (!p) exit(1);
	p->key = key;
	strcpy_s(&p->value, 32, Value);
	p->right = p->left = NULL;
	return p;
}

void print(Element_ptr tree) {
	//In-Order
	if (!tree) return;
	print(tree->left);
	printf("(%d, %s)\n", tree->key, tree->value);
	print(tree->right);
}

Element_ptr find(Element_ptr tree, int key) {
	if (!tree) return NULL;
	/*if (tree->key == key) {
		return tree;
	}
	else */if (key < tree->key) {
		return find(tree->left, key);
	}
	else if (key > tree->key) {
		return find(tree->right, key);
	}
	return tree;
}

void printUsage(bool extended) {
	printf("Usage:\n[I/P/F/E/H]: <args, comma separated>\n");
	if (extended) {
		printf("\nI(nsert): <key: int>, <value: String>\n");
		printf("P(rint):\n");
		printf("F(ind): <key: int>,\n");
		printf("E(xit):\n");
		printf("H(elp):\n");
	}
}
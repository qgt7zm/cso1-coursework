/*
 * Eric Weng
 * CS 2130 HW 9
 * Tue 12/6/22
 * Postfix Calculator
 */

#include <ctype.h> // for checking whitespace
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Struct Definitions */

// A growable list that stores characters
typedef struct {
	int capacity;	// max allowed size
	int size;		// number of elements
	char* arr;		// array of strings
} char_list;

// A growable list that stores strings
typedef struct {
	int capacity;	// max allowed size
	int size;		// number of elements
	char** arr;		// array of strings
} str_list;


/* String Methods */

// Construct a string on the heap with a given length
char* createString(int strLen) {
	return (char*) calloc(strLen, sizeof(char));
}

/* List Methods */

// Construct a new list on the heap
char_list* createCharList(int startCapacity) {
	char_list* list = (char_list*) malloc(sizeof(char_list));
	list->capacity = startCapacity;
	list->size = 0;
	list->arr = createString(startCapacity);
	return list;
}

str_list* createStrList(int startCapacity) {
	str_list* list = (str_list*) malloc(sizeof(str_list));
	list->capacity = startCapacity;
	list->size = 0;
	list->arr = (char**) calloc(startCapacity, sizeof(char*));
	return list;
}

// Print all elements in a list
void printCharList(char_list* list) {
	if (!list || !(list->arr)) return;
	printf("[ ");
	for (int i = 0; i < list->size; i++) {
		if (i == list->size - 1) printf("%c", list->arr[i]);
		else printf("%c, ", list->arr[i]);
	}
	puts(" ]");
}

void printStrList(str_list* list) {
	if (!list || !(list->arr)) return;
	printf("[ ");
	for (int i = 0; i < list->size; i++) {
		if (i == list->size - 1) printf("%s", list->arr[i]);
		else printf("%s, ", list->arr[i]);
	}
	puts(" ]");
}

// Add an element to the end of a list
void pushStr(str_list* list, char* elem) {
	if (list->size >= list->capacity) { // Double capacity if size will exceed capacity
		list->capacity *= 2;
		list->arr = (char**) realloc(list->arr, sizeof(char*) * list->capacity);
	}
	list->arr[(list->size)++] = elem;
}

// Add many elements to the end of a list
void pushChars(char_list* list, char* elems) {
	int len = strlen(elems);
	if (list->size + len >= list->capacity) { // Double capacity if new size exceeds capacity
		list->capacity *= 2;
		list->arr = (char*) realloc(list->arr, sizeof(char) * list->capacity); 
	}
	strncpy(list->arr + list->size, elems, len);
	list->size += len;
}

// Remove and return many elements from the front of a list
char* pollChars(char_list* list, int len) { 
	// pop elements
	char* elems = createString(len + 1);
	strncpy(elems, list->arr, len);
	// shift elements down
	char* copy = createString(list->size);
	strcpy(copy, list->arr + len);
	strcpy(list->arr, copy);
	free(copy);
	// decrease list size
	list->size -= len;
	if (list->size < 0) list->size = 0; // don't let size get negative
	return elems;
}

// Clear all elements from the list

void clearChar(char_list* list) {
	memset(list->arr, '\0', list->size * sizeof(char));
	list->size = 0;
}

void clearStr(str_list* list) {
	for (int i = 0; i < list->size; i++) free(list->arr[i]); // free stored strings
	memset(list->arr, 0, list->size * sizeof(char*));
	list->size = 0;
}

/* Main Function */

int main(int argc, const char* argv[]) {

	// Create initial list
	const int startCap = 8; // initial list capacity
	str_list* calcList = createStrList(startCap);
	char_list* inputBuff = createCharList(startCap);
	
	if (argc > 1) { // Read input from CL
		for (int i = 1; i < argc; i++) {
			int len = strlen(argv[i]);
			char* str = createString(len + 1);
			printf("%d: %s\n", i, argv[i]);
			strncpy(str, argv[i], len);
			pushStr(calcList, str);
		}
	} else { // Read input from terminal
		while (!feof(stdin)) { // use CTRL-D for EOF
			// Add next line to input buffer
			char* line = createString(startCap); // new string for each read
			fgets(line, startCap, stdin);
			pushChars(inputBuff, line);
			free(line);
			// printCharList(inputBuff);

			// Divide input into tokens
			char* input = inputBuff->arr;
			for (int i = 0; input[i]; i++) { // track token length
				if (isspace(input[i])) { // end of token
					// Construct token
					char* token = createString(i + 1);
					strncpy(token, input, i);
					if (i > 0) pushStr(calcList, token); // Don't push empty strings
					pollChars(inputBuff, i + 1);
					i = 0; // reset token length
				}
			}
		}
		// Add anything left over
		char* input = inputBuff->arr;
		int tokenLen = strlen(inputBuff->arr);
		char* token = createString(tokenLen + 1);
		strncpy(token, input, tokenLen);
		if (tokenLen > 0) pushStr(calcList, token); // Don't push empty strings
		pollChars(inputBuff, tokenLen + 1);
	}
	printStrList(calcList);

	// Free resources
	clearChar(inputBuff);
	free(inputBuff->arr);
	free(inputBuff);

	clearStr(calcList);
	free(calcList->arr);
	free(calcList);
	
	return 0;
}
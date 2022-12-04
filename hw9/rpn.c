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

int streq(const char* str1, const char* str2) {
	return !strcmp(str1, str2);
}

int strisnumber(char* str) {
	if (!str) return 0;
	for (int i = 0; str[i] != '\0'; i++)
		if (str[i] != '-' && !isdigit(str[i])) return 0;
	return 1;
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

// Remove and return the last element in a list
char* popStr(str_list* list) {
	char* elem = list->arr[list->size - 1];
	list->arr[--list->size] = NULL;
	return elem;
}

// Remove and return many elements from the front of a list
char* pollChars(char_list* list, int count) { 
	// pop elements
	char* elems = createString(count + 1);
	strncpy(elems, list->arr, count);
	// shift elements down
	char* rest = createString(list->size + 1);
	strncpy(rest, list->arr + count, list->size);
	strcpy(list->arr, rest);
	memset(list->arr + (list->size - count), '\0', count * sizeof(char));
	free(rest);
	// decrease list size
	list->size -= count;
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

// Free a list created on the heap
void freeCharList(char_list* list) {
	clearChar(list);
	free(list->arr);
	free(list);
}

void freeStrList(str_list* list) {
	clearStr(list);
	free(list->arr);
	free(list);
}

/* Calc Functions */

// Add a token to the calculator and perform operations
int addToCalc(str_list* calc, char* token, int len) {
	if (len > 0) { // Don't push empty strings
		int op = 0; // no op

		if (streq(token, "+")) op = 1; // set operation
		else if (streq(token, "-")) op = 2;
		else if (streq(token, "*")) op = 3;
		else if (streq(token, "/")) op = 4;
		else if (strisnumber(token)) pushStr(calc, token); // add number to stack
		else return 0; // unrecognized token

		if (op) {
			// printf("%s\n", token);
			// get operands
			int size = calc->size;
			int i1 = size - 2;
			int i2 = size - 1;
			char* op1 = (i1 < 0) ? NULL : calc->arr[i1];
			char* op2 = (i2 < 0) ? NULL : calc->arr[i2];
			if (!strisnumber(op1) || !strisnumber(op2)) return 0; // no enough operands
			int num1 = atoi(op1);
			int num2 = atoi(op2);
			// printf("%d %s %d\n", num1, token, num2);

			// do math
			int result = 0;
			if (op == 1) result = num1 + num2;
			else if (op == 2) result = num1 - num2;
			else if (op == 3) result = num1 * num2;
			else if (op == 4) result = num1 / num2;
			// printf("%d\n", num1);
			// printf("%d\n", num2); 
			// printf("%d\n", result);

			// convert result to string
			int len = snprintf(NULL, 0, "%d", result);
			char* str = createString(len + 1);
			snprintf(str, len + 1, "%d", result);

			// update stack
			free(popStr(calc));
			free(popStr(calc));
			pushStr(calc, str);
		}
		// printStrList(calc);
		// return 1;
	}
	printStrList(calc);
	return 1;
}

/* Main Method */

int main(int argc, const char* argv[]) {
	// Create initial list
	const int startCap = 64; // initial list capacity
	str_list* calcList = createStrList(startCap);
	char_list* inputBuff = createCharList(startCap);
	
	if (argc > 1) { // Read input from CL
		for (int i = 1; i < argc; i++) {
			int len = strlen(argv[i]);
			char* token = createString(len + 1);
			strncpy(token, argv[i], len);
			if (!addToCalc(calcList, token, len)) {
				printStrList(calcList);
				freeCharList(inputBuff);
				freeStrList(calcList);
				return 0;
			}
		}
	} else { // Read input from terminal
		char* input = inputBuff->arr;
		int offset = 0; // where to start reading

		while (!feof(stdin)) { // use CTRL-D for EOF
			input = inputBuff->arr;
			// Add next line to input buffer
			char* line = createString(startCap); // need new string for each read
			fgets(line, startCap, stdin);
			pushChars(inputBuff, line);
			free(line);
			// printCharList(inputBuff);

			// Divide input into tokens
			int start = offset;
			int end = start;

			int isSpace = isspace(input[0]); // looking at whitespace
			for (int i = offset; input[i] && i < inputBuff->size; i++) {
				// printf("'%c'\n", input[i]);
				if (isSpace && !isspace(input[i])) { // switching to text
					start = i; // start tracking length
					isSpace = !isSpace;
				} else if (!isSpace && isspace(input[i])) { // switching to space
					end = i; // not inclusive
					isSpace = !isSpace;

					// create new token
					int len = end - start;
					char* token = createString(len + 1);
					strncpy(token, input + start, len);
					// printf("(%d, %d)\n", start, end);
					// printf("token = '%s'\n", token);

					if (!addToCalc(calcList, token, len)) {
						printStrList(calcList);
						freeCharList(inputBuff);
						freeStrList(calcList);
						return 0;
					}
				}
				offset++;
			}
			free(pollChars(inputBuff, offset - 1));
			offset = 0;
			// printCharList(inputBuff);
		}
		// Add anything left over
		// offset--;
		// printCharList(inputBuff);
		printf("%s\n", inputBuff->arr);
		int len = inputBuff->size - offset;
		char* token = createString(len + 1);
		strncpy(token, input + offset, len);
		if (addToCalc(calcList, token, len)) {
			// free(pollChars(inputBuff, len + 1));
		} else {
			printStrList(calcList);
			freeCharList(inputBuff);
			freeStrList(calcList);
			return 0;
		}
	}
	// printStrList(calcList);

	// Free resources
	freeCharList(inputBuff);
	freeStrList(calcList);
	
	return 0;
}
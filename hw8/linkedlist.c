/*
 * Eric Weng
 * CS 2130 HW 8
 * Tue 12/6/22
 * Linked List
 */

#include <stddef.h> // needed for "NULL"
#include <stdio.h>
#include <stdlib.h> // needed for malloc()
#include "linkedlist.h"

ll_node *ll_head(ll_node *list){
    // Keep looking left until the previous node is null
    if (!list) return NULL;
    ll_node *curr = list;
    while (curr->prev) curr = curr->prev;
    return curr; 
}


ll_node *ll_tail(ll_node *list) {
    // Keep looking right until the next node is null
    if (!list) return NULL;
    ll_node *curr = list;
    while (curr->next) curr = curr->next;
    return curr; 
}


unsigned long ll_length(ll_node *list) {
    // Loop through the list left to right and count each node
    long length = 0l;
    ll_node *curr = ll_head(list);
    while (curr) {
        length++;
        curr = curr->next;
    }
    return length; 
}

ll_node *ll_find(ll_node *list, int value) {
    // Loop through the list left to right until reaching the value
    ll_node *curr = ll_head(list);
    while (curr) {
        if (curr->value == value) return curr;
        if (curr == list) break;
        curr = curr->next;
    }
    return NULL; 
}


ll_node *ll_remove(ll_node *list) {
    if (!list) return NULL;
    ll_node *prev = list->prev;
    ll_node *next = list->next;
    // reconnect nodes
    if (prev) prev->next = next;
    if (next) next->prev = prev;
    free(list); // delete node
    return next; // shift nodes down
}


ll_node *ll_insert(int value, ll_node *list, int before) {  
    // create new node
    ll_node *curr = (ll_node*) malloc(sizeof(ll_node));
    curr->value = value;

    // add node to list
    if (!list) { // create new list
        curr->prev = NULL;
        curr->next = NULL;
    } else { // insert in existing list
        if (before) { // before node
            ll_node *prev = list->prev;
            ll_node *next = list;
            // connect new node
            curr->prev = prev;
            curr->next = next;
            // reconnect nodes
            if (prev) prev->next = curr;
            if (next) next->prev = curr;
        } else { // after node
            ll_node *prev = list;
            ll_node *next = list->next;
            // connect new node
            curr->prev = prev;
            curr->next = next;
            // reconnect nodes
            if (prev) prev->next = curr;
            if (next) next->prev = curr;
        }
    }
    return curr; 
}


/**
 * Displays the contents of the list separated by commas and surrounded by
 * brackets, with the pointed-to node highlighted with asterisks.
 *
 * Reference solution provided with assignment description
 */
void ll_show(ll_node *list) {
    // Find the head of the linked list
    ll_node *ptr = ll_head(list);
    // Print opening [
    putchar('[');
    // Loop through list printing values
    while(ptr) {
        if (ptr->prev) 
            printf(", "); // include a comma if not the first element
        if (ptr == list) 
            putchar('*'); // include * surrounding parameter element
        printf("%d", ptr->value);
        if (ptr == list) 
            putchar('*');
        ptr = ptr->next; // next pointer
    }
    // Print closing ]
    puts("]");
}


/**
 * Debugging display: shows the address and all the fields of the node,
 * optionally with the nodes before (if b is true) and after (if f is true).
 *
 * Written by CSO1 course staff and made available to all students.
 */
void ll_dump(ll_node *list, int f, int b) {
    if (b && list->prev) 
        ll_dump(list->prev, 0, 1);
    printf("%p: %p\t[%d]\t%p\n", list, list->prev, list->value, list->next);
    if (f && list->next) 
        ll_dump(list->next, 1, 0);
}


int main(int argc, const char *argv[]) {
    // Example debugging code.  You should write other
    // tests, including testing corner cases.
    ll_node *x = NULL;
    // list insertion
    putchar('A'); ll_show(x);
    x = ll_insert(25, x, 1);
    putchar('B'); ll_show(x);
    x = ll_insert(1, x, 0);
    putchar('C'); ll_show(x);
    x = ll_insert(98, x, 1);
    putchar('D'); ll_show(x);
    x = ll_insert(0, x, 1);
    putchar('E'); ll_show(x);
    x = ll_insert(-8, ll_tail(x), 0);
    putchar('F'); ll_show(x);

    // list get head/tail
    ll_node *y = ll_head(x);
    putchar('G'); ll_show(y);
    ll_node *z = ll_tail(x);
    putchar('G'); ll_show(z);

    // list length
    printf("Length: %lu (or %lu)\n", ll_length(y), ll_length(x));

    // list remove
    x = ll_remove(x);
    putchar('H'); ll_show(x);
    putchar('I'); ll_show(y);

    // list find
    x = ll_remove(ll_find(ll_tail(y), 98));
    putchar('J'); ll_show(x);
    putchar('K'); ll_show(y);

    // nulls
    ll_node *a = NULL;
    a = ll_head(NULL);
    ll_show(a);
    a = ll_tail(NULL);
    ll_show(a);
    printf("%lu\n", ll_length(a));
    a = ll_remove(a);
    ll_show(a);

    // finding
    ll_node *nums = ll_insert(1, NULL, 0);
    nums = ll_insert(2, nums, 0);
    nums = ll_insert(3, nums, 0);
    nums = ll_insert(4, nums, 0);
    nums = ll_insert(5, nums, 0);
    ll_show(nums);
    nums = ll_tail(nums);
    putchar('0'); ll_show(ll_find(nums, 0));
    putchar('1'); ll_show(ll_find(nums, 1));
    putchar('2'); ll_show(ll_find(nums, 2));
    putchar('3'); ll_show(ll_find(nums, 3));
    putchar('4'); ll_show(ll_find(nums, 4));
    putchar('5'); ll_show(ll_find(nums, 5));
    putchar('1'); ll_show(ll_find(ll_head(nums), 1));
    putchar('2'); ll_show(ll_find(ll_head(nums), 2));

    return 0;
}

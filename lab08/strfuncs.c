
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Finds the length of a string
size_t mystrlen(const char* s);
char* simple_split(char* s, char delim);

int main() {
    const char* s1 = "even elephants exfoliate";
    size_t slen = mystrlen(s1);
    puts(s1 + (slen/2));
    
    puts("");
    
    char *trash, *split, *s2;
    trash = split = s2 = strdup("can all aardvarks quaff?");
    do {
        s2 = split;
        split = simple_split(s2, 'a');
        puts(s2);
    } while(split);
    free(trash);

    return 0;
}

// size_t is returned from sizeof and depends on the platform
size_t mystrlen(const char* s) {
    size_t len = 0;
    // while (*(s + len)) len++; // get next char* and dereference
    while (s[len]) len++; // just index the string and check each character 
    return len;
}

char* simple_split(char* s, char delim) {
    if (!s || !s[0]) return NULL; // null input or first char is null
    
    // call mystrlen instead of checking char
    for (int i = 0; i < mystrlen(s); i++) { // loop through string
        if (s[i] == delim) {
            s[i] = '\0';
            return &s[i + 1]; // return next char*
        }
    }
    return NULL;
}

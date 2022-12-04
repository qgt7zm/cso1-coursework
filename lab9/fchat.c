#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char buffer[4096]; // message, 2^12 chars = 4kB
const int buffer_size = 4096;

char filename[128]; // filename
const int filename_size = 128;

void prune(char* str) { // Prune newline character
    for (int i = 0; str[i]; i++) {
        if (str[i] == '\n') {
            str[i] = 0;
            break;
        }
    }
}

int main() {
    // Get user ID
    char* user = getenv("USER");
    printf("Hello there, %s\n", user);

    // Open own inbox
    snprintf(filename, filename_size, "/bigtemp/cso1-f22/%s.chat", user);
    FILE* in = fopen(filename, "r");

    // Print new messages
    puts("Inbox:");
    char* line = fgets(buffer, buffer_size, in);
    int messages = 0;
    while (line && line[0] != '\n') {
        messages++;
        printf(">> %s", line);
        line = fgets(buffer, buffer_size, in);
    }
    if (!messages) puts("You have no new messages");
    printf("Who would you like to message? ");
    truncate(filename, 0); // Clear messages

    // Ask for recipient
    line = fgets(buffer, buffer_size, stdin);
    if (!line || line[0] == '\n') { // Exit if empty
        puts("Closing inbox");
        return 0;  
    }
    prune(line);

    // Open outbox file
    char filename[100]; 
    snprintf(filename, 100, "/bigtemp/cso1-f22/%s.chat", line);
    FILE* out = fopen(filename, "a");

    // Ask for message 
    printf("What would you like to send? ");
    fgets(buffer, buffer_size, stdin);
    if(buffer[0] != '\n') {
        // Write the message
        prune(buffer);
        fprintf(out, "%s: %s\n", user, buffer);
        puts("Message sent");
    } else {
        puts("Closing inbox");
    }    

    return 0;
}


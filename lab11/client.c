#include <stdio.h>	
#include <stdlib.h>
#include <string.h> // memset
#include <unistd.h>

#include <sys/socket.h> // define socket functions
#include <arpa/inet.h> // define in_port_t and in_addr_t types
#include <netinet/in.h> // define sockaddr_in struct

// Starter Code: https://www.cs.virginia.edu/~jh2jf/courses/cs2130/fall2022/labs/lab11-sockets.html

int main(int argc , char *argv[]){
    // Create Socket
    int connection = socket(AF_INET, SOCK_STREAM, 0);
    if (connection == -1) { // Check return value for error
        puts("An error occured creating the socket.");
        return -1;
    }

    // Get the IP and Port
    if(argc < 3) {
        puts("Error: An IP address and port are required.");
        puts("Usage: client <ip> <port>");
    }
    char* ip = argv[1];
    int port = atoi(argv[2]);
    printf("Connecting to address %s:%d...\n", ip, port);

    // Create Client IP Instance
    struct sockaddr_in serverIP;
    memset(&serverIP, 0, sizeof(struct sockaddr_in));
    serverIP.sin_family = AF_INET; // IPv4 protocol
    serverIP.sin_addr.s_addr = inet_addr(ip); // server IP address
    serverIP.sin_port = htons(port); // given port
    
    // Connect to Server
    int status = connect(connection, (struct sockaddr*) &serverIP , sizeof(serverIP));
    if (status == -1) {
        printf("Error: Could not connect to address %s:%d...\n", ip, port);
        puts("Make sure that your IP is four numbers connected by periods and your port is a number.");
        puts("Check that the server is also running.");
        return -1;
    }
    puts("Connected to server.");

    // Read Messages from Server
    char msg[64];
    int len;
    while ((len = read(connection, msg, 64))) {
        if (len == -1) { // Shouldn't happen if invalid IP
            printf("Error: Could not read from address %s:%d...\n", ip, port);
            puts("Check that you entered a valid address and that the server is running.");
            return -1;
        }
        // Need to set last character as '0'
        msg[len] = '\0';
        printf("%s", msg);
    }
    
    // Close Socket and free anything created on heap.
    puts("Disconnected from server.");
    close(connection);
    return 0;

}

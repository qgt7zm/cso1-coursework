#include <stdio.h>	
#include <stdlib.h>
#include <string.h> // memset
#include <unistd.h>

#include <sys/socket.h> // define socket functions
#include <arpa/inet.h> // define in_port_t and in_addr_t types
#include <netinet/in.h>

// Starter Code: https://www.cs.virginia.edu/~jh2jf/courses/cs2130/fall2022/labs/lab11-sockets.html

int main(int argc , char *argv[]){
    // Create Socket
    int client = socket(AF_INET, SOCK_STREAM, 0);
    if (client == -1) { // Check return value for error
        puts("An error occured creating the socket.");
        return -1;
    }
    // printf("Socket ID = %d\n", client);

    // Get the IP and Port
    if(argc < 3) {
        puts("Error: An IP address and port are required.");
        puts("Usage: client <ip> <port>");
    }
    char* ip = argv[1];
    int port = atoi(argv[2]);
    printf("Connecting to address %s:%d...\n", ip, port);

    // Create client sockaddr_in instance
    struct sockaddr_in clientIP;
    memset(&clientIP, 0, sizeof(struct sockaddr_in));
    clientIP.sin_family = AF_INET; // IPv4 protocol
    clientIP.sin_addr.s_addr = inet_addr(ip); // server IP address
    clientIP.sin_port = htons(port); // given port

    // Bind the Socket
    bind(client, (struct sockaddr*) &clientIP , sizeof(clientIP));
    
    // Connect to Server
    listen(client, 20);
    int comm = accept(client, 0, 0); // communication socket
    if (comm == -1) { // Check return value for error
        printf("Error: Could not connect to address %s:%d...\n", ip, port);
        puts("Make sure your IP is four numbers separated by periods and your port is a number");
        return -1;
    }
    
    
    // Read Message from Server. (Note: The server sends multiple messages, so you might need a loop)
    printf("Connected to server %s:%d...\n", ip, port);
    
    
    // Close Socket and free anything created on heap.
    close(comm);
    close(client);

}

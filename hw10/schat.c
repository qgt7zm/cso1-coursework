#include <stdio.h>	
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <poll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

// Starter Code: https://www.cs.virginia.edu/~jh2jf/courses/cs2130/fall2022/labs/lab11-sockets.html

const char *msg = "Congratulations, you've successfully received a message from the server!\n";
const int timeout = 60000;
const int msg_size = 4096;

int runServer() {
    puts("Starting server");
    srandom(getpid());
    int port = 0xc000 | (random()&0x3fff); // choose random port between 49152–65535
    
    // Create IPv4 Address
    struct sockaddr_in serverIP;
    memset(&serverIP, 0, sizeof(struct sockaddr_in));
    serverIP.sin_family = AF_INET;
    // serverIP.sin_addr.s_addr = htonl(INADDR_ANY);
    serverIP.sin_addr.s_addr = inet_addr("127.0.0.1"); // localhost
    serverIP.sin_port = htons(port);
    
    // Create Socket
    int listener = socket(AF_INET, SOCK_STREAM, 0);
    bind(listener, (struct sockaddr*) &serverIP , sizeof(serverIP));

    // Wait for up to 20 connections
    listen(listener, 20);

    // Display IP and port
    system("host $HOSTNAME");
    printf("The server is now listening on port %d\n", port);

    printf("Waiting for a connection\n");
    // get a connection socket (this call will wait for one to connect)
    int client = accept(listener, (struct sockaddr*)NULL, NULL);
    close(listener);

    poll(&client, 1, timeout);
    // write(client, msg, strlen(msg)); // send a full message

    // Read/Write to client
    char msg[msg_size];
    int len;
    while ((len = read(client, msg, msg_size))) {
        printf("Message: ");
        if (len <= -1) { // Shouldn't happen if invalid IP
            puts("An error occured reading from the server.");
            return -1;
        }
        // Need to set last character as '0'
        msg[len] = '\0';
        printf("%s", msg);
    }

    // Disconnect
    close(client);
    puts("Client disconnected from server.");
    return 0;
}

int runClient(char* ip, int port) {
    printf("Starting client and connecting to address %s:%d.\n", ip, port);

    // Create Socket
    int server = socket(AF_INET, SOCK_STREAM, 0);
    if (server <= -1) { // Check return value for error
        puts("An error occured creating the socket.");
        return -1;
    }

    // Create Client IP Instance
    struct sockaddr_in serverIP;
    memset(&serverIP, 0, sizeof(struct sockaddr_in));
    serverIP.sin_family = AF_INET; // IPv4 protocol
    serverIP.sin_addr.s_addr = inet_addr(ip); // server IP address
    serverIP.sin_port = htons(port); // given port
    
    // Connect to Server
    int status = connect(server, (struct sockaddr*) &serverIP , sizeof(serverIP));
    if (status <= -1) {
        printf("Error: Could not connect to address %s:%d.\n", ip, port);
        puts("Make sure that your IP is four numbers connected by periods and your port is a number.");
        puts("Check that the server is also running.");
        return -1;
    }
    puts("Connected to server.");

    // Read/Write to server
    poll(&server, 1, timeout);

    char msg[msg_size];
    int len;
    while ((len = read(server, msg, msg_size))) {
        if (len <= -1) { // Shouldn't happen if invalid IP
            printf("Error: Could not read from address %s:%d.\n", ip, port);
            puts("Check that you entered a valid address and that the server is running.");
            return -1;
        }
        // printf("Message: ");
        // Need to set last character as '0'
        msg[len] = '\0';
        printf("%s", msg);
    }

    snprintf(msg, msg_size, "Hello there");
    puts(msg);
    write(server, msg, strlen(msg));
    
    // Close Socket and free anything created on heap.
    puts("Disconnected from server.");
    close(server);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc <= 1) { // no args
        return runServer();
    } else { // with args
        // Get the IP and Port
        if(argc < 3) {
            puts("Error: An IP address and port are required.");
            puts("Usage: client <ip> <port>");
            return -1;
        }
        char* ip = argv[1];
        int port = atoi(argv[2]);
        return runClient(ip, port);
    }

    return 0;

}

// Ex 3B: modify the C code to receive as input a message from Fortran (unknown length)
//        and send it through the socket.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zmq.h>
#include <assert.h>

void *context;
void *publisher;

// Function to initialize the socket
void init_socket() {
    context = zmq_ctx_new();

    if (context == NULL) {
        perror("Error in opening ZMQ context");
        exit(EXIT_FAILURE);
    }

    publisher = zmq_socket(context, ZMQ_PUB);

    if (publisher == NULL) {
        perror("Error in opening ZMQ publisher socket");
        zmq_ctx_destroy(context);
        exit(EXIT_FAILURE);
    }

    int rc = zmq_bind(publisher, "tcp://127.0.0.1:5555");
    if (rc != 0) {
        perror("Error in binding ZMQ publisher socket");
        zmq_close(publisher);
        zmq_ctx_destroy(context);
        exit(EXIT_FAILURE);
    }

    printf("Publisher connected to socket and ready to broadcast.\n");
}

// Function to send a message through the socket
void send_message(const char *message) {
    int rc = zmq_send(publisher, message, strlen(message), 0);
    // assert(rc == strlen(message));
}

// Function to close the socket
void close_socket() {
    printf("Communication ended. Closing socket. \n");
    zmq_close(publisher);
    zmq_ctx_destroy(context);
}

int main() {
    // Example usage in C
    init_socket();
    while (1){
    const char *fortran_message = "Hello from Fortran!";
    send_message(fortran_message);
    }
    close_socket();

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zmq.h>
#include <assert.h>
#include <unistd.h>

int main() {
    void *context = zmq_ctx_new();
    int i;

    if (context == NULL) {
        perror("Error in opening ZMQ context");
        return -1; // Return -1 to indicate failure
    }

    void *publisher = zmq_socket(context, ZMQ_PUB);

    if (publisher == NULL) {
        perror("Error in opening ZMQ publisher socket");
        zmq_ctx_destroy(context);
        return -1; // Return -1 to indicate failure
    }

    // using zmq_bind() instead of connect since this will be a long-lived process

    int rc = zmq_bind(publisher, "tcp://127.0.0.1:5555");
    if (rc != 0) {
        perror("Error in binding ZMQ publisher socket");
        zmq_close(publisher);
        zmq_ctx_destroy(context);
        return -1; // Return -1 to indicate failure
    }

    printf("Publisher connected to socket and ready to broadcast.\n");

    while(1) {

        char message[50] = "Hello from C Publisher! ";
        char number[10];
        snprintf(number, sizeof(number), "%d", i);
        strcat(message, number);
        rc = zmq_send(publisher, message, strlen(message), 0);
        assert(rc == strlen(message));
        i++;
        usleep(25000); // Pause for 10 milliseconds 
    }

    printf("Connection ended. Closing socket. \n"); 

    zmq_close(publisher);
    zmq_ctx_destroy(context);

    return 0;
}

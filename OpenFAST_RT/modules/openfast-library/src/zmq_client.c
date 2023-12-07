// ||  ----- ZeroMQ Client (re-adapted from ROSCO) ------ ||
// ||            Real-time interactor for FAST            ||
// || --------------------------------------------------- ||


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <zmq.h>


// void delete_blank_spaces_in_string(char *s)
// {
//     int  i,k=0;
//     for(i=0;s[i];i++)
//     {
//         s[i]=s[i+k];
//         if(s[i]==" "|| s[i]=="\t")
//         {
//         k++;
//         i--;
//         }
//     }
// }


void printString(const char *str) {
    printf("String: ");
    
    // Print each character of the string including the null terminator
    for (int i = 0; str[i] != '\0'; ++i) {
        printf("%c", str[i]);
    }
    printf("\\0");
}

float *zmq_client(const char *socket_address, const char *request) {
    // add null-termination from Fortran strings inputs 

    printf("received request from fortran in C: %s \n\n", request);

    size_t socket_len = strlen(socket_address);
    size_t request_len = strlen(request);

    int send_status_req;
    int send_status_data;
    // Allocate memory for null-terminated strings
    // char *socket_copy = malloc(socket_len + 1);
    // char *request_copy = malloc(request_len + 1);

    // // Copy received strings and null-terminate them
    // strncpy(socket_copy, socket_address, socket_len);
    // socket_copy[socket_len +1] = '\0';

    // strncpy(request_copy, request, request_len);
    // request_copy[request_len +1] = '\0';

    void *context = zmq_ctx_new();

    if (context == NULL) {
        perror("Error in opening ZMQ comm");
        return NULL; // Return NULL to indicate failure
    }

    void *requester = zmq_socket(context, ZMQ_REQ);
    if (requester == NULL) {
        perror("Error in opening ZMQ comm");
        zmq_ctx_destroy(context);
        return NULL; // Return NULL to indicate failure
    }
    printf("Connecting...");

    int rc = zmq_connect(requester, socket_address);

    if (rc != 0) {
        perror("Error in connecting to specified address");
        fprintf(stderr, "Address: %s\n", socket_address);
        zmq_close(requester);
        zmq_ctx_destroy(context);
        return NULL; // Return NULL to indicate failure
    }

    printf("C: Sending request %s to socket... \n", request);
    send_status_req = zmq_send(requester, request, strlen(request), 0);

    if (send_status_req >= 0) {
        printf("C: Request sent successfully.\n");
    } else {
        printf("C: Error sending request: %s\n", zmq_strerror(errno));
    }

    float *received_value = (float *)malloc(sizeof(float));

    int recv_size = zmq_recv(requester, (char *)received_value, sizeof(float), 0);
    if (recv_size == sizeof(float)) {
        printf("C: Received float value: %f\n", *received_value);
        return received_value; // Return pointer to received float data

    } else {
        free(received_value); // Free the allocated memory in case of failure
        printf("C: Error receiving float value\n");
        zmq_close(requester);
        zmq_ctx_destroy(context);

        return NULL;
    }

    zmq_close(requester);
    zmq_ctx_destroy(context);
    free(socket_address);
    free(request);

    return NULL; // Return NULL to indicate failure
}


void *context;
void *publisher;

// // Function to initialize the socket
int init_zmq_socket(const char *socket_address) {
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

    int rc = zmq_bind(publisher, socket_address);
    if (rc != 0) {
        perror("Error in binding ZMQ publisher socket");
        zmq_close(publisher);
        zmq_ctx_destroy(context);
        exit(EXIT_FAILURE);
    }

    printf("Publisher connected to socket and ready to broadcast.\n");
    return 1; // Success 
}

// // Function to send a message through the socket
// void send_message(const char *message) {
//     int rc = zmq_send(publisher, message, strlen(message), 0);
//     // assert(rc == strlen(message));
// }

// // Function to close the socket
// void close_socket() {
//     printf("Communication ended. Closing socket. \n");
//     zmq_close(publisher);
//     zmq_ctx_destroy(context);
// }

// int main() {
//     // Example usage in C
//     init_socket();
//     while (1){
//     const char *fortran_message = "Hello from Fortran!";
//     send_message(fortran_message);
//     }
//     close_socket();

//     return 0;
// }

// ||  ----- ZeroMQ Client (re-adapted from ROSCO) ------ ||
// ||            Real-time interactor for FAST            ||
// || --------------------------------------------------- ||


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <zmq.h>


void delete_blank_spaces_in_string(char *s)
{
    int  i,k=0;
    for(i=0;s[i];i++)
    {
        s[i]=s[i+k];
        if(s[i]==" "|| s[i]=="\t")
        {
        k++;
        i--;
        }
    }
}


void printString(const char *str) {
    printf("String: ");
    
    // Print each character of the string including the null terminator
    for (int i = 0; str[i] != '\0'; ++i) {
        printf("%c", str[i]);
    }
    printf("\\0");
}

char* zmq_client(const char *socket_address, const char *request, const char *data) {
    // add null-termination from Fortran strings inputs 
    size_t socket_len = strlen(socket_address);
    size_t request_len = strlen(request);
    size_t data_len = strlen(data);

    int send_status_req;
    int send_status_data;
    // Allocate memory for null-terminated strings
    char *socket_copy = malloc(socket_len + 1);
    char *request_copy = malloc(request_len + 1);
    char *data_copy = malloc(data_len + 1);

    // Copy received strings and null-terminate them
    strncpy(socket_copy, socket_address, socket_len);
    socket_copy[socket_len +1] = '\0';

    strncpy(request_copy, request, request_len);
    request_copy[request_len +1] = '\0';

    strncpy(data_copy, data, data_len);
    data_copy[data_len +1] = '\0';

    printString(socket_copy); 
    printString(data_copy); 
    printString(request_copy);

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

    int rc = zmq_connect(requester, socket_copy);

    if (rc != 0) {
        perror("Error in connecting to specified address");
        fprintf(stderr, "Address: %s\n", socket_copy);
        zmq_close(requester);
        zmq_ctx_destroy(context);
        return NULL; // Return NULL to indicate failure
    }

    printf("Sending request to socket...");
    send_status_req = zmq_send(requester, request_copy, strlen(request_copy), 0);

    if (send_status_req >= 0) {
        printf("Request sent successfully.\n");
    } else {
        printf("Error sending request: %s\n", zmq_strerror(errno));
    }

    // printf("Sending data to socket...");
    // send_status_data = zmq_send(requester, data_copy, strlen(data_copy), 0);

    // if (send_status_data >= 0) {
    //     printf("Data sent successfully.\n");
    // } else {
    //     printf("Error sending data: %s\n", zmq_strerror(errno));
    // }

    char *response_buffer = (char *)malloc(256 * sizeof(char)); // Adjust size as needed
    int recv_size = zmq_recv(requester, response_buffer, 255, 0);
    if (recv_size > 0) {
        response_buffer[recv_size] = '\0';
        printf("Received response: %s\n", response_buffer);

        zmq_close(requester);
        zmq_ctx_destroy(context);

        return response_buffer; // Return pointer to received data
    }

    zmq_close(requester);
    zmq_ctx_destroy(context);
    free(response_buffer); // Free allocated memory in case of failure
    free(socket_copy);
    free(request_copy);
    free(data_copy);

    return NULL; // Return NULL to indicate failure
}


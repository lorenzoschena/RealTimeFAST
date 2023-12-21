// ||  ----- ZeroMQ Client (re-adapted from ROSCO) ------ ||
// ||            Real-time interactor for FAST            ||
// || --------------------------------------------------- ||


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <zmq.h>
#include </home/cJSON/cJSON.h>

void *publisher = NULL;

void printString(const char *str) {
    printf("String: ");
    
    // Print each character of the string including the null terminator
    for (int i = 0; str[i] != '\0'; ++i) {
        printf("%c", str[i]);
    }
    printf("\\0");
}

float *zmq_req_rep(const char *socket_address, const char *request) {
    // add null-termination from Fortran strings inputs 

    // printf("received request from fortran in C: %s \n\n", request);

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
    // printf("Connecting...");

    int rc = zmq_connect(requester, socket_address);

    if (rc != 0) {
        perror("Error in connecting to specified address");
        fprintf(stderr, "Address: %s\n", socket_address);
        zmq_close(requester);
        zmq_ctx_destroy(context);
        return NULL; // Return NULL to indicate failure
    }

    // printf("C: Sending request %s to socket... \n", request);
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

int zmq_init_pub(const char *req_address) {
    void *context = zmq_ctx_new();
    publisher = zmq_socket(context, ZMQ_PUB);

    int rc = zmq_connect(publisher, req_address);

    if (rc != 0) {
        printf("Error binding at %s : %s\n", req_address, zmq_strerror(errno));
        zmq_close(publisher);
        zmq_ctx_destroy(context);
        return -1; // Return error code if binding fails
    }

    printf("Established connection at address %s \n", req_address);
    return 0; // Return success
}

// int zmq_broadcast(const char *message) {
//     if (publisher == NULL) {
//         printf("Socket not initialized. Call zmq_initialize_publisher first.\n");
//         return -1; // Return error if socket is not initialized
//     }

//     size_t msg_len = strlen(message);
//     printf("Sending message: %s\n", message);
//     int rc = zmq_send(publisher, message, msg_len, 0);
//     if (rc < 0) {
//         printf("Error sending message: %s\n", zmq_strerror(errno));
//     }

//     return rc; // Return the result of zmq_send
// }


int zmq_broadcast(const char *data, const char *names) {
    if (publisher == NULL) {
        printf("Socket not initialized. Call zmq_initialize_publisher first.\n");
        return -1; // Return error if socket is not initialized
    }
    printf("inside C broadcasting routine.... \n");

    size_t size = 0;

    printf("Received names: %s\n", names);
    printf("Received string: %s\n", data);

    // // Calculate the size of the array
    // while (data[size] != 0) {
    //     ++size;
    // }

    // printf("Received data of size %zu\n", size);
    // for (size_t i = 0; i < size; ++i) {
    //     printf(" %lf", data[i]);
    // }
    // printf("\n");

    // cJSON *data_array = cJSON_CreateArray();
    // for (size_t i=0; i < size ; i++){
    //     cJSON_AddItemToArray(data_array, cJSON_CreateNumber(data[i]));
    // }

    // cJSON *root = cJSON_CreateObject(); 
    // cJSON_AddStringToObject(root, "names", names); 
    // cJSON_AddItemToObject(root, "data", data_array); 

    // char *json_msg = cJSON_PrintUnformatted(root); 

    // // Send the JSON message
    // int rc = zmq_send(publisher, json_msg, strlen(json_msg), 0);
    // if (rc < 0) {
    //     printf("Error sending message: %s\n", zmq_strerror(errno));
    // }

    // // Free cJSON objects and the JSON message
    // cJSON_Delete(root);
    // free(json_msg);

    return 0; // Success
}

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


// int zmq_client (
//     char *zmq_address,
//     double meas_wspeed[1] // receiving wind speed from out-of-FAST
// )
// {
//     //int num_measurements = 17;  // Number of setpoints and measurements, respectively, and float precision (character length)
//     int char_buffer_size_single = 20; // Char buffer for a single measurement
//     //int char_buffer_size_array = (num_measurements * (char_buffer_size_single + 1));  // Char buffer for full messages to and from ROSCO
//     //char string_to_ssc[char_buffer_size_array];
//     char string_from_ssc[char_buffer_size_single + 1]; // only one variable passed at this stage 
//     int i;
//     int verbose = 0; // Variable to define verbose
    
//     if (verbose == 1) {
//         printf ("Connecting to ZeroMQ server at %s...\n", zmq_address);
//     }
//     // Open connection with ZeroMQ server
//     void *context = zmq_ctx_new ();
//     void *requester = zmq_socket (context, ZMQ_REQ);
//     zmq_connect (requester, zmq_address);  // string_to_zmq is something like "tcp://localhost:5555"

//     // Create a string with measurements to be sent to ZeroMQ server (e.g., Python)
//     // char a[char_buffer_size_array], b[char_buffer_size_single];
//     // sprintf(b, "%.6e", measurements[0]);
//     // strncpy(a, b, char_buffer_size_single);
//     //printf ("zmq_client.c: a[char_buffer_size_single]: measurements[0]: %s\n", a);
//     // int i = 1;
//     // while (i < num_measurements) {
//     //     strcat(a, ",");  // Add a comma
//     //     sprintf(b, "%.6e", measurements[i]);  // Add value
//     //     strcat(a, b);  // Concatenate b to a
//     //     //printf ("zmq_client.c: b[char_buffer_size_single]: measurements[i]: %s\n", b);
//     //     //printf (" --> zmq_client.c: a[char_buffer_size_single]: measurements[i]: %s\n", a);
//     //     i = i + 1;
//     // }
//     // strncpy(string_to_ssc, a, char_buffer_size_array);

//     // Print the string
//     // if (verbose == 1) {
//     //     printf ("zmq_client.c: string_to_ssc: %s…\n", string_to_ssc);
//     // }

//     // Core ZeroMQ communication: receive data and send back signals
//     // zmq_send (requester, string_to_ssc, char_buffer_size_array, 0);

//     zmq_recv (requester, string_from_ssc, char_buffer_size_single + 1, 0);

//     if (verbose == 1) {
//         printf ("zmq_client.c: Received comm: %s\n", string_from_ssc);
//     }

//     // Convert string_from_ssc string to separate floats
//     delete_blank_spaces_in_string(string_from_ssc);
//     char *pt;
//     pt = strtok (string_from_ssc,",");
//     i = 0;
//     while (pt != NULL) {
//         double dtmp = atof(pt);
//         if (verbose == 1) {
//             printf("pt subloop: %s (var), %f (double) \n", pt, dtmp);
//             printf("zmq_client.c: setpoint[%d]: %f \n", i, dtmp);
//         }
//         pt = strtok (NULL, ",");
//         meas_wspeed[i] = dtmp;  // Save values to setpoints
//         i = i + 1;
//     }

//     // Close connection
//     zmq_close (requester);
//     zmq_ctx_destroy (context);
//     return 0;
// }

// int zmq_client(const char *socket_address, const char *request, const char *data, char *response_buffer, size_t buffer_size) {
//     void *context = zmq_ctx_new();

//     if (context == NULL) {
//         perror("Error in opening ZMQ comm");
//         return 0; // Return 0 to indicate failure
//     }

//     void *requester = zmq_socket(context, ZMQ_REQ);
//     if (requester == NULL) {
//         perror("Error in opening ZMQ comm");
//         zmq_ctx_destroy(context);
//         return 0; // Return 0 to indicate failure
//     }

//     int rc = zmq_connect(requester, socket_address);

//     if (rc != 0) {
//         perror("Error in connecting to specified address");
//         fprintf(stderr, "Address: %s\n", socket_address);
//         zmq_close(requester);
//         zmq_ctx_destroy(context);
//         return 0; // Return 0 to indicate failure
//     }
//     printf("Sending request: %s \n", request);
//     zmq_send(requester, request, strlen(request), 0);
//     printf("With accompanying data: %s \n", data);
//     zmq_send(requester, data, strlen(data), 0);

//     int recv_size = zmq_recv(requester, response_buffer, buffer_size - 1, 0);
//     if (recv_size > 0) {
//         response_buffer[recv_size] = '\0';
//         printf("Received response: %s\n", response_buffer);

//         // Close the socket and context
//         zmq_close(requester);
//         zmq_ctx_destroy(context);

//         return 1; // Return 1 to indicate success
//     }

//     zmq_close(requester);
//     zmq_ctx_destroy(context);
//     return 0; // Return 0 to indicate failure
// }

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zmq.h>



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


// int main(int argc, char *argv[]) {
//     if (argc != 4) {
//         fprintf(stderr, "Usage: %s <socket_address> <request> <data>\n", argv[0]);
//         return 1;
//     }

//     const char *socket_address = argv[1];
//     const char *request = argv[2];
//     const char *data = argv[3];

//     char response_buffer[256];
//     int success = zmq_client(socket_address, request, data, response_buffer, sizeof(response_buffer));

//     if (success) {
//         printf("Received response in C: %s\n", response_buffer);
//     } else {
//         printf("No response received in C\n");
//     }

//     return 0;
// }

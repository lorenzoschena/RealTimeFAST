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


void zmq_client(const char *socket_address) { 
    void *context = zmq_ctx_new(); 

    if (context == NULL) {
        perror("Error in opening ZMQ comm");
        return;
    }

    void *requester = zmq_socket(context, ZMQ_REQ);
    if (requester == NULL) {
        perror("Error in opening ZMQ comm");
        zmq_ctx_destroy(context);
    }

    int rc = zmq_connect(requester, socket_address);

    if (rc!=0) {
        perror("Error in connecting to specified address"); 
        fprintf(stderr, "Address: %s\n", socket_address);
        zmq_close(requester); 
        zmq_ctx_destroy(context);
        return; 
    }

    const char *requests[] = {
        "Request 1 from C", 
        "Request 2 from C", 
        "Request 3 from C",
    };

    for (int i = 0; i < sizeof(requests) / sizeof(requests[0]); ++i) {
        zmq_send(requester, requests[i], strlen(requests[i]), 0);
        printf("Sent request: %s\n", requests[i]);
        char response[256];
        int recv_size = zmq_recv(requester, response, sizeof(response) - 1, 0);
        if (recv_size > 0) {
            response[recv_size] = '\0';
            printf("Received response: %s\n", response);
        }
    }

    zmq_close(requester);
    zmq_ctx_destroy(context);
}

int main(int argc, char *argv[]) { //
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <socket_address>\n", argv[0]);
        return 1;
    }

    const char *socket_address = argv[1];
    zmq_client(socket_address);
    return 0;
}
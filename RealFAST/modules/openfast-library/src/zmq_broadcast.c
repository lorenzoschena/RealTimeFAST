#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <zmq.h>


float *zmq_broadcast(const char *req_address, const char *message) 
{
    void *context = zmq_ctx_new();
    void *publisher = zmq_socket(context, ZMQ_SUB);
    size_t msg_len = strlen(message);
    int rc = zmq_bind(publisher, req_address); 

    // assert (rc == 0); 
    rc = zmq_send(publisher, message, msg_len + 1, 0); 
    
    free(req_address); 
    free(message); 

    return rc; 
}
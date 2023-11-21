"""
Simple examples to verify that zmq interacts correctly.
--------------------
Test 2
Wrap Fortran around the C client. Send/receive from Fortran.
"""
import zmq
import subprocess
import time 
import os 
from random import random  
import struct

def launch_zmq_client():
    zmq_client_path = './zmq_req_rep'
    lib_path = './'  # Update with the directory containing zmq_client.so
    os.environ['LD_LIBRARY_PATH'] = lib_path
    proc = subprocess.Popen(zmq_client_path, env=os.environ)
    # _, stderr = proc.communicate()
    # if stderr:
    #     print(f"Error launching Fortran executable: {stderr.decode()}")
    # print("Socket opened successfully")
    pass 
    
def send_messages(socket_address):
    context = zmq.Context()
    socket = context.socket(zmq.REP)
    socket.bind(socket_address)

    try:
        while True: 
            req = socket.recv_string()
            print('Fortran received request: \t {}'.format(req))
            
            if req.lower() == 'wspeed':
                n = random()
                print("Python is sending: \t {}".format(n))
                socket.send(struct.pack('<f', n))
            elif req.lower() == 'rho':
                n = 1.18
                print("Python is sending: \t {}".format(n))
                socket.send(struct.pack('<f', n))
            else: 
                n = 0.00
                print("Python is sending: \t {}".format(n))
                socket.send(struct.pack('<f', n))
                
    except KeyboardInterrupt:
        socket.close()
        context.term()
        

if __name__ == '__main__': 
    address = "tcp://127.0.0.1:5555"

    launch_zmq_client()
    send_messages(address)
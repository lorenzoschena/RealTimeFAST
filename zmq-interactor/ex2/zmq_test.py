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

def launch_zmq_client():
    zmq_client_path = './zmq_fortran_ex2'
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

    messages = [
        "Hello from Python!",
        "Sending messages to zmq_client.",
        "Testing communication."
    ]

    for message in messages:
        req = socket.recv_string()
        print(f"Received request: {req}")
        # data = socket.recv_string()
        # print(f"received data: {data}")
        socket.send_string(message)

    socket.close()
    context.term()

if __name__ == '__main__': 
    address = "tcp://127.0.0.1:5555"

    launch_zmq_client()
    send_messages(address)
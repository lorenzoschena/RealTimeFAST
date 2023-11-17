"""
Simple example to verify that zmq interacts correctly. 
"""
#TODO: I. Establish connection with C. Understand moving parts.
#TODO: Wrap Fortran around the C communicator 
#TODO: Receive from Fortran
#TODO: Insert in InflowWind position 
import os
import zmq
import subprocess

def launch_zmq_client(socket_address):
    # Get the absolute path to the 'src' directory
    zmq_client_path = '../src/zmq_client'
    command = [zmq_client_path, socket_address] #
    subprocess.Popen(command)

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
        
        socket.send_string(message)

    socket.close()
    context.term()

if __name__ == '__main__': 
    address = "tcp://127.0.0.1:5555"

    launch_zmq_client(address)
    send_messages(address)

import subprocess
import zmq
import multiprocessing
import time

def start_c_publisher():
    # Launch the C publisher code as a subprocess
    subprocess.Popen("./zmq_main_program", shell=True)

def subscriber():
    
    context = zmq.Context()
    subscriber_socket = context.socket(zmq.SUB)
    subscriber_socket.connect('tcp://127.0.0.1:5555')
    subscriber_socket.setsockopt(zmq.SUBSCRIBE, b'')  # Subscribe to all messages

    # Capture the start time before receiving messages
    start_time = time.time()

    first_message_received = False
    first_message = None

    while True:
        message = subscriber_socket.recv_string()
        
        # Capture the first received message
        if not first_message_received:
            first_message = message
            first_message_received = True
            print(f"First message received: {first_message}")

        print("Received message:", message)

    # Calculate and print the time taken to receive the first message
    end_time = time.time()
    if first_message_received:
        print(f"First message received: {first_message}")
        print(f"Time taken to receive first message: {end_time - start_time:.6f} seconds")



if __name__ == "__main__":
    # Create a multiprocessing pool with two processes
    pool = multiprocessing.Pool(processes=2)

    # Start the Python subscriber and C publisher concurrently
    pool.apply_async(subscriber)
    pool.apply_async(start_c_publisher)

    # Close the pool
    pool.close()
    pool.join()  # Wait for both processes to complete

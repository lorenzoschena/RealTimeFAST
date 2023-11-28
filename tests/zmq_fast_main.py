import zmq
import subprocess
import time 
import os 
import numpy as np
from random import random  
import struct
import pyFAST.case_generation.case_gen as case_gen
import pyFAST.case_generation.runner as runner
import pyFAST.postpro as postpro
import multiprocessing as mp
from pyFAST.input_output import FASTOutputFile

def send_messages(): 
    socket_address = "tcp://127.0.0.1:5555"
    print('Python is connecting...')
    context = zmq.Context()
    socket = context.socket(zmq.REP)
    socket.bind(socket_address)
    print('Python is connected')
    t = 0 
    try:
        while True: 
            req = socket.recv_string()
            print('Fortran received request: \t {}'.format(req))
            
            if req.lower() == 'wspeed':
                n = 9.5 +  np.sin(t)
                print("Python is sending: \t {}".format(n))
                socket.send(struct.pack('<f', n))
                t += 1
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
        
    return True
        
def launch_fast():
    FAST_EXE  = '/usr/local/bin/openfast'  # Location of a FAST exe (and dll)
    ref_dir   = './templateDir/'        # Folder where the fast input files are located (will be copied)
    main_file = 'Main.fst'            # Main file in ref_dir, used as a template
    work_dir  = './templateDir/'         # Output folder (will be created)
    PARAMS = []                         # leave it like this for future tests
    
    BaseDict = {'TMax': 10.0, 'DT': 0.00625, '__name__': 'Main2'}
    
    PARAMS.append(BaseDict.copy())
    
    fastFiles=case_gen.templateReplace(PARAMS, ref_dir, outputDir=work_dir, removeRefSubFiles=True, main_file=main_file, oneSimPerDir=False)
    print('Main input files written:')
    print(fastFiles)
    runner.run_fastfiles(fastFiles, fastExe=FAST_EXE, parallel=False, 
                         showOutputs=True, nCores=1)
    return True
    
    
if __name__ == '__main__': 
    p1 = mp.Process(target=send_messages)
    p2 = mp.Process(target=launch_fast)
    p2.start()
    p1.start()
    p1.join()
    p2.join()
    
    
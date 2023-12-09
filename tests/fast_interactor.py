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
import pickle
# --------------------------------------- #

req_rep_address = "tcp://127.0.0.1:5555"
pub_sub_address = "tcp://127.0.0.1:8888"

# --------------------------------------- #

def req_rep_handler(address: str = req_rep_address):
    context = zmq.Context()
    req_rep_socket = context.socket(zmq.REP)
    req_rep_socket.bind(address)
    
     # --- Mimic real time wind signal information --- #
    ts_out = FASTOutputFile('./tsim_rpc_1.dat').toDataFrame()
    uh = ts_out['U_[3.40]']
    
    t = 0 
    try:
        while True: 
            req = req_rep_socket.recv_string()
            print('Fortran received request: \t {}'.format(req))
            
            if req.lower() == 'wspeed':
                n = uh[t] #9.5 +  np.sin(t)
                print("Python is sending: \t {}".format(n))
                req_rep_socket.send(struct.pack('<f', n))
                t += 1
            else: 
                n = 0.00
                print("Python is sending: \t {}".format(n))
                req_rep_socket.send(struct.pack('<f', n))
                
    except KeyboardInterrupt:
        req_rep_socket.close()
        context.term()
        uh_sent = uh[:t]
        np.savez('./sent_uh.npz', uh_sent)
        
    return True

# ----------------------------------------- # 

def pub_sub_handler(address: str = pub_sub_address): 
    context = zmq.Context()
    pub_sub_socket = context.socket(zmq.SUB)
    pub_sub_socket.connect(address)
    pub_sub_socket.setsockopt_string(zmq.SUBSCRIBE, "")
    received_updates = []
    try:
        while True:
            update = pub_sub_socket.recv()
            print('{}\n'.format(update.decode))
            received_updates.append(update)
            
    except KeyboardInterrupt: 
        pub_sub_socket.close()
        
        with open('./updates_backlog.pickle', 'wb') as f: 
            pickle.dump(received_updates, f)
    
    return True

# ----------------------------------------- #

def launch_fast():
    FAST_EXE  = '/usr/local/bin/openfast'  # Location of a FAST exe (and dll)
    ref_dir   = './templateDir/'        # Folder where the fast input files are located (will be copied)
    main_file = 'Main02.fst'            # Main file in ref_dir, used as a template
    work_dir  = './TemplateDir/'         # Output folder (will be created)
    PARAMS = []                         # leave it like this for future tests
    
    BaseDict = {'TMax': 20.0, 'DT': 0.005, '__name__': 'Main01'}
    
    PARAMS.append(BaseDict.copy())
    
    fastFiles=case_gen.templateReplace(PARAMS, ref_dir, outputDir=work_dir, removeRefSubFiles=True, main_file=main_file, oneSimPerDir=False)
    print('Main input files written:')
    print(fastFiles)
    runner.run_fastfiles(fastFiles, fastExe=FAST_EXE, parallel=False, 
                         showOutputs=True, nCores=1)
    return True

# ------------------------------------------ #

if __name__ == '__main__':
    p1 = mp.Process(target=req_rep_handler)
    p2 = mp.Process(target=launch_fast)
    # p3 = mp.Process(target=pub_sub_handler)
    
    p2.start()
    p1.start()
    # p3.start()
    
    p1.join()
    p2.join()
    # p3.join()
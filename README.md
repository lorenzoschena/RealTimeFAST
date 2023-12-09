Real-time interactor for OpenFAST
==================================

Development of additional open Fast module which enables real-time communication and interaction with the solver at runtime. 

_Main goals_
- Allow for live change of wind speed
- Allow for adaptation (or checkpointing) of simulation states ($\omega_g$) each N timesteps (re-alignment with live measurements)
- Live broadcasting of simulation outputs (e.g. plotly)

_Exercises_
---
Ex1
--

Ex2
--
Compilation of binaries: 

`
gcc -shared -o zmq_client.so -fPIC zmq_client.c -lzmq
`

`
gfortran-10 -o zmq_fortran_ex2 zmq_test.f90 zmq_client.so
`
Ex3A
--
Goal: publish from C and receive from Python. 


Ex3B
--
Goal: publish from Fortran, send to C and receive in Python. 

Note: need to implement pausing system on the fortran side (which does not support directly milliseconds). 
`
Publisher connected to socket and ready to broadcast.
First message received: Mock Message 2                                    
Received message: Mock Message 2                                    
Received message: Mock Message 3                                    
Received message: Mock Message 4                                    
Received message: Mock Message 5                                    
Received message: Mock Message 6 
`

REQ-REP: asking for specific messages from FAST
--------


PUB-SUB: retrieving live results as simulation progresses 
--------

1. Define quantities of interest to be broadcasted from FAST (later in a specific input file of OpenFAST)
2. Look for indexes of these quantities inside of the outputs of FAST
3. Query the output array at the desired index before passing it to the ZMQ channel 
4. Tag messages with wind turbine number (needed for the Fast.Farm extension of the work)

OpenFAST RT v 1.0 
==================

- Include ZMQ inputs in main `.fst` file
- Enable / disable ZMQ communication for real time REQ-REP (e.g. wind speed, control, etc)
- Format data types in JSON with C
- Enable / disable ZMQ broadcast of desired quantities 
- Build Python interactor for the C istances
	- attach dash / plotly server to the received data (and possibly log it/ dump it to memory)
	- define interactor to modify quantities on-the-go
	

Note
-----

For some reason, OpenFAST loses reference to external library `zmq_client.o`. I.e., if called

`ldd /usr/local/bin/openfast`

I get: 

```
(obi-wan) (yoda) root@613a5f25d104:/home/of_rt/tests# ldd /usr/local/bin/openfast
        linux-vdso.so.1 (0x00007ffed2991000)
        zmq_client.o => not found
        liblapack.so.3 => /lib/x86_64-linux-gnu/liblapack.so.3 (0x00007f10a3185000)
        libblas.so.3 => /lib/x86_64-linux-gnu/libblas.so.3 (0x00007f10a30df000)
        libgfortran.so.5 => /lib/x86_64-linux-gnu/libgfortran.so.5 (0x00007f10a2e04000)
        libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007f10a2d1d000)
        libgcc_s.so.1 => /lib/x86_64-linux-gnu/libgcc_s.so.1 (0x00007f10a2cfb000)
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f10a2ad3000)
        libquadmath.so.0 => /lib/x86_64-linux-gnu/libquadmath.so.0 (0x00007f10a2a8b000)
        /lib64/ld-linux-x86-64.so.2 (0x00007f10a699c000)

```

Fixing (temporary) modifying the environment variable

`export LD_LIBRARY_PATH=/path/to/zmq_client.o:$LD_LIBRARY_PATH`



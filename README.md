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
	



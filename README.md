ZMQ-Comm module for OpenFAST
==================================

OpenFAST module to enable real-time communication with the solver at runtime. 

Main functionalities: 

*IN*:
- _Control(s) override_, blades angles, generator torque and yaw momentum predisposition (in alternative to ROSCO)
- _Wind Flow_, exploiting Inflow Wind in uniform mode, it is possible to change flow experienced by the wind turbine in both 
magnitude and direction (per each direction) plus the VelGust variable$^*$ 

*OUT*:
- _Any Fast output is available to be broadcasted_

($^*$) NOTE: Because of the Uniform mode used in Inflow Wind, the wake meandering model will not model the wake accurately (
no spatial variation implicit in uniform Inflow mode). It is suggested to use another inflow for Fast.Farm at least 
for the current version of the model. 


Dependencies
------------

The main dependencies for both the Fortran/C side and the relevant Python interactor are:

- `cJSON`: C JSON interface. Used to compact messages, easing unpacking across different languages (`https://github.com/DaveGamble/cJSON`)
- `ZMQ`  : engine of the message-passing communications. (`apt-get install libzmq3-dev`)

Python dependencies: 

- `plotly`: used to generate interactive plots as new data is available 
- `zmq`   : used to send/receive messages from the other end of the socket (from FAST).

Usages and examples
-------------------



Python Wrapper interface
-------------------

Fast.Farm extension
-------------------

The Farm extension provides the broadcast protocol, in which each turbine streams its current states and relevant outputs (set-up in the main `.fst` file of each). 
While possible to ovverride values also on the control-side, it is adivised to use the ROSCO supercontroller ZMQ example, as the SuperController of Farm is
going to be discontinued in the upcoming version. 
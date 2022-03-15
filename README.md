# 5G UE Selective Jammer MQP 

### Code Structure:
  * MATLAB - contains all scripts used for simulation, code generation, and data analysis
  * UE_Jammer - contains best version of functional jammer code 
    - MATLAB - contains all C code generated from MATLAB using code generation
    - trx - contains all code for interfacing with the USRP as well as the transmit and receive threads
    - main - source file containing thread initialization, frame synchronization, PDCCH decode, GUI, and jamming signal generation threads.

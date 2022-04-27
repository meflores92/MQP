# 5G UE Selective Jammer MQP 
### Project Abstract 
5G will become a key component in future military operations due to the technology’s low latency, high data rates, and connectivity. However, as 5G becomes integrated into military communication activities, adversaries pose new threats to network security when operating in contested environments. The objective of this project was to design a solution capable of identifying and jamming an adversarial UE (User Equipment) operating on a 5G network, therefore removing the threat posed by the malicious user. Achieving this objective will in turn identify vulnerabilities in the 5G standard that can be exploited revealing possible areas for  improvement. The project was implemented using several software defined radios (SDR) to create a network consisting of one UE and one base station (gNodeB). At the completion of the project, the proposed solution was capable of identifying an adversarial UE within a network, following the UE’s path in the 5G resource frame, and jamming the adversarial UE. 
### Code Structure
  * Archive - contains zip files with previous revisions of functional jammer code. Testing and validation was perfromed with "Jammer_Edit_GUI".  
  * MATLAB - contains all scripts used for simulation, code generation, and data analysis
  * UE_Jammer - contains best version of functional jammer code 
    - MATLAB - contains all C code generated from MATLAB using code generation
    - trx - contains all code for interfacing with the USRP as well as the transmit and receive threads
    - main - source file containing thread initialization, frame synchronization, PDCCH decode, GUI, and jamming signal generation threads.

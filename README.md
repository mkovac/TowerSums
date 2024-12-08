# Install and run instructions

Clone the repository:
```
git clone git@github.com:mkovac/TowerSums.git
```

Run the Makefile:
```
make 
```

Run the code:
```
./run
```


# Stage 1 Tower Sums - Mapping Instructions

This document provides instructions for the various mappings required to build Stage 1 Tower Sums.

## 1. Mapping FrontEnd (Modules, Module Sums, and STCs) to Stage-1 Boards

This mapping defines which module is connected to each Stage-1 board. It is essential because it specifies from which modules the unpacker will receive data. The unpacker will then reformat the data in a uniform way and send it to the TowerSums block.

A visual representation of how each module connected to Board 0 is numbered can be found in this [sketch](https://gitlab.cern.ch/tsculac/TPGStage1/-/blob/main/Emulator/inputs/mapping/InputMappingPlots/Input%20of%20Boards%200.png?ref_type=heads). This indicates that the TowerSums block will receive data (energies) from these modules in **N** parallel streams, with each number indicating the stream from which the data originates.

A human-readable version of this mapping is available [here](https://gitlab.cern.ch/tsculac/TPGStage1/-/blob/main/Emulator/inputs/mapping/v2/Input_CEE_v2.txt?ref_type=heads), which shows how the boards are connected to the modules and how they are organized in different channels.

## 2. Mapping of How to Build pTTs from MS and STCs

Once data is received from the frontend, the vh files provide instructions on how to combine this data to build pTTs. These instructions are available in Verilog Header format for firmware production [here](https://gitlab.cern.ch/tsculac/TPGStage1/-/tree/main/Firmware/inputs/v3?ref_type=heads) and also in a human-readable format [here](https://gitlab.cern.ch/tsculac/TPGStage1/-/tree/main/Emulator/inputs/mapping/v2?ref_type=heads).

This step needs to be added to the emulator, as it is not currently included.

## 3. Mapping of Stage 1 Outputs (TowerSums Block and Trigger Cell Processor Block) to Stage 2 Boards

This mapping specifies how pTTs and ordered TC energies should be organized into channels, frames, and words so that the data can be transmitted to Stage 2 boards through 4+2 links.

An example of this mapping, viewed from the perspective of Stage 1 or Stage 2 boards, can be found [here](https://gitlab.cern.ch/tsculac/TPGStage1/-/tree/main/Emulator/inputs/mapping/S1S2_Mapping?ref_type=heads).

It is understood that this part is already being implemented by Adinda for the TC section, so it should be used as is. There is no need for two separate implementations, as both pTTs and TCs will be packed in the same words.
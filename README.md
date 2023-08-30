# CETT Solver Source Code for ITC-2007

This repository contains the source code for solving the ITC-2007 Capacitated Examination Timetabling Problem, as discussed in the paper referenced below. If you use this code, kindly cite the paper in your work:
Van Bulck, D., Goossens, D. and Schaerf, A. (2023). Multi-Neighbourhood Simulated Annealing for the ITC-2007 Capacitated Examination Timetabling Problem. Submitted to Journal of Scheduling.

## Compilation Instructions for Linux

Follow these steps to compile the source code on a Linux system:

1. Clone the repository using the following command:
   

```bash
git clone https://github.com/davidvanbulck/ITC-2007-CETT.git
```


2. Ensure that Boost is installed on your system.

3. Navigate to the main directory and execute the following command:

```bash
make
```


4. To run the program, use the following command:

```bash
./EasyLocal --main::feasibleOnly 3 --main::hard_weight 279 --main::init_method OBSI --main::room_bias_move 0.17 --main::room_bias_kick 0.04 --main::kick_rate 0.43 --main::swap_bias 0.93 --main::kempe_rate 0.03 --main::shake_rate 0.09 --main::beam_rate 0.01 --main::beamdepth 7 --main::beamwidth 2 --main::component_rate 0.03 --SSA::start_temperature 749 --SSA::expected_min_temperature 0.72 --SSA::cooling_rate 0.97 --SSA::neighbors_accepted_ratio 0.2 --main::kempeRoomMode 0 --main::method SSA --main::instance ./examtt-instances/Instances/set1.exam --main::seed 1000 --main::output_file my_outputfile.sol
```


5. To determine the allowable runtime for your system, execute:
```bash
./benchmark_machine/benchmark_my_linux_machine
```
Please note that this program was originally sourced from the official ITC2007 competition website (https://www.cs.qub.ac.uk/itc2007/), which may no longer be available.

6. To embed neighbourhoods into the framework of Simulated Annealing, note that this software makes use of EasyLocal++. For more information on this fantastic piece of software, refer to https://bitbucket.org/satt/easylocal-3/src/no_output/.

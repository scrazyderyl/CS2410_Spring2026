- [CS 2410 - Computer Architecture](#cs-1541---introduction-to-computer-architecture)
- [Introduction](#introduction)
  * [Description](#description)
- [Building and Running](#building-and-running)
  * [Environment Setup](#environment-setup)
  * [Directory Structure and Makefile Script](#directory-structure-and-makefile-script)

# CS 2410 - Computer Architecture
Spring Semester 2026 - Project

* DUE: April 26 (Sunday), 2026 11:59 PM 

Please accept the Project on **GitHub Classroom** using this link: https://classroom.github.com/a/Nsi2q5Zc

When you click on the above link, you will be asked to select your email
address from a list.  After you select your Pitt email, it will fork a
repository off the template repository for this project that you can work on.
It is created in **private** mode by default --- please keep it that way.

# Introduction

## Description

Please refer to the [Project.pdf](Project.pdf) file for detailed instructions.

# Building and Running

## Environment Setup

The project is setup to run with the g++ compiler (GNU C++ compiler) and a
Make build system.  This system is already in place on the departmental
Linux machine (thoth.cs.pitt.edu).  If you have a similar setup on your
local computer, please feel free to use your machine for development.
Otherwise, you need to log in to thoth.cs.pitt.edu which may involve some
setup.  Note that you need to be on Pitt VPN to connect to thompson.cs.pitt.edu
off-campus (instructions at
https://www.technology.pitt.edu/services/pittnet-vpn-pulse-secure).

Assuming you are on the Pitt network, here are the steps you need to take:

1. Most OSes (Windows, MacOS, Linux) comes with built-in SSH clients accessible using this simple command on your commandline shell:
   ```
   ssh USERNAME@thompson.cs.pitt.edu
   ```

2. Once connected, the host will ask for your Pitt SSO credentials.  Enter your password.

3. Create and go to a directory of your choice (or you can stay at your default
   home directory) and then clone your GitHub Classroom repository:

   ```
   git clone <your GitHub Classroom repository HTTPS URL>
   ```

   This will ask for your Username and Password.  Username is your GitHub
account username, but Password is not your password.  Password authentication
on GitHub has been deprecated on August 2021, so now you have to use something
called a Personal Authenication Token (PAT) in place of the password.  Here are
instructions on how to create a PAT:
https://docs.github.com/en/github/authenticating-to-github/keeping-your-account-and-data-secure/creating-a-personal-access-token

## Directory Structure and Makefile Script

Here is an overview of the directory structure:

* src/ : Implementation .cpp files for the simulator.  Start by reading main.cpp.
* include/ : Header .h and .hpp files for the simulator.  Start by reading Simulator.h.
* Makefile : The build script for the Make tool.
* configs/ : Directory where processor configuration files are.
* **diffs/** : Directory where **differences** between outputs/ and outputs_solution/ are stored.
* outputs/ : Directory where outputs after running five_stage are stored.
* outputs_solution/ : Directory where outputs produced by five_stage_solution are stored.
* programs/ : Directory where RISC-V assembly programs used to test the simulator are stored.

In order to build the project and run the simulations, you only need to do 'make' to invoke the 'Makefile' script:

```
make
```

The output should look like:

```
g++  -fsanitize=address -g -c -Wall -Wextra -std=c++11 -Iinclude/ `pkg-config --cflags glib-2.0` -c  src/Config.cpp -o obj/Config.o
g++  -fsanitize=address -g -c -Wall -Wextra -std=c++11 -Iinclude/ `pkg-config --cflags glib-2.0` -c  src/main.cpp -o obj/main.o
g++  -fsanitize=address -g -c -Wall -Wextra -std=c++11 -Iinclude/ `pkg-config --cflags glib-2.0` -c  src/Simulator.cpp -o obj/Simulator.o
src/Simulator.cpp: In constructor ‘Simulator::Simulator(std::ifstream*, Config*)’:
src/Simulator.cpp:6:37: warning: unused parameter ‘program’ [-Wunused-parameter]
    6 | Simulator::Simulator(std::ifstream* program, Config *c)
      |                      ~~~~~~~~~~~~~~~^~~~~~~
src/Simulator.cpp:6:54: warning: unused parameter ‘c’ [-Wunused-parameter]
    6 | Simulator::Simulator(std::ifstream* program, Config *c)
      |                                              ~~~~~~~~^
g++  -fsanitize=address -g -c -Wall -Wextra -std=c++11 -Iinclude/ `pkg-config --cflags glib-2.0` -c  src/SimulatorDebug.cpp -o obj/SimulatorDebug.o
g++  obj/Config.o obj/main.o obj/Simulator.o obj/SimulatorDebug.o -fsanitize=address `pkg-config --libs glib-2.0` -o sim
```

If successful, it will produce the executable "sim" as the above.

By invoking the "run" target, you can run the test programs against the configurations, and compare the output with the solution output:

```
make run
```

This will generate the following output:

```
Running ./sim -p programs/prog-loop.dat -c configs/1-wide.cfg -j outputs/prog-loop.1-wide.json -d > outputs/prog-loop.1-wide.out
Running ./sim -p programs/prog-noloop.dat -c configs/1-wide.cfg -j outputs/prog-noloop.1-wide.json -d > outputs/prog-noloop.1-wide.out
Running ./sim -p programs/prog-loop.dat -c configs/2-wide.cfg -j outputs/prog-loop.2-wide.json -d > outputs/prog-loop.2-wide.out
...
```

You can write your own programs under the programs/ folder and also add new
configurations under the configs/ folder and they will be added to the test
suite.  The results are stored in the outputs/ directory and also diffs with
the outputs_solution/ directory are generated and stored in the diffs/
directory.  When you debug the program, you will find these diffs useful.

If you wish to remove all files generated from your build, invoke the "clean" target:

```
make clean
```


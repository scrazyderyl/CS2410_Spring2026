- [CS 2410 - Computer Architecture](#cs-2410---computer-architecture)
- [Introduction](#introduction)
  * [Description](#description)
- [Building and Running](#building-and-running)
  * [Environment Setup](#environment-setup)
  * [Directory Structure and Makefile Script](#directory-structure-and-makefile-script)
  * [Debugging](#debugging)
    + [VSCode Debugger](#vscode-debugger)
    + [Google Address Sanitizer](#google-address-sanitizer)
    + [Logging](#logging)

# CS 2410 - Computer Architecture
Spring Semester 2026 - Project

* DUE: April 29 (Wednesday), 2026 11:59 PM 

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

The project is setup to run with the g++ compiler (GNU C++ compiler) and a Make
build system.  This system is already in place on the departmental Linux
machine (thompson.cs.pitt.edu) and on the Codespace Devcontainer.  If you have
a similar setup on your local computer, please feel free to use your machine
for development (you can view the [packages installed in the
Devcontainer](.devcontainer/Dockerfile) to see what setup you need).

If you choose to use the departmental machine, note that you need to be on Pitt
VPN to connect to thompson.cs.pitt.edu (instructions at
https://www.technology.pitt.edu/services/pittnet-vpn-pulse-secure).

Here are the steps you need to take:

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
called a Personal Authentication Token (PAT) in place of the password.  Here are
instructions on how to create a PAT:
https://docs.github.com/en/github/authenticating-to-github/keeping-your-account-and-data-secure/creating-a-personal-access-token

Alternatively, you can choose to use 
[Codespaces](https://docs.github.com/en/codespaces).  You do not have to worry
about billing since you will be using my educational credits if you create
Codespaces from the github classroom repository you fork off using the above
link.  If you haven't used Codespaces before, The only thing is that you have
to be careful of is that the Codespace will be garbage collected after 30 days
of inactivity (it uses cloud storage after all).  You will get a warning email
to the email connected with your github account.  So, it is a good habit to
regularly commit and push any changes to the central repository.

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

## Debugging

Here are some suggestions for debugging your simulator.

### VSCode Debugger

You can use the VSCode debugger to run the simulator in debug mode.  If you
have never used the VSCode debugger before, here is a tutorial:
https://code.visualstudio.com/docs/debugtest/debugging.

You can change the debugger launch configurations by editing the
[launch.json](.vscode/launch.json) file.  Currently it is configured to run the
2-wide.cfg configuration on the prog-noloop.dat program.

You can use the standard debugging techniques such as setting break points,
stepping through code, and watching values of variables and expressions.


### Google Address Sanitizer

You might have noticed that the Makefile passes the "-fsanitize=address" flag
to gcc when you compile the program.  What this flag does is to instrument your
binary with memory access checking code so that whenever you have an illegal
memory access or memory leak it throws an exception, much like how Java throws
an exception on an illegal access.  It is meant to help you and make C++
programming feel more like Java programming, as the biggest headache with C/C++
programming often is memory management.

This instrumentation pass and associated library is called the Gooogle Address
Sanitizer (ASAN).  You can read more about it here:
https://github.com/google/sanitizers/wiki/addresssanitizer.

The expectation is that your simulator has no memory errors when submitted.  If
it does, then it is going to be a very flaky simulator that is unusable.

### Logging

You will benefit from having logging functions that output internal state of
the CPU so that you can view what is happening at each cycle.  Writing these
logging functions is a part of writing a good simulator.  Ideally, you can
enable logging by passing a command line flag.

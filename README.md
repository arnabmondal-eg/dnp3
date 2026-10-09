# dnp3 Parser
This project is an attempt to simulate the **dnp3** protocol often used for communication in **SCADA** systems. This project is still in its early stages, only part of the spec is implemented. 

There are no plans to impelemtent the entire spec however, as this protol is far to large. Only specifc, commonly used parts will be implemented, such that a common packet can be parsed succesfuly.

## Goals
* Fast, Memory Efficient Proccesing
* Most of spec (much will have to be skipped however)

## Project Structure
```
dnp3/                                                             
│                                                                 
├──── .vscode/           // settings and configurations for vscode
│                                                                 
├───┬ backend/           // all parser logic                 
│   │                                                             
│   ├──── build/         // build files, binaries, etc.           
│   │                                                             
│   ├──── inc/           // header files                          
│   │                                                             
│   └───┬ src/           // source files                          
│       │                                                         
│       ├──── dnp3/      // struct implementations                
│       │                                                         
│       ├──── helper/    // helper functions                      
│       │                                                         
│       └──── main.c     // backend start                                           
│                                                                 
└──── log/               // logs from backend                     
```
## Curently Implmented
* Parsing of dnp3:
    * Header
    * DLC
    * Transport Header
    * Application Header
    * First Object Header

## Usage
The easiest method to run the backend is to open the repo in VsCode and use the built in tasks. Manual Methods are also avalible.

### General
* Clone the repository
* Install both a C compiler, JDK, and CMake
    * I used Clang on MacOS and on Debian through WSL


### Backend
Contains Parser, Client, Server, as well as 4 libraries

#### vscode Task
1. Hit `Ctrl + Shift + P` and type `Tasks: Run Task`
2. Select `CMake Build` or `CMake Run Binary`, depending on your goal
3. Select `Clean Backend` or `Clean All` to remove build dir

#### Manual
Backend is compiled with CMake to make your life a little easier
1. Compile:
    ```bash
    cmake -S backend -B backend/build
    cmake --build backend/build --config Debug
    ```
2. Run the built binary(s):
    ```bash
    ./backend/build/bin/paser
    ./backend/build/bin/client
    ./backend/build/bin/server
    ```

# dnp3 Protocol
This project is an attempt to simulate the **dnp3** protocol often used for communication in **SCADA** systems. This project is still in its early stages, so little to no functionality is still implemented.

## Goals
* Full JavaFX or Swing ui implementiation of;
    * Packet Analyser
    * Master Client
    * Slave Client
* Fast, Memory Efficient Proccesing
* Integration of both Java and C
## Curently Implmented
* Partial Packet Viewer UI
* Header Datastructure thing...

## Usage
Currently, backend and frontend are not linked. Plans to use Network Sockets to avoid using JNI or JNA

### General
* Clone the repository
* Install both a C compiler, JDK, and CMake
    * I used Clang and Java 25

### Backend
Backend is auto compiled with CMake to make your life a little easier
1. First cd into `backend/`
2. Compile:
    ```bash
    cmake -S backend -B backend/build
    cmake --build backend/build --config Debug
    ```
3. Run the build binary:
    ```bash
    ./backend/build/bin/dnp3
    ```

### Frontend
Not implemented yet
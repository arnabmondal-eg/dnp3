# dnp3 Parser
This project is an attempt to simulate the **dnp3** protocol often used for communication in **SCADA** systems. This project is still in its early stages, so little to no functionality is still implemented.

## Goals
* Pure Swing UI implementiation of;
    * Packet Parser
    * Master Client
    * Child Client
* Fast, Memory Efficient Proccesing

## Project Structure
```
dnp3/
|---- .vscode/
|   |                                           // settings/configs for vscode
|   |---- settings.json                         // contains information on source dirs
|   |---- tasks.json                            // runnable tasks (compile, clean, ...)
|
|---- backend/                                  // c based backend
|   |---- src/                                  // source files
|   |   |
|   |   |---- main.c                            // main backend invocation
|   |   |
|   |   |---- header.h                          // header typedef
|   |   |---- header.c                          // implementation of header struct
|   |   
|   |---- CMakeLists.txt                        // compile instructionns for cmake
|
|---- frontend/                                 // java based swing ui implementation
|   |---- src/                                  // source files
|       |---- ui/                               // window defs (parser, master, client)
|       |   |
|       |   |---- parserWindow.java         // parser window
|       |
|       |---- helper/
|       |   |
|       |   |---- hexHelper.java
|       |   |---- frameCreator.java             // simple window creator
|       |
|       |---- Main.java                         // main frontend invocation
|
|---- CMakeLists.text                           // cmake defs
```
## Curently Implmented
* Partial Packet Viewer UI
* Header Datastructure thing...

## Usage
The easiest method to run either the frontend or backend is to open the repo in VsCode and use the built in tasks. Manual Methods are also avalible.

Currently, backend and frontend are not linked. Plans are to use Network Sockets to avoid using JNI or JNA

### General
* Clone the repository
* Install both a C compiler, JDK, and CMake
    * I used Clang and Java 25


### Backend
Contains Parser Logic.

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
2. Run the built binary:
    ```bash
    ./backend/build/bin/dnp3
    ```

### Frontend
Contains UI with Some simulated Data (for now...)

#### vscode Task
1. Hit `Ctrl + Shift + P` and type `Tasks: Run Task`
2. Select `Java Compile` or `Run Java Frontend`, depending on your goal
3. Select `Clean Frontend` or `Clean All` to remove build dir

#### Manual
Frontend Compiled with `javac`
1. Compile:
    ```bash
    javac -d bin frontend/src/**/*.java
    ```
2. Run the bult binary:
    ```bash
    java -cp frontend/bin Main
    ```

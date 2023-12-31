# MyLittleIDS - DRACSIS

## Table of Contents
0. [Downloads](#0-downloads)
    - [GITHUB](#Download)
1. [Introduction](#introduction)
    - [Overview](#overview)
    - [Purpose](#purpose)
2. [Installation](#installation)
    - [Prerequisites](#prerequisites)
    - [Compilation](#compilation)
3. [Usage](#usage)
    - [Running the Program](#running-the-program)
    - [Understanding Output](#understanding-output)
4. [Functionality](#functionality)
    - [`exiting` Function](#exiting-function)
    - [`monitorPort` Function](#monitorport-function)
    - [`isPortOpen` Function](#isportopen-function)
    - [`Makescript` Function](#makescript-function)
    - [`runscript` Function](#runscript-function)
5. [Example](#example)
    - [Running the Program](#running-the-program-1)
    - [Sample Output](#sample-output)
6. [Conclusion](#conclusion)
    - [Cybersecurity Challenges](#cybersecurity-challenges)
        - [Challenge 1](#challenge-1-port-scanning)
        - [Challenge 2](#challenge-2-authentication-failure-monitoring)
        - [Challenge 3](#challenge-3-real-time-connection-monitoring)
    - [Summary](#summary)
    - [Future Improvements](#future-improvements)
7. [Acknowledgments](#acknowledgments)
    - [CHATGPT](#chatgpt)
    - [Libraries](#libraries)
8. [Credits](#8-credits)
    - [Dracsis](#dracsis)
---

## 0. Downloads

### Download
**[HERE](https://github.com/rolisis17/IDS.git)** - Download Source Code from **[GITHUB](https://github.com/rolisis17)**

## 1. Introduction

### Overview
"MyLittleIDS" is a simple Intrusion Detection System implemented in C++ and Bash. It monitors network ports for suspicious activity and logs authentication failures in a Linux system's log files.

### Purpose
The purpose of this program is to:
- Check for open ports on a range of port numbers.
- Monitor these open ports for incoming connections.
- Log suspicious login attempts by parsing log files.
- Provide a simple IDS solution for detecting potential security breaches on a Linux system.

## 2. Installation

### Prerequisites
Before using the "MyLittleIDS" program, ensure you have the following prerequisites:
- A Linux-based operating system.
- A C++ compiler (e.g., `g++`).
```bash
sudo apt update
sudo apt install g++
```
- Make installed
```bash
sudo apt update
sudo apt install Make
```
- Basic knowledge of Bash scripting.

### Compilation
To compile the program, use the provided Makefile. You can run `make name` to build the program, and `make run` to execute it. The Makefile also includes a `clean` rule for removing compiled files.

## 3. Usage

### Running the Program
To run the program, inside the folder execute:

```bash
make run
```

or the compiled and run the binary.
Compile with:
```bash
make
```
or
```bash
c++ mylittleids.cpp -o IDS
```
execute:
```bash
./IDS
```
The program will check for open ports and monitor authentication failure attempts.

### Understanding Output
The program will first check a range of ports (from 0 to 1023) and report open ports in red.
It will then monitor these open ports for incoming connections and print "Incoming connection accepted" for each connection.
After port monitoring, the program checks the system's authentication logs for suspicious login attempts. Suspicious login attempts are printed in red, and successful login attempts are displayed in green.
The program also creates and executes a Bash script to check for authentication failures using journalctl.

## 4. Functionality
### exiting() Function
This function handles errors by printing an error message and closing a socket if provided.
It is used for error handling in the program.

### monitorPort() Function
Monitors an open network port for incoming connections.
Implements a timeout mechanism (default 10 seconds) to stop monitoring if no connections occur.
Prints "Timeout reached" if the timeout is reached.
Prints "Incoming connection accepted" for each incoming connection.

### isPortOpen() Function
Checks if a port is open by attempting to connect to it.
Prints "Port is open" in red if the connection is successful.
Returns true if the port is open, otherwise false.

### Makescript() Function
Creates a Bash script named script.sh based on the check parameter.
The script checks for authentication failures in the system logs using journalctl.

### runscript() Function
Executes the script.sh Bash script.
Uses fork and waitpid to run the script as a child process.
Calls Makescript before executing the script.

## 5. Example

### Running the Program
```bash
./IDS
```
![https://imgur.com/gallery/OgW687f](https://i.imgur.com/l9b80wh.png)

IF FAIL: See image **[HERE](https://i.imgur.com/l9b80wh.png)**

### Sample Output
The program will display information about open ports, incoming connections, and authentication attempts.
Ports found open will be printed in red.
Suspicious login attempts will be printed in red.
Successful login attempts will be printed in green.

## 6. Conclusion
## Cybersecurity Challenges

### Challenge 1: Port Scanning

One common cybersecurity challenge is detecting open ports on a system. "MyLittleIDS" addresses this challenge as follows:

- The program scans a range of ports from 0 to 1023 to identify open ports.

- If an open port is detected, it is displayed in red. This can help system administrators identify potentially vulnerable services running on the system.

### Challenge 2: Authentication Failure Monitoring

Monitoring authentication failures is crucial for detecting unauthorized access attempts. "MyLittleIDS" provides the following capabilities:

- It creates and executes a Bash script named script.sh to monitor authentication failures using journalctl.

- Suspicious login attempts are displayed in red, making it easy to spot potentially malicious activity in the system logs.

### Challenge 3: Real-time Connection Monitoring

To enhance security, it's essential to monitor incoming connections in real-time. "MyLittleIDS" accomplishes this as follows:

- The monitorPort function continuously listens for incoming connections on open ports.

- If an incoming connection is detected, it is logged with the message "Incoming connection accepted."

### Summary
"MyLittleIDS" is a simple Intrusion Detection System designed to check for open ports and monitor authentication logs on a Linux system. It provides basic functionality to detect potential security threats.

### Future Improvements
Future improvements to the program may include:

Enhanced log analysis and reporting.
Support for more advanced authentication monitoring.
Integration with system monitoring and alerting mechanisms.
Configuration options for customizing behavior.

## 7. Acknowledgments
### ChatGPT
- **[CHATGPT](https://chat.openai.com/)**
Big Part of this project could be finish faster and better only with the IA's help.

### Libraries

This project relies on several libraries and tools that have contributed to its functionality and success. We would like to acknowledge and express our gratitude to the following:

- **C++ Standard Library**: The C++ Standard Library provides essential functionality for C++ programs. We appreciate the wealth of features and utilities it offers, which greatly simplify development.

- **[libstdc++](https://gcc.gnu.org/onlinedocs/libstdc++/)**: libstdc++ is the Standard C++ Library for the GNU Compiler Collection (GCC). It extends the C++ Standard Library and is an integral part of GCC. We thank the developers of libstdc++ for their continuous efforts in providing a robust and reliable library.

- **[Socket Programming Libraries](https://man7.org/linux/man-pages/man7/socket.7.html)**: The socket programming libraries, including `<sys/socket.h>`, `<netinet/in.h>`, `<cstring>`, and others, enable network communication in C++ programs. We acknowledge the importance of these libraries for building network-related applications.

- **[fcntl.h](https://man7.org/linux/man-pages/man2/fcntl.2.html)**: The `fcntl.h` library is essential for setting the socket to non-blocking mode in your program. It provides valuable functionality for controlling file descriptors, including sockets.

- **[unistd.h](https://man7.org/linux/man-pages/man2/unlink.2.html)**: The `unistd.h` library offers various system calls and constants that are crucial for system-level programming, including process management and file handling.

- **[arpa/inet.h](https://man7.org/linux/man-pages/man3/inet.3.html)**: The `arpa/inet.h` library provides functions for manipulating Internet addresses and converting between numeric and text forms. It's fundamental for working with IP addresses.

- **[sys/wait.h](https://man7.org/linux/man-pages/man2/wait.2.html)**: The `sys/wait.h` library is used for process management in your program, specifically for waiting for child processes to complete.

- **[fstream](https://en.cppreference.com/w/cpp/header/fstream)**: The `<fstream>` library allows for file input and output operations in C++. It's used for creating and executing the Bash script in this program.

We are grateful to the developers and maintainers of these libraries for their contributions to the open-source community, making it possible for us to build powerful and feature-rich software.

## 8. Credits
# Dracsis

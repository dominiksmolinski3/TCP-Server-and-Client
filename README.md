# TCP Server and Client

This project consists of a TCP server and client implementation in C++. The server can handle multiple clients simultaneously using pthreads. The client can send messages to the server and receive responses.

## Features
- Server handles multiple clients concurrently using threads.
- If the client sends "hello", the server responds with "world", otherwise the server repeats what the client sent.
- The client can exchange messages with the server until it sends "disconnect".
- Proper error handling and resource management.

## Compilation and Usage

To build the project, use the provided Makefile:

```bash
make

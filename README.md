# simple_messanger

## Overview

This is a simple client-server chat application written in C++. The application consists of:
1. A **server** that accepts multiple client connections and broadcasts messages to all connected clients
2. A **client** with a graphical interface that connects to the server and can send/receive messages

## Purpose

The application demonstrates:
- Basic TCP socket programming in C++
- Multithreaded server architecture
- Client-server communication model
- Simple GUI client implementation using Windows Forms

## How It Works

### Server
1. The server starts and listens on port 5555 by default
2. When a client connects, the server creates a new thread to handle that client
3. Messages received from any client are broadcast to all other connected clients
4. The server maintains a list of all active client connections

### Client
1. The client provides a GUI with connection settings (IP and port)
2. After connecting, messages can be sent via the text box
3. Received messages are displayed in the chat history area
4. The connection status is indicated by a red/green indicator

## Compilation Instructions

### Server
1. Compile with a C++ compiler that supports C++11 (tested with Visual Studio/MSVC)
2. Requires linking with `ws2_32.lib` (Winsock library)

### Client
1. This is a Windows Forms application and requires .NET framework
2. Compile as a Windows Forms project in Visual Studio
3. Requires linking with `ws2_32.lib` (Winsock library)
4. The project should be set up as a CLR (Common Language Runtime) project

## Running the Application

1. First start the server (it will show available IP addresses and port)
2. Run the client application
3. In the client:
   - Enter the server's IP address (use 127.0.0.1 for local testing)
   - Enter the port (default 5555)
   - Click CONNECT
4. Once connected, type messages in the bottom text box and click SEND

## Notes

- The server can handle multiple clients simultaneously
- Messages are broadcast to all connected clients except the sender
- Type "exit" in the client to disconnect
- The server runs continuously until manually stopped 

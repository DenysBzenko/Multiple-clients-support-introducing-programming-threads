# Multiple-clients-support-introducing-programming-threads

## Description

This project is written in c++. It includes a server that has the ability to accept more than 2 clients at the same time, and accept their requests.

## Functions

- View file list: The two clients can get a list of all files in a specific directory on the server st the same time.
- File deletion: The two clients can delete a file on the server at the same time .
- Get file information: The two clients can get information about when it was last modified, how much it weighs at the same time.

## How do you get started with this project?

### Dowload 
``` bash
git clone https://github.com/DenysBzenko/Introduction-to-client-server-programming-using-TCP.git
```
-And just compile the code

### Server

The server automatically starts listening for incoming connections on the port specified in the code ( 12345).

### Clients

After starting the client, you can enter the following COMMANDS: For example:
- LIST to get a list of files.
- DELETE <filename>  to delete a file.
- INFO <filename> to get information about the file.

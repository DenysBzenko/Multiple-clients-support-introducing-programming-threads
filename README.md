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
https://github.com/DenysBzenko/Multiple-clients-support-introducing-programming-threads.git
```
-And just compile the code

### Server

The server automatically starts listening for incoming connections on the port specified in the code (12345).

### Clients

After starting the client, you can enter the following COMMANDS:
- LIST to get a list of files.
- DELETE <filename>  to delete a file.
- INFO <filename> to get information about the file.

For example:

- LIST

- DELETE TEST.txt

- INFO TEST.txt

## LIST

The server can accept the LIST command.
- CLIENT>>LIST
  
After the server receives a request for the LIST command, it processes the request and calls the 'handleList' function. Server sends the client a C string with the names of all the files located in the directory. The server can send a C string the maximum capacity of 4096 bytes.

- SERVER>> The names of all the files that this directory contains.(EXAMPLE: TEST.txt Fish.txt Milk.txt)

Client can recv a C string the maximum capicity of 4096 bytes. 
  


## DELETE 
The server can accept the DELETE <FILENAME> command.
- Client>> DELETE <FILENAME> (EXAMPLE: Test.txt)
  
The client must write the delete command and the file name with the correct extension. After that, the server calls the 'handleDelete' function. Then, the server deletes this file from directory, if the file is found. After successful deletion, the client will receive the following command
- SERVER>>'File deleted successfully'
* OR 
- SERVER>> 'Error deleting file'. 

## INFO

This command must be entered correctly with INFO <FILENAME>, with the correct file extension.
- CLIENT>> INFO Milk.txt
  
After that, the 'handleinfo' function is called and searches for this file in the directory.This command helps you find out the main current information about the file (SIZE, LAST MODIFICATION, NAME). We have a basic size in c++ to read the time when it was last modified [26] bytes.

- SERVER>> File: Milk.txt
Size: 16 bytes
Last modified: Mon Jan 22 09:38:33 202



# Socket Programming with C++ in Windows 

#### This repository include all the basic program of Socket Programming using C++ language in Windows.

## Table of contents
* [General info](#general-info)
* [Libraries](#libraries)

## General info
This repository include four type of Socket programs 
1. [Data Transfer Program](#1-data-transfer-program)
2. [Chat Application](#2-chat-application)
3. [File Transfer Program](#3-file-transfer-program)
4. [CRC Error Checking Program](#4-crc-error-checking-program)

## Libraries
To run this Program you must have following C++ libraries:
* <windows.h>
* <winsock2.h>
* <ws2tcpip.h>
* <stdlib.h>
* Ws2_32.lib
* Mswsock.lib
* AdvApi32.lib

## 1. Data Transfer Program
#### Client program send Bytes of data to Server program. 
To run this project, Compile file and run it:

```
>> 0_server
>> 0_client localhost
```
![App image](https://github.com/anant0103/Socket-Programming-with-C-in-Windows/blob/main/image0.png)

## 2. Chat Application
#### Send messages between Client and Server. 
To run this project, Compile file and run it:

```
>> 1_server
>> 1_client localhost
```
![App image](https://github.com/anant0103/Socket-Programming-with-C-in-Windows/blob/main/image1.png)

## 3. File Transfer Program
#### Client program send File to Server program. 
To run this project, Compile file and run it:

```
>> 2_server
>> 2_client localhost
```
![App image](https://github.com/anant0103/Socket-Programming-with-C-in-Windows/blob/main/image2.png)

## 4. CRC Error Checking Program
#### Client program send data to Server program and server check its CRC and checking Error. 
To run this project, Compile file and run it:

```
>> 3_server
>> 3_client localhost
```
![App image](https://github.com/anant0103/Socket-Programming-with-C-in-Windows/blob/main/image3.png)
 

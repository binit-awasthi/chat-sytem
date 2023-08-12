# chat_sytem
A Group chat program

# PROJECT MEMBERS
   1. Binit Awasthi   (https://github.com/Polarizer5)
   2. Anik Chaudhary  (https://github.com/anik-c)
   3. Krish Karn      (https://github.com/Chriskarna)
   4. Karna Tamang    

# PREREQUISITES
Your compiler must contain winsock and pthread libraries 


# HOW TO COMPILE AND RUN THE PROGRAM

1. Open a terminal and enter:

  For server:     "gcc -o server "filepath" -lws2_32 -lpthread"
  For client(s):  "gcc -o client "filepath" -lws2_32 -lpthread"

  where filepath is the respective path of client or server program.

2. Now to run the program simply enter (on separate terminal windows):
   For server: ".\server"
   For client: ".\client"

# NOTE: 
1. This program is for windows only.
2. The code by default supports a local host connection where the server and client programs run on the same device.
3. For local host connection run the server program in one terminal and the client program on several different terminal windows.
   where each client terminal client acts as an individual client.

5. FOR CONNECTION BETWEEN DIFFERENT DEVICES:

   1. Simply modify the SERVER_IP macro in the client code to store the server device's ipv4 address.
   2. Run the server code on a device, and the same client code on different devices with the modified ipv4 address of the server device.

6. The program supports connection over both same network and different networks(remote communication).

7. For connection over different networks:
    Your router must support port forwarding (not set by default in most routers).
    The program lacks appropriate security mechanisms, so it is recommended to not use this for remote communication

8. For connection over the same network
   Use the same approach as described earlier in point 4.
   

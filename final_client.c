#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <windows.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define PORT 9000
#define USERNAME_SIZE 100
#define SERVER_IP "127.0.0.1"
#define delay 50000

#pragma comment(lib, "ws2_32.lib")

void *receive_messages(void *socket_ptr);

char login_signup_choice();
void handle_login(SOCKET client_socket);
void handle_signup(SOCKET client_socket);
void *send_messages(void *socket_ptr);
void *receive_messages(void *socket_ptr);
void after_login_signup(SOCKET *client_socket, WSADATA *wsa);
void credentials_prompt(char *buffer, char *username, char *password);

WSADATA wsa;
SOCKET connection()
{
    while (1)
    {
        system("cls");
        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        {
            printf("WSAStartup failed\n");
            WSACleanup();
            sleep(1);
            return 1;
        }

        SOCKET client_socket;
        struct sockaddr_in serv_addr;
        char buffer[BUFFER_SIZE];

        client_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (client_socket == INVALID_SOCKET)
        {
            printf("error creating socket.\n");
            closesocket(client_socket);
            WSACleanup();
            sleep(1);
            return 1;
        }

        ZeroMemory(&serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(PORT);
        serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

        if (connect(client_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
        {
            printf("\nError connecting to server.\n\n");
            closesocket(client_socket);
            WSACleanup();
            printf("Retry connecting to server (y/n)? ");

        here:
            char c = getch();
            if (c == 'y' || c == 'Y')
            {
                continue;
            }

            else if (c != 'y' || c != 'Y' || c != 'n' || c != 'N')
            {
                printf("\nPlease enter 'y','Y' or 'n','N' only.");
                goto here;
            }
            else if (c != 'y' || c != 'Y' || c != 'n' || c != 'N')
            {
                char a = getch();
                do
                {
                    printf("\nExit the program then? (y/n) ");
                    if (a == 'n' || a == 'N')
                    {
                        printf("\nYou sure you know what you want? ");
                        printf("\n\nExiting anyway %c", 2);
                        sleep(1);
                        exit(0);
                    }

                    else if (a == 'y' || a == 'Y')
                    {
                        printf("\n\nexiting....");
                        sleep(1);
                        exit(0);
                    }
                    else
                        printf("\nPlease enter 'y','Y' or 'n','N' only.");
                } while (a != 'y' || a != 'Y' || a != 'n' || a != 'N');
            }
        }
        return client_socket;
    }
}

void welcome()
{
    usleep(300000);

    printf("\x1B[38;5;%dm\n\n\n", 196);
    printf("\t\t                ########                                       ########                     /////\\\\\\\\                \n");
    usleep(delay);

    printf("\t\t            #################     |||||                    #################               /////  \\\\\\\\               \n");
    usleep(delay);
    printf("\t\t          ################        |||||                  ################                 /////    \\\\\\\\              \n");
    usleep(delay);
    printf("\t\t       ###############            |||||               ###############                    /////      \\\\\\\\             \n");
    usleep(delay);
    printf("\t\t     ##############               |||||             ##############                      /////        \\\\\\\\            \n");
    usleep(delay);
    printf("\t\t   #####                          |||||           #####                                /////          \\\\\\\\           \n");
    usleep(delay);
    printf("\t\t  ###                             |||||          ###                                  /////            \\\\\\\\          \n");
    usleep(delay);
    printf("\t\t ###                              |||||         ###                                  /////              \\\\\\\\         \n");
    usleep(delay);
    printf("\t\t###                               |||||        ###                                  /////                \\\\\\\\        \n");
    usleep(delay);
    printf("\t\t###                               |||||        ###                                 /////                  \\\\\\\\       \n");
    usleep(delay);
    printf("\t\t###          ##########           |||||        ###          ##########            /////####################\\\\\\\\      \n");
    usleep(delay);
    printf("\t\t###         ##########            |||||        ###         ##########            /////######################\\\\\\\\     \n");
    usleep(delay);
    printf("\t\t ##               ###             |||||         ##               ###            /////                        \\\\\\\\    \n");
    usleep(delay);
    printf("\t\t  ###            ###              |||||          ###            ###            /////                          \\\\\\\\   \n");
    usleep(delay);
    printf("\t\t    ###        ###                |||||           ###        ###              /////                            \\\\\\\\  \n");
    usleep(delay);
    printf("\t\t      **######**                                    **######**               /////                              \\\\\\\\ \n\n");

    usleep(delay);
    printf("\t\t                                           ########            #####          #####                    /////\\\\\\\\             ----------------------------\n");
    usleep(delay);
    printf("\t\t                                       #################       #####          #####                   /////  \\\\\\\\            ----------------------------\n");
    usleep(delay);
    printf("\t\t                                     ################          #####          #####                  /////    \\\\\\\\           ----------------------------\n");
    usleep(delay);
    printf("\t\t                                  ###############              #####          #####                 /////      \\\\\\\\                     |||||||\n");
    usleep(delay);
    printf("\t\t                                ##############                 #####          #####                /////        \\\\\\\\                    |||||||\n");
    usleep(delay);
    printf("\t\t                              #####                            #####          #####               /////          \\\\\\\\                   |||||||\n");
    usleep(delay);
    printf("\t\t                             ###                               #####          #####              /////            \\\\\\\\                  |||||||\n");
    usleep(delay);
    printf("\t\t                            ###                                ####################             /////              \\\\\\\\                 |||||||\n");
    usleep(delay);
    printf("\t\t                           ###                                 ####################            /////                \\\\\\\\                |||||||\n");
    usleep(delay);
    printf("\t\t                           ###                                 #####          #####           /////                  \\\\\\\\               |||||||\n");
    usleep(delay);
    printf("\t\t                           ###                                 #####          #####          /////####################\\\\\\\\              |||||||\n");
    usleep(delay);
    printf("\t\t                           ###                                 #####          #####         /////######################\\\\\\\\             |||||||\n");
    usleep(delay);
    printf("\t\t                            ##                                 #####          #####        /////                        \\\\\\\\            |||||||\n");
    usleep(delay);
    printf("\t\t                             ###                               #####          #####       /////                          \\\\\\\\           |||||||\n");
    usleep(delay);
    printf("\t\t                               ###                             #####          #####      /////                            \\\\\\\\          |||||||\n");
    usleep(delay);
    printf("\t\t                                 **######**                    #####          #####     /////                              \\\\\\\\         |||||||\x1B[0m\n");
    usleep(delay);

    sleep(1);
}

int main()
{
    system("cls");
    printf("WELCOME TO GIGA CHAT.\n\n");
    printf("Enter ''/exit/'' -> exit chatroom");
    printf("\nPress any key to continue...\n");
    getch();
    system("cls");
    welcome();
    while (1)
    {
        system("cls");
        char choice = login_signup_choice();
        SOCKET sock = connection();
        switch (choice)
        {
        case '1':
            handle_login(sock);
            after_login_signup(&sock, &wsa);
            break;

        case '2':
            handle_signup(sock);
            after_login_signup(&sock, &wsa);
            break;

        case '3':
            printf("\nexiting.");
            exit(0);

        default:
            printf("\nInvalid option.");
            break;
        }
    }

    return 0;
}

char login_signup_choice()
{
    char choice;
    printf("OPTIONS:\n\n1. Login\n2. Signup\n3. Exit\n\n");
    printf("SELECT: ");

    choice = getch();
    return choice;
}

void handle_login(SOCKET client_socket)
{
    char buffer[BUFFER_SIZE];
    char username[USERNAME_SIZE];
    char password[USERNAME_SIZE];

    credentials_prompt(buffer, username, password);

    sprintf(buffer, "//*#!$login$!#*// %s %s", username, password);
    int sendresult = send(client_socket, buffer, strlen(buffer), 0);
    if (sendresult == SOCKET_ERROR)
    {
        printf("Send failed\n");
        closesocket(client_socket);
        WSACleanup();
    }

    // Receive and print the server's response
    memset(buffer, 0, sizeof(buffer));
    int recvresult = recv(client_socket, buffer, sizeof(buffer), 0);
    if (recvresult > 0)
    {
        // buffer[recvresult] = '\0';
        printf("%s\n", buffer);
    }
    else if (recvresult == 0)
    {
        printf("Connection closed by the server\n");
        closesocket(client_socket);
        WSACleanup();
    }
    else
    {
        printf("Receive failed\n");
        closesocket(client_socket);
        WSACleanup();
    }

    if (strcmp(buffer, "Login failed") == 0)
    {
        // printf("Login failed.\n");
        printf("Press enter. ");
        closesocket(client_socket);
        WSACleanup();
    }
}

void handle_signup(SOCKET client_socket)
{
    char buffer[BUFFER_SIZE];
    char username[USERNAME_SIZE];
    char password[USERNAME_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    memset(username, 0, USERNAME_SIZE);
    memset(password, 0, USERNAME_SIZE);

    credentials_prompt(buffer, username, password);
    sprintf(buffer, "//*#!$signup$!#*// %s %s", username, password);
    send(client_socket, buffer, strlen(buffer), 0);

    // Receive and print the server's response

    memset(buffer, 0, sizeof(buffer));
    int recvresult = recv(client_socket, buffer, sizeof(buffer), 0);
    if (recvresult > 0)
    {
        printf("%s\n", buffer);
    }
    else if (recvresult == 0)
    {
        printf("Connection closed by the server\n");
        closesocket(client_socket);
        WSACleanup();
    }
    else
    {
        printf("Receive failed\n");
        closesocket(client_socket);
        WSACleanup();
    }

    if (strcmp(buffer, "Signup failed") == 0)
    {
        usleep(1000000);
        closesocket(client_socket);
        WSACleanup();
    }
}

void *send_messages(void *socket_ptr)
{
    SOCKET client_socket = *((SOCKET *)socket_ptr);

    printf("\n\t\t\t\tYOUR MESSAGES");
    while (1)
    {
        char buffer[BUFFER_SIZE];
        printf("\n\n\t\t\t\t");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strlen(buffer)] = '\0';

        if (strcmp(buffer, "/exit/\n") == 0)
        {
            printf("\nexiting...");
            usleep(1000000);
            closesocket(client_socket);
            WSACleanup();
            return NULL;
        }
        //

        int sendresult = send(client_socket, buffer, strlen(buffer), 0);
        if (sendresult == SOCKET_ERROR)
        {
            printf("Send failed\n");
            closesocket(client_socket);
            WSACleanup();
            return NULL;
        }

        //
        else if (sendresult < 0)
        {
            printf("Error writing to socket");
            closesocket(client_socket);
            WSACleanup();
            return NULL;
        }
    }

    return NULL;
}

void *receive_messages(void *socket_ptr)
{
    SOCKET client_socket = *((SOCKET *)socket_ptr);
    char buffer[BUFFER_SIZE];

    while (1)
    {
        memset(buffer, 0, BUFFER_SIZE);
        //
        int recvresult = recv(client_socket, buffer, sizeof(buffer), 0);
        if (recvresult > 0)
        {
            printf("\n%s\n\n\t\t\t\t", buffer);
        }
        else if (recvresult == 0)
        {
            printf("Connection closed by the server\n");
            closesocket(client_socket);
            WSACleanup();
            sleep(1);
            return NULL;
        }
        else
        {
            printf("Receive failed\n");
            closesocket(client_socket);
            WSACleanup();
            sleep(1);
            return NULL;
        }

        //
    }

    return NULL;
}

void after_login_signup(SOCKET *client_socket, WSADATA *wsa)
{
    // thread to keep sending messages to server
    pthread_t send_thread;
    if (pthread_create(&send_thread, NULL, send_messages, client_socket) != 0)
    {
        printf("Error creating sending thread");
        closesocket(*client_socket);
        WSACleanup();
        sleep(1);
    }

    // thread to keep receiving messages from the server
    pthread_t receive_thread;
    if (pthread_create(&receive_thread, NULL, receive_messages, client_socket) != 0)
    {
        printf("Error creating receive thread");
        closesocket(*client_socket);
        WSACleanup();
        sleep(1);
    }

    // Wait for the receive_messages thread to finish
    pthread_join(receive_thread, NULL);

    // Wait for the send_messages thread to finish (client disconnected)
    pthread_join(send_thread, NULL);

    closesocket(*client_socket);
    WSACleanup();
}

void credentials_prompt(char *buffer, char *username, char *password)
{
    memset(buffer, 0, BUFFER_SIZE);
    memset(username, 0, USERNAME_SIZE);
    memset(password, 0, USERNAME_SIZE);

    printf("Enter username: ");
    scanf("%99s", username); // Read at most USERNAME_SIZE - 1 characters
    getchar();               // to eat up newline

    printf("Enter password: ");
    scanf("%99s", password); // Read at most USERNAME_SIZE - 1 characters
    getchar();
}

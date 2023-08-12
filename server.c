// SERVER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <pthread.h>

#define PORT 9000
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024
#define CREDENTIALS_SIZE 100
#define CREDENTIALS_FILE "usercredentials.txt"

SOCKET clients[MAX_CLIENTS];
int num_clients = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *handle_client(void *socket_ptr);
int is_valid_credentials(const char *username, const char *password);
int add_user(const char *username, const char *password);
void broadcast_message(const char *message, SOCKET sender_socket);
typedef struct
{
    char username[CREDENTIALS_SIZE];
    SOCKET client_socket;
} user;

user USER[MAX_CLIENTS];

int main()
{
    system("cls");
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("WSAStartup failed");
    }

    pthread_mutex_init(&mutex, NULL);

    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int clientaddrsize = sizeof(client_addr);
    pthread_t threads[MAX_CLIENTS];

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET)
    {
        printf("Error creating socket");
        WSACleanup();
        return 1;
    }

    ZeroMemory(&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
    {
        printf("Error on binding");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    listen(server_socket, MAX_CLIENTS);
    printf("\nListening on port %d...\n", PORT);

    while (1)
    {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &clientaddrsize);
        if (client_socket == INVALID_SOCKET)
        {
            printf("Error on accept");
            closesocket(client_socket);
            closesocket(server_socket);
            WSACleanup();
            return 1;
        }

        // clients[num_clients++] = client_socket;

        pthread_t thread;
        if (pthread_create(&thread, NULL, handle_client, (void *)&client_socket) != 0)
        {
            printf("Error creating thread");
            closesocket(client_socket);
            closesocket(server_socket);
            WSACleanup();
            return 1;
        }
    }

    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    pthread_mutex_destroy(&mutex);

    return 0;
}

void *handle_client(void *socket_ptr)
{
    SOCKET client_socket = *(SOCKET *)socket_ptr;

    char buffer[BUFFER_SIZE];
    clients[num_clients++] = client_socket;

    char current_username[CREDENTIALS_SIZE] = ""; // To store the current user's username

    while (1)
    {
        memset(buffer, 0, BUFFER_SIZE);
        int recvresult = recv(client_socket, buffer, sizeof(buffer), 0);
        if (recvresult <= 0)
        {
            printf("Client disconnected.\n");
            break;
        }

        char action[BUFFER_SIZE];
        char message[BUFFER_SIZE];
        char username[CREDENTIALS_SIZE];
        char password[CREDENTIALS_SIZE];

        sscanf(buffer, "%s %s %s", action, username, password);

        if (strcmp(action, "//*#!$signup$!#*//") == 0)
        {
            if (add_user(username, password))
            {
                send(client_socket, "Signup successful", strlen("Signup successful"), 0);
                strcpy(current_username, username); // Set the current username
            }
            else
            {
                send(client_socket, "Signup failed", strlen("Signup failed"), 0);
            }
        }
        else if (strcmp(action, "//*#!$login$!#*//") == 0)
        {
            if (is_valid_credentials(username, password))
            {
                send(client_socket, "Login successful", strlen("Login successful"), 0);

                strcpy(current_username, username); // Set the current username
            }
            else
            {
                send(client_socket, "Login failed", strlen("Login failed"), 0);
            }
        }
        else
        {
            // Regular chat message
            char broadcast_msg[BUFFER_SIZE];
            sprintf(broadcast_msg, "%s: %s", current_username, buffer);
            broadcast_message(broadcast_msg, client_socket);
        }
    }

    for (int i = 0; i < num_clients; i++)
    {
        if (clients[i] == client_socket)
        {
            for (int j = i; j < num_clients - 1; j++)
            {
                clients[j] = clients[j + 1];
            }
            num_clients--;
            break;
        }
    }

    closesocket(client_socket);
    return NULL;
}

int is_valid_credentials(const char *username, const char *password)
{
    FILE *file = fopen(CREDENTIALS_FILE, "r");
    if (file == NULL)
    {
        return 0;
    }

    char line[BUFFER_SIZE];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        char stored_username[CREDENTIALS_SIZE];
        char stored_password[CREDENTIALS_SIZE];
        sscanf(line, "%s %s", stored_username, stored_password);
        if (strcmp(username, stored_username) == 0 && strcmp(password, stored_password) == 0)
        {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

int add_user(const char *username, const char *password)
{
    FILE *file = fopen(CREDENTIALS_FILE, "a");
    if (file == NULL)
    {
        return 0;
    }

    fprintf(file, "%s %s\n", username, password);
    fclose(file);
    return 1;
}

void broadcast_message(const char *message, SOCKET sender_socket)
{
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < num_clients; i++)
    {
        if (clients[i] != sender_socket)
        {
            send(clients[i], message, strlen(message), 0);
        }
    }
    pthread_mutex_unlock(&mutex);
}

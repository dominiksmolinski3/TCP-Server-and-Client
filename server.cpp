#include <iostream>
#include <pthread.h>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <vector>

#define PORT 8080
#define MAX_CLIENTS 10

void *handle_client(void *client_socket) {
    int sock = *((int *)client_socket);
    char buffer[1024];
    int bytes_read;

    while ((bytes_read = read(sock, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0';
        std::string message(buffer);

        if (message == "hello") {
            std::string response = "world\n";
            write(sock, response.c_str(), response.size());
        } else {
            write(sock, message.c_str(), message.size());
        }

        if (message == "disconnect") {
            std::cout << "Client disconnected" << std::endl;
            break;
        }
    }

    close(sock);
    pthread_exit(NULL);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    pthread_t threads[MAX_CLIENTS];
    int client_count = 0;

    // create socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // bind socket to the specified IP and port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // listen for incoming connections
    if (listen(server_socket, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is listening on port " << PORT << std::endl;

    while (true) {
        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len)) < 0) {
            perror("Accept failed");
            continue;
        }

        std::cout << "New client connected" << std::endl;

        if (pthread_create(&threads[client_count], NULL, handle_client, (void *)&client_socket) != 0) {
            perror("Thread creation failed");
            close(client_socket);
        }

        client_count++;
        if (client_count >= MAX_CLIENTS) {
            std::cerr << "Max clients limit reached" << std::endl;
            break;
        }
    }

    close(server_socket);
    return 0;
}

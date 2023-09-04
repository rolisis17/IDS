#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

void monitorPort(int port) {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket." << std::endl;
        return;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error binding to port." << std::endl;
        close(serverSocket);
        return;
    }

    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Error listening on port." << std::endl;
        close(serverSocket);
        return;
    }

    std::cout << "Waiting for connections on port " << port << "..." << std::endl;

    while (true) {
        struct sockaddr_in clientAddr;
        socklen_t clientAddrSize = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrSize);
        if (clientSocket == -1) {
            std::cerr << "Error accepting connection." << std::endl;
            continue;
        }

        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);

        std::cout << "Connection from IP: " << clientIP << " on port " << port << std::endl;

        close(clientSocket);
    }

    close(serverSocket);
}

bool isPortOpen(int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        return false;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) == -1) {
        close(sock);
        return false;
    }

	std::cout << port << " is open." << std::endl;

    close(sock);
    return true;
}

int main() {
    int port = 0; // Change this to the port you want to monitor
	int count = 0;

	while (++port <= 1024)
	{
		if (isPortOpen(port))
		{
			count++;
    		monitorPort(port);
		}
	}
	if (count < 1)
		std::cout << "No ports open. Nice!" << std::endl;

    return 0;
}
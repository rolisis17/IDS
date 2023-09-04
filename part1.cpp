#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <cerrno>
#include <fcntl.h>
#include <ctime>
#include <unistd.h>
#include <arpa/inet.h>
# include <sys/wait.h>

// this massive function will try to connect to the open port
// and wait for some IP to connect. If in less than 10 seconds
// someone connect to the port it will print the IP.

int monitorPort(int port) {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Error creating socket");
        return 1;
    }

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port); // Change to your desired port

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("Error binding");
        close(serverSocket);
        return 1;
    }

    // Set the socket to non-blocking mode
    int flags = fcntl(serverSocket, F_GETFL, 0);
    if (flags == -1) {
        perror("Error getting socket flags");
        close(serverSocket);
        return 1;
    }
    if (fcntl(serverSocket, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("Error setting socket to non-blocking");
        close(serverSocket);
        return 1;
    }

    if (listen(serverSocket, 5) == -1) {
        perror("Error listening");
        close(serverSocket);
        return 1;
    }

    // Set a timeout for listening (e.g., 10 seconds)
    time_t start_time = time(NULL);
    time_t timeout = 10; // Adjust this value as needed

    while (true) {
        if (time(NULL) - start_time >= timeout) {
            std::cout << "Timeout reached. No incoming connections." << std::endl;
            break;
        }

        int clientSocket = accept(serverSocket, NULL, NULL);

        if (clientSocket == -1) {
            // Check for EWOULDBLOCK to handle non-blocking socket
            if (errno == EWOULDBLOCK) {
                // No incoming connection yet, continue waiting
                continue;
            } else {
                perror("Error accepting connection");
                break;
            }
        } else {
            std::cout << "Incoming connection accepted." << std::endl;
            // Handle the incoming connection
            close(clientSocket);
        }
    }

    close(serverSocket);
    return 0;
}

// this function is just for check if the port is open in a very simple way.

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

// this function is to run the script.

int runscript(int check)
{
    int status;
    int pid = fork();

    if (pid == 0)
    {
        if (check == 1)
            system("bash script.sh");
        if (check == 2)
            system("bash script2.sh"); 
    }
    else if (pid > 0)
        waitpid(pid, &status, 0);
    // printf("%d", WIFEXITED(status));
    return status; //WIFEXITED(status) && WEXITSTATUS(status);
}

// the main function will loop 1 to 1024 port to check if it is open and then run the script.

int main() {
    int port = 0;
	int count = 0;
    int status = 0;

    std::cout << "CHECKING PORTS: " << std::endl;
	while (++port <= 1024)
	{
		if (isPortOpen(port))
		{
			count++;
    		monitorPort(port);
            std::cout << std::endl;
		}
	}
	if (count < 1)
    {
		std::cout << "No ports open. Nice!" << std::endl;
    }
    std::cout << "CHECKING LOGIN: " << std::endl;
    if (runscript(1))
    {
        runscript(2);
    }
    return 0;
}

#include "mylittleids.h"

int exiting(const char *toPrint, int socketToClose)
{
    perror(toPrint);
    if (socketToClose != -1)
        close(socketToClose);
    return 1;
}

// this massive function will try to connect to the open port
// and wait for some IP to connect. If in less than 10 seconds
// someone connect to the port it will print the IP.

int monitorPort(int port)
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
        return exiting("Error creating socket", -1);

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port); // Change to your desired port

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
        return exiting("Error binding", serverSocket);

    // Set the socket to non-blocking mode
    int flags = fcntl(serverSocket, F_GETFL, 0);
    if (flags == -1)
        return exiting("Error getting socket flags", serverSocket);
    if (fcntl(serverSocket, F_SETFL, flags | O_NONBLOCK) == -1)
        return exiting("Error setting socket to non-blocking", serverSocket);

    if (listen(serverSocket, 5) == -1)
        return exiting("Error listening", serverSocket);


    // Set a timeout for listening (e.g., 10 seconds)
    time_t start_time = time(NULL);
    time_t timeout = 10; // Adjust this value as needed

    while (true)
    {
        if (time(NULL) - start_time >= timeout)
        {
            std::cout << "Timeout reached. No incoming connections." << std::endl;
            break;
        }

        int clientSocket = accept(serverSocket, NULL, NULL);

        if (clientSocket == -1)
        {
            // Check for EWOULDBLOCK to handle non-blocking socket
            if (errno == EWOULDBLOCK) {
                // No incoming connection yet, continue waiting
                continue;
            } else {
                perror("Error accepting connection");
                break;
            }
        }
        else
        {
            std::cout << "Incoming connection accepted." << std::endl;
            // Handle the incoming connection
            close(clientSocket);
        }
    }

    close(serverSocket);
    return 0;
}

// this function is just for check if the port is open in a very simple way.

bool isPortOpen(int port)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        return false;

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        close(sock);
        return false;
    }

	std::cout << "\033[0;91m" << port << " is open.\033[0m" << std::endl;

    close(sock);
    return true;
}

int Makescript(int check)
{
    std::ofstream script("script.sh", std::ios::trunc);

    if (script.is_open())
    {
        if (check == 1)
            script << "#!/bin/bash\n\nLOG_FILE=\"/var/log/auth.log\"\n\nif [ -r \"$LOG_FILE\" ]; then\n\tsuspicious_detected=0\n\ttail -n 0 -f \"$LOG_FILE\" | \\\n\t\twhile read line; do\n\t\t\tif echo \"$line\" | grep -q \"authentication failure\"; then\n\t\t\t\techo -e \"\\033[0;91mSuspicious login attempt detected: $line\\033[0m\"\n\t\t\tfi\n\t\tdone\n\n\tif [ $suspicious_detected -eq 0 ]; then\n\t\techo -e \"\033[0;92mNo suspicious login attempts detected.\\033[0m\"\n\tfi\nelse\n\texit 1;\nfi\n";
        else
            script << "#!/bin/bash\n\nsuspicious_detected=0\n\nwhile read line; do\n\tif echo \"$line\" | grep -q \"authentication failure\"; then\n\t\techo -e \"\\033[0;91mSuspicious login attempt detected: $line\\033[0m\"\n\t\tsuspicious_detected=1\n\tfi\n\tdone < <(journalctl -q)\n\nif [ $suspicious_detected -eq 0 ]; then\n\techo -e \"\\033[0;92mNo suspicious login attempts detected.\\033[0m\"\nfi\n";
        script.close();
        return 1;
    }
    return 0;
}

int runscript(int check)
{
    int status;

    if (!Makescript(check))
        return 1;
    int pid = fork();
    if (pid == 0)
        system("bash script.sh");
    else if (pid > 0)
        waitpid(pid, &status, 0);
    //printf("%d\n", WIFEXITED(status));
    return status; //WIFEXITED(status) && WEXITSTATUS(status);
}

// the main function will loop 1 to 1024 port to check if it is open and then run the script.

int main() {
    int port = -1;
	int count = 0;

    std::cout << "\033[0;93mCHECKING PORTS: \033[0m" << std::endl << std::endl;
 	while (++port <= 1023)
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
		std::cout << "\033[0;92mNo ports open. Nice!\033[0m" << std::endl << std::endl;
    } 
    std::cout << "\033[0;93mCHECKING LOGIN: \033[0m" << std::endl << std::endl;
    if (runscript(1))
    {
        runscript(2);
    }
    std::remove("script.sh"); 
    return 0;
}

#include <sys/socket.h>
#include <string.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <unistd.h>
#include <netinet/in.h>

void main() {
    //Create a web socket for streaming	
    int s = socket(AF_INET, SOCK_STREAM, 0); //AF_INET for IPv4, 
    struct sockaddr_in addr = {
        AF_INET,
        0x901f, //Port 8080 in HEX(0x1f90) in reverse
        0
    };

    bind(s, &addr, sizeof(addr));

    //Keep listening to incoming socket connections
    listen(s, 10);
    //Get Client info
    int client_fd = accept(s, 0, 0);
    char buffer[256] = {0};
    //Receive Client's message
    recv(client_fd, buffer, 256, 0);

    // GET /file.html .....
    //get the exact filename
    char* f = buffer + 5;
    *strchr(f, ' ') = 0;
    //open the file
    int opened_fd = open(f, O_RDONLY);
    //Call the sendfile system call to send the file back to the client
    sendfile(client_fd, opened_fd, 0, 256);
    //close the connections
    close(opened_fd);
    close(client_fd);
    close(s);
}

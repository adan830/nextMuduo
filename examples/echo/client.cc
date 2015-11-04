#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h> //for bzero
#include <iostream>
#include <unistd.h>

using namespace std;

#define MAX_LINE 100
#define MAX_EVENTS 500
#define MAX_LISTENFD 5


int main(int args, char** argv)
{
    std::cout << "Starting, lib-muduo-client" << std::endl;

    char line[MAX_LINE];

    int connfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in servaddr;
    //fcntl(listenfd, F_SETFL, O_NONBLOCK); //no-block io
    //setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(11111);

    int err = connect(connfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if (err == -1) {
        cout << "connect failed" << endl;
        return 0;
    }

    std::cout << "Say hello to server" << endl;
    std::string msg = "hello";
    if (args > 1) {
        msg = argv[1];
    }
    send(connfd, msg.c_str(), msg.length(), 0);
    int readLength = read(connfd, line, MAX_LINE);
    cout << "Message from server: " << readLength << ":" << line << endl;
    close(connfd);

    std::cout << "Goodbye, lib-muduo-client" << std::endl;
    return 0;
}


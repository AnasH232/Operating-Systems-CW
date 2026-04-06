#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

using namespace std;

#define PORT 8080

int main(){
    //defining server and address data 
    int server_fd = socket(AF_INET, SOCK_STREAM,0);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port= htons(PORT);
    addr.sin_addr.s_addr=INADDR_ANY;

    //wait for client to connect then output message to confirm
    bind(server_fd, (sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 1);
    cout<<"waiting for client"<<endl;

    int adlen = sizeof(addr);
    int client_socket = accept(server_fd, (sockaddr*)&addr, (socklen_t*)&adlen);
    cout<<"Client connected"<<endl;
    char buffer[1024];

    //loop to keep chat running until empty message sent
    while(true){
        memset(buffer,0,sizeof(buffer));
        //receive message
        int bytes=recv(client_socket, buffer,1024,0);
        if (bytes<=0||strcmp(buffer, "exit") == 0) break;
        cout << "Client: " << buffer << endl;

        //send message
        cout << "You: ";
        cin.getline(buffer, 1024);
        if (strlen(buffer) == 0) continue;

        if (strcmp(buffer, "exit") == 0) {
            cout << "Exiting chat" << endl;
            break;
        }
        send(client_socket, buffer, strlen(buffer), 0);
    }

    close(client_socket);
    close(server_fd);
}
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

using namespace std;

#define PORT 8080

int main(){
    //set up client socket and server address info
    sockaddr_in server_addr;
    int client_socket = socket(AF_INET,SOCK_STREAM,0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    //try to connect to server
    if (connect(client_socket,(sockaddr*)&server_addr,sizeof(server_addr))<0){
        cout<<"Error connecting"<<endl;
        return 0;
    }
    cout<<"Connected to server"<<endl;
    char buffer[1024];

    while (true){
        //send message
        cout << "You: ";
        cin.getline(buffer, 1024);
        if (strlen(buffer) == 0) continue;

        if (strcmp(buffer, "exit") == 0) {
            cout << "Exiting chat" << endl;
            break;
        }
        send(client_socket, buffer, strlen(buffer), 0);

        //receive message
        memset(buffer, 0, sizeof(buffer));
        int bytes = recv(client_socket, buffer, 1024, 0);
        if (bytes <= 0||strcmp(buffer, "exit") == 0) break;
        cout << "Server: " << buffer << endl;
    }

    close(client_socket);
}
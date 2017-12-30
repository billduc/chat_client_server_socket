#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

int main(){
    int client, server;
    int portNum = 1234;
    bool isExit = false;
    int bufsize = 1024;
    char buffer[bufsize];

    struct sockaddr_in server_addr;
    socklen_t size;
    
    //init socket.
    
    client = socket(AF_INET, SOCK_STREAM, 0);
    
    if (client < 0){
        cout << "Error establish connection\n";
        exit(1);
    } 
    
    cout << "server socket connection create..." << endl;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(portNum);

    if ( (bind(client , (struct sockaddr*) &server_addr, sizeof(server_addr))) < 0){
        cerr << "error binding connection, the socket has already been establish..." << endl;
        return 1;
    } 

    size = sizeof(server_addr);
    cout << "=> Looking for clients..." << endl;

    listen(client, 1);

    int clientCount = 1;
    server = accept(client,(struct sockaddr *)&server_addr,&size);

    if (server < 0) 
        cout << "=> Error on accepting..." << endl;

    while (server > 0){
        strcpy(buffer, "=> Server connected... hello message\n");
        send(server, buffer, bufsize, 0);
        cout << "=> Connected with the client #" << clientCount << ", you are good to go..." << endl;
        cout << "\n\nEnter # to end the connection\n" << endl;
        
        cout << "client : ";

        do {
            recv(server, buffer, bufsize, 0);
            cout << buffer << " ";
            if (*buffer == '#') {
                *buffer = '*';
                isExit = true;
            }
        } while (*buffer != '*');       
            
        do {
            cout << "\nServer: " << endl;
            do {
                cin >> buffer;
                send(server, buffer, bufsize, 0);
                if (*buffer == '#'){
                    send(server, buffer, bufsize, 0 );
                    *buffer = '*';
                    isExit = true;
                }
            } while (*buffer != '*');
        
            cout << "Client: ";
            do {
                recv(server, buffer, bufsize, 0);
                cout << buffer << " ";
                if (*buffer == '#') {
                    *buffer == '*';
                    isExit = true;
                }
            } while (*buffer != '*');
        
        } while (!isExit);

        cout << "\n\n=> Connection terminated with IP " << inet_ntoa(server_addr.sin_addr);
        close(server);
        cout << "\nGoodbye..." << endl;
        isExit = false;
        exit(1);      
    

    }
    close(client);
    return 0;
}

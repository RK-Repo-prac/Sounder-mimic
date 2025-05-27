#include "socket.h"

communication::communication(){
    int wsaResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsaResult != 0) {
        std::cerr << "WSAStartup failed with error: " << wsaResult << std::endl;
        exit(EXIT_FAILURE);
    }
    address_family_=AF_UNIX;
    socket_type_=SOCK_DGRAM;
    protocol_=IPPROTO_UDP;
    memset(&socket_addr_,0,sizeof(sockaddr_in));
}

int communication::create_socket(int port,int pos){
    int sock_d=socket(address_family_,socket_type_,protocol_);
    if (sock_d == INVALID_SOCKET) {
        std::cerr << "Socket initialization failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    else{
    socket_fd.insert(socket_fd.begin()+pos,sock_d);
    }
    socket_addr_.sin_port=htons(port);
    socket_addr_.sin_family=AF_UNIX;
    socket_addr_.sin_addr.s_addr=inet_addr("127.0.0.1");
    int bind_result=bind(sock_d,(sockaddr*)&socket_addr_,sizeof(socket_addr_));
    if (bind_result == SOCKET_ERROR) {
        std::cerr << "Unable to bind the port number to socket with error: " << WSAGetLastError() << std::endl;
        closesocket(sock_d);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
}

void communication::send_msg(std::string msg,int pos){
    const char* send_msg=msg.c_str();
    send(socket_fd.at(pos), send_msg, strlen(send_msg), 0);
}

std::string communication::recv_msg(int pos){
  char buffer[BUFFER_SIZE];
  int bytes_read=0;
  while(1){
  bytes_read = recv(socket_fd.at(pos), buffer, BUFFER_SIZE - 1, 0);
  if (bytes_read > 0) {
        buffer[bytes_read] = '\0';  
        std::string recv_msg(buffer);
        std::cout<<"The Message Received is: "<< recv_msg;
        return recv_msg;
    }
    else if(bytes_read == 0){
      continue;
    }
    else{
        std::cerr << "Reading Message from the socket :"<<pos<< "failed due the error: " << WSAGetLastError() << std::endl;
        closesocket(socket_fd.at(pos));
        WSACleanup();
        exit(EXIT_FAILURE);

    }

  }
}

communication::~communication(){}
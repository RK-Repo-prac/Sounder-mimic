#include "socket.h"

communication::communication(){
    int wsaResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsaResult != 0) {
        std::cerr << "WSAStartup failed with error: " << wsaResult << std::endl;
        exit(EXIT_FAILURE);
    }
    address_family_=AF_INET;
    socket_type_=SOCK_DGRAM;
    protocol_=IPPROTO_UDP;
    ss_.sin_port=htons(SENSING_SERVER_PORT);
    ss_.sin_family=address_family_;
    ss_.sin_addr.s_addr=inet_addr(SS_IP_ADDR);
}

void communication::create_socket(int port,std::string key){
    int sock_d=socket(address_family_,socket_type_,protocol_);
    if (sock_d == INVALID_SOCKET) {
        std::cerr << "Socket initialization failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in socket_addr_;
    memset(&socket_addr_,0,sizeof(sockaddr_in));
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
    socket_fd_[key]=std::make_pair(sock_d,socket_addr_);

}

void communication::send_msg(std::string msg,std::string key ,const struct sockaddr_in addr)
{
    const char* Message=msg.c_str();
    int result=sendto(socket_fd_[key].first,Message,strlen(Message),0,(sockaddr*)&addr,sizeof(addr));
    if(result<0)
    {
       std::cerr<<"Unable to send message on socket" <<key;
    }
}  

void communication::recv_msg(std::string key,std::queue<std::string> &msg,std::mutex &lock,std::condition_variable& cv){
  char buffer[BUFFER_SIZE];
  int bytes_read=0;
  while(1){
  int sockfd=socket_fd_[key].first;
  struct sockaddr_in addr=socket_fd_[key].second;
  int len=sizeof(addr);
  bytes_read = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0,(sockaddr*)&addr,&len);
  if (bytes_read > 0) {
        std::unique_lock<std::mutex> recvlock(lock);
        buffer[bytes_read] = '\0';  
        std::string recv_msg(buffer);
        std::cout<<"The Message Received on "<<key<<"is: "<< recv_msg;
        msg.push(recv_msg);
        recvlock.unlock();
        cv.notify_one();
    }
    else if(bytes_read == 0){
      continue;
    }
    else{
        std::cerr << "Reading Message from the socket :"<<key<< "failed due the error: " << WSAGetLastError() << std::endl;
        closesocket(sockfd);
        if (socket_fd_.find(key) != socket_fd_.end()) {
                socket_fd_.erase(key);}
        WSACleanup();
        exit(EXIT_FAILURE);

    }

  }
}

 struct sockaddr_in communication::get_map_addr(const std::string key){

    return socket_fd_[key].second;


 };

communication::~communication(){
    for(auto& pair : socket_fd_) {
        closesocket(pair.second.first);
    }
    WSACleanup();
}
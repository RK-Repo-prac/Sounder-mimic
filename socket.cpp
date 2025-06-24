#include "socket.h"
#include "util.h"

communication::communication(){
    address_family_=AF_INET;
    socket_type_=SOCK_DGRAM;
    protocol_=IPPROTO_UDP;
    ss_.sin_port=htons(SENSING_SERVER_PORT);
    ss_.sin_family=address_family_;
    ss_.sin_addr.s_addr=inet_addr(SS_IP_ADDR);
}

void communication::create_socket(int port,std::string key){
    int sock_d=socket(address_family_,socket_type_,protocol_);
    if (sock_d < 0) {
        std::cerr << "Socket initialization failed with error: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in socket_addr_;
    memset(&socket_addr_,0,sizeof(sockaddr_in));
    socket_addr_.sin_port=htons(port);
    socket_addr_.sin_family=AF_INET;
    socket_addr_.sin_addr.s_addr=inet_addr("127.0.0.1");
    int bind_result=bind(sock_d,(sockaddr*)&socket_addr_,sizeof(socket_addr_));
    if (bind_result < 0) {
        std::cerr << "Unable to bind the port number "<<port<< "to socket with error: " << strerror(errno) << std::endl;
        close(sock_d);
        exit(EXIT_FAILURE);
    }
    LOG("Successfully creates socket for "<<key<<" on port: "<<port);
    socket_fd_[key]=std::make_pair(sock_d,socket_addr_);
}

void communication::send_msg(std::string msg,std::string key ,const struct sockaddr_in addr,const std::string &name)
{
    const char* Message=msg.c_str();
    LOG("Sending Message to "<<name<<" on port: "<<addr.sin_port);
    int result=sendto(socket_fd_[key].first,Message,strlen(Message),0,(sockaddr*)&addr,sizeof(addr));
    if(result<0)
    {
       std::cerr<<"Unable to send message to "<<name;
    }
}  

void communication::recv_msg(std::string key,std::queue<std::string> &msg,std::mutex &lock,std::condition_variable& cv,std::function<void()> recievecallback){
  char buffer[BUFFER_SIZE];
  int bytes_read=0;
  int sockfd=socket_fd_[key].first;
  struct sockaddr_in addr=socket_fd_[key].second;
  socklen_t len=sizeof(addr);
  
  // Set socket to non-blocking mode
  int flags = fcntl(sockfd, F_GETFL, 0);
  if (flags == -1) {
      LOG("Failed to get socket flags: " << strerror(errno));
      return;
  }
  if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1) {
      LOG("Failed to set socket to non-blocking mode: " << strerror(errno));
      return;
  }
  
  while(1){
  bytes_read = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0,(sockaddr*)&addr,&len);
  if (bytes_read > 0) {
        {std::unique_lock<std::mutex> recvlock(lock);
        buffer[bytes_read] = '\0';  
        std::string recv_msg(buffer);
        LOG("The Message Received on "<<key<<"is: "<< recv_msg);
        msg.push(recv_msg);
        recvlock.unlock();}
        cv.notify_one();
        if (recievecallback) {
                recievecallback();
            }
    }
    else if(bytes_read == 0){
      continue;
    }
    else{
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            continue;
        } else {
            LOG("Socket error on " << key << ": " << strerror(errno));
            break;
        }
    }
  }
}

 struct sockaddr_in communication::get_map_addr(const std::string key){
    return socket_fd_[key].second;
 };

communication::~communication(){
    for(auto& pair : socket_fd_) {
        close(pair.second.first);
    }
}
//
//  client.hpp
//  sockdak
//
//  Created by 김재민 on 2024/01/02.
//

#ifndef client_hpp
#define client_hpp

#include <boost/asio.hpp>
#include <iostream>

#define SERVER_PORT 9000

using namespace boost::asio;
using namespace boost::asio::ip;

class Client {
    ip::tcp::endpoint ep;
    io_service ios;
    ip::tcp::socket sock;
    std::shared_ptr<io_service::work> work;
    boost::asio::detail::thread_group thread_group;
    std::string sbuf;
    std::string rbuf;
    char buf[80];
    boost::asio::detail::mutex lock;
    const int THREAD_SIZE = 4;
    
public:
    Client(std::string ip_address, unsigned short port_num);
    
    void Start();
    
private:
    void WorkerThread();
    
    void TryConnect();
    
    void OnConnect(const boost::system::error_code &ec);
    
    void Send();
    
    void Recieve();
    
    void SendHandle(const boost::system::error_code &ec);
    
    void OnReceive(const boost::system::error_code &ec, size_t size);
    
    void StopAll();
};

#endif /* client_hpp */

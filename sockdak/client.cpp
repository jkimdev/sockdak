//
//  client.cpp
//  sockdak
//
//  Created by 김재민 on 2024/01/02.
//

#include "client.hpp"
    
    Client::Client(std::string ip_address, unsigned short port_num) :
    ep(ip::address::from_string(ip_address), port_num),
    sock(ios, ep.protocol()),
    work(new io_service::work(ios)) {}
    
    void Client::Start() {
        for (int i = 0; i < THREAD_SIZE; i++)
            thread_group.create_thread(std::bind(&Client::WorkerThread, this));
        std::this_thread::sleep_for(boost::asio::chrono::milliseconds(100));
        ios.post(std::bind(&Client::TryConnect, this));
        thread_group.join();
    }
    
    void Client::WorkerThread() {
        
        ios.run();
    }
    
    void Client::TryConnect() {
        std::cout << "connecting.." << std::endl;
        
        sock.async_connect(ep, std::bind(&Client::OnConnect, this, std::placeholders::_1));
    }
    
    void Client::OnConnect(const boost::system::error_code &ec) {
        std::cout << "connected!" <<  std::endl;
        if (ec) {
            std::cout << "connect failed: " << ec.message() << std::endl;
            StopAll();
            return;
        }
        
        ios.post(std::bind(&Client::Send, this));
        ios.post(std::bind(&Client::Recieve, this));
    }
    
    void Client::Send() {
        getline(std::cin, sbuf);
        
        sock.async_write_some(boost::asio::buffer(sbuf), std::bind(&Client::SendHandle, this, std::placeholders::_1));
    }
    
    void Client::Recieve() {
        std::cout << buf << std::endl;
        sock.async_read_some(boost::asio::buffer(buf, 80), std::bind(&Client::OnReceive, this, std::placeholders::_1, std::placeholders::_2));
    }
    
    void Client::SendHandle(const boost::system::error_code &ec) {
        if (ec) {
            std::cout << "async_read_some error: " << ec.message() << std::endl;
            StopAll();
            return;
        }
        
        Send();
    }
    
    void Client::OnReceive(const boost::system::error_code &ec, size_t size) {
        if (ec) {
            std::cout << "async_write_some error: " << ec.message() << std::endl;
            StopAll();
            return;
        }
        
        if (size == 0) {
            return;
        }
        
        buf[size] = '\0';
        rbuf = buf;
        
//        lock.lock();
//        cout << rbuf << endl;
//        lock.unlock();
        Recieve();
    }
    
    void Client::StopAll() {
        sock.close();
        work.reset();
    }

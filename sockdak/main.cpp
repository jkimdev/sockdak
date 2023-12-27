//
//  main.cpp
//  sockdak
//
//  Created by Jimmy on 2023/10/19.
//

#include <boost/asio.hpp>
#include <iostream>

#define SERVER_PORT 9000

using namespace std;
using namespace boost::asio;
using namespace boost::asio::ip;


class Client {
    ip::tcp::endpoint ep;
    io_service ios;
    ip::tcp::socket sock;
    shared_ptr<io_service::work> work;
    boost::asio::detail::thread_group thread_group;
    std::string sbuf;
    std::string rbuf;
    char buf[80];
    mutex lock;
    const int THREAD_SIZE = 4;
    
public:
    Client(string ip_address, unsigned short port_num) :
    ep(ip::address::from_string(ip_address), port_num),
    sock(ios, ep.protocol()),
    work(new io_service::work(ios)) {}
    
    void Start() {
        for (int i = 0; i < THREAD_SIZE; i++)
            thread_group.create_thread(bind(&Client::WorkerThread, this));
        this_thread::sleep_for(boost::asio::chrono::milliseconds(100));
        cout << "Threads Created" << endl;
        ios.post(bind(&Client::TryConnect, this));
        thread_group.join();
    }
    
private:
    void WorkerThread() {
        lock.lock();
        cout << "[" << this_thread::get_id() << "]" << " Thread Start" << endl;
        lock.unlock();
        
        ios.run();
    }
    
    void TryConnect() {
        cout << "[" << this_thread::get_id() << "]" << " TryConnect" << endl;
        
        sock.async_connect(ep, bind(&Client::OnConnect, this, std::placeholders::_1));
    }
    
    void OnConnect(const boost::system::error_code &ec) {
        cout << "[" << this_thread::get_id() << "]" << " OnConnect" << endl;
        if (ec) {
            cout << "connect failed: " << ec.message() << endl;
            StopAll();
            return;
        }
        
        ios.post(bind(&Client::Send, this));
        ios.post(bind(&Client::Recieve, this));
    }
    
    void Send() {
        getline(std::cin, sbuf);
        
        sock.async_write_some(boost::asio::buffer(sbuf), bind(&Client::SendHandle, this, std::placeholders::_1));
    }
    
    void Recieve() {
        cout << buf << endl;
        sock.async_read_some(boost::asio::buffer(buf, 80), bind(&Client::OnReceive, this, std::placeholders::_1, std::placeholders::_2));
    }
    
    void SendHandle(const boost::system::error_code &ec) {
        if (ec) {
            cout << "async_read_some error: " << ec.message() << endl;
            StopAll();
            return;
        }
        
        Send();
    }
    
    void OnReceive(const boost::system::error_code &ec, size_t size) {
        if (ec) {
            cout << "async_write_some error: " << ec.message() << endl;
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
    
    void StopAll() {
        sock.close();
        work.reset();
    }
};

int main(int argc, const char * argv[]) {
    Client chat("192.168.45.58", SERVER_PORT);
    chat.Start();

    return 0;
}

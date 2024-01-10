# SOCKDAK

### CODE
-   `Client` 클래스의 생성자입니다. IP 주소와 포트 번호를 입력으로 받아서 클라이언트를 초기화합니다.
```cpp
Client::Client(std::string ip_address, unsigned short port_num) :
    ep(ip::address::from_string(ip_address), port_num),
    sock(ios, ep.protocol()),
    work(new io_service::work(ios)) {}
```
-   `Start` 함수는 클라이언트를 시작하는 함수로, 작업 스레드를 생성하고 서버에 연결을 시도합니다.
```cpp
void Client::Start() {
    for (int i = 0; i < THREAD_SIZE; i++)
        thread_group.create_thread(std::bind(&Client::WorkerThread, this));
    std::this_thread::sleep_for(boost::asio::chrono::milliseconds(100));
    ios.post(std::bind(&Client::TryConnect, this));
    thread_group.join();
}
```
-   `WorkerThread` 함수는 각 작업 스레드에서 실행되는 함수로, `io_service`의 `run` 함수를 호출하여 비동기 작업을 실행합니다.
```cpp
void Client::WorkerThread() {
    ios.run();
}
```
-   `TryConnect` 함수는 서버에 연결을 시도하는 함수로, 비동기 `async_connect`를 호출합니다.
```cpp
void Client::TryConnect() {
    std::cout << "connecting.." << std::endl;
    
    sock.async_connect(ep, std::bind(&Client::OnConnect, this, std::placeholders::_1));
}
```
-   `OnConnect` 함수는 서버 연결 결과를 처리하고 연결이 성공하면 데이터 송수신을 위한 함수를 호출합니다.
```cpp
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
```
-   `Send` 함수는 사용자로부터 입력을 받아 서버에 데이터를 비동기적으로 전송합니다.
```cpp
void Client::Send() {
    getline(std::cin, sbuf);
    
    sock.async_write_some(boost::asio::buffer(sbuf), std::bind(&Client::SendHandle, this, std::placeholders::_1));
}
```
-   `SendHandle` 함수는 데이터 전송 후 발생한 오류를 처리하고 재귀적으로 `Send` 함수를 호출합니다.
```cpp
void Client::SendHandle(const boost::system::error_code &ec) {
    if (ec) {
        std::cout << "async_read_some error: " << ec.message() << std::endl;
        StopAll();
        return;
    }
    
    Send();
}
```
-   `Recieve` 함수는 서버로부터 데이터를 비동기적으로 수신하고 출력합니다.
```cpp
void Client::Recieve() {
    std::cout << buf << std::endl;
    sock.async_read_some(boost::asio::buffer(buf, 80), std::bind(&Client::OnReceive, this, std::placeholders::_1, std::placeholders::_2));
}
```
-   `OnReceive` 함수는 데이터 수신 후 발생한 오류를 처리하고 데이터를 출력한 다음, 재귀적으로 `Recieve` 함수를 호출합니다.
```cpp
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
    Recieve();
}
```
-   `StopAll` 함수는 클라이언트의 모든 작업을 중지하고 소켓을 닫습니다.
```cpp
void Client::StopAll() {
    sock.close();
    work.reset();
}
```

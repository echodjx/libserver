//
// Created by echo-djx on 2021/8/5.
//
#pragma once
#include <TcpConnection/TcpServer.h>
#include <functional>
#include <TcpConnection/Logger.h>
#include <TcpConnection/Buffer.h>
#include <TcpConnection/Timestamp.h>
#include <string>
class HttpRequest;
class HttpResponse;

class HttpServer
{
public:
    using HttpCallback = std::function<void (const HttpRequest&,HttpResponse*)>;
    using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
    HttpServer(EventLoop* loop,
               const InetAddress& listenAddr,
               const std::string& name,
               TcpServer::Option option = TcpServer::kNoReusePort);

    EventLoop* getLoop() const { return server_.getLoop(); }

    /// Not thread safe, callback be registered before calling start().
    void setHttpCallback(const HttpCallback& cb)
    {
        httpCallback_ = cb;
    }

    void setThreadNum(int numThreads)
    {
        server_.setThreadNum(numThreads);
    }

    void start();
private:
    void onConnection(const TcpConnectionPtr& conn);
    void onMessage(const TcpConnectionPtr& conn,
                   Buffer* buf,
                   Timestamp receiveTime);
    void onRequest(const TcpConnectionPtr&, const HttpRequest&);

    TcpServer server_;
    HttpCallback httpCallback_;
};
//
// Created by echo-djx on 2021/9/2.
//

#include <HttpConnection/HttpServer.h>
#include <HttpConnection/HttpRequest.h>
#include <HttpConnection/HttpResponse.h>
#include <TcpConnection/EventLoop.h>
#include <TcpConnection/Logger.h>
#include <TcpConnection/Any.h>
#include <TcpConnection/Buffer.h>
#include <TcpConnection/Timestamp.h>

#include <iostream>
#include <map>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <sys/stat.h>
#include <sstream>
bool benchmark = false;

void onRequest(const HttpRequest &req, HttpResponse *resp) {
    std::cout << "Headers " << req.methodString() << " " << req.path() << std::endl;
    if (!benchmark) {
        const std::map<std::string, std::string> &headers = req.headers();
        for (const auto &header : headers) {
            std::cout << header.first << ": " << header.second << std::endl;
        }
    }
    std::string filename = req.path();
    if( filename == "/" || filename == "/index.html" ) {
            resp->setContentType("text/html");
    } else if( filename.find( ".jpg" ) != std::string::npos || filename.find( ".png" ) != std::string::npos ) {
            resp->setContentType("image/jpg");
    } else if( filename.find( ".html" ) != std::string::npos ) {
            resp->setContentType("text/html");
    } else if( filename.find( ".ico") != std::string::npos ) {
            resp->setContentType("image/x-icon");
    } else if( filename.find( ".js") != std::string::npos ) {
            resp->setContentType("yexy/javascript");
    } else if( filename.find( ".css" ) != std::string::npos ) {
            resp->setContentType("text/css");
    } else if( filename.find( ".mp3" ) != std::string::npos ) {
            resp->setContentType("audio/mp3");
    } else if( filename.find( ".mp4" ) != std::string::npos ) {
            resp->setContentType("audio/mp4");
        } else {
            resp->setContentType("text/plain");
        }
    filename = req.path().substr(1);
    if(filename == "index" || filename == "")
    {
        filename  = "index.html";
    }
    struct stat filestat;
    int ret = stat( filename.c_str(), &filestat );
    if( ret < 0 || !S_ISREG( filestat.st_mode ) )
    {  // 打开文件出错或没有该文件
        LOG_ERROR("can't find:%s \n",filename.c_str());
        resp->setStatusCode(HttpResponse::k404NotFound);
        resp->setStatusMessage("Not Found");
        resp->setCloseConnection(true);
    } else
    {
        LOG_ERROR("尝试读取文件:%s \n",filename.c_str());
        resp->setStatusCode(HttpResponse::k200Ok);
        resp->setStatusMessage("OK");
        std::string tmp;
        std::string buf;
        std::ifstream  fin(filename.c_str());
        fin.(std::locale("en_US.UTF-8"));
        while(std::getline(fin,tmp))
        {
            buf = buf + tmp;
            buf.append("\n");
        }
        fin.close();
        resp->setBody(buf);
    }
//    if (req.path() == "/") {
//        resp->setStatusCode(HttpResponse::k200Ok);
//        resp->setStatusMessage("OK");
//        resp->setContentType("text/html");
//        resp->addHeader("Server", "djx");
//        std::string now = Timestamp::now().toString();
//        resp->setBody("<html><head><title>This is title</title></head>"
//                      "<body><h1>Hello</h1>Now is " + now +
//                      "</body></html>");
//    } else if (req.path() == "/favicon.ico") {
//        resp->setStatusCode(HttpResponse::k200Ok);
//        resp->setStatusMessage("OK");
//        resp->setContentType("image/png");
//        resp->setBody(std::string(favicon, sizeof favicon));
//    }
}


int main() {
    int numThreads = 0;
    EventLoop loop;
    HttpServer server(&loop, InetAddress(8888), "dummy");
    server.setHttpCallback(onRequest);
    server.setThreadNum(numThreads);
    server.start();
    loop.loop();
    return 0;
}
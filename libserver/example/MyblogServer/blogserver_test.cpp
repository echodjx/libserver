//
// Created by echo-djx on 2021/9/2.
//
#include <HttpConnection/HttpServer.h>
#include <HttpConnection/HttpRequest.h>
#include <HttpConnection/HttpResponse.h>
#include <fstream>
#include <sys/stat.h>


bool benchmark = false;

void onRequest(const HttpRequest &req, HttpResponse *resp) {
    std::cout << "Headers " << req.methodString() << " " << req.path() << std::endl;
    if (!benchmark) {
        const std::map <std::string, std::string> &headers = req.headers();
        for (const auto &header : headers) {
            std::cout << header.first << ": " << header.second << std::endl;
        }
    }
    std::string filename = req.path();
    if (filename == "/" || filename == "/index.html") {
        resp->setContentType("text/html");
    } else if (filename.find(".jpg") != std::string::npos || filename.find(".png") != std::string::npos) {
        resp->setContentType("image/jpg");
    } else if (filename.find(".html") != std::string::npos) {
        resp->setContentType("text/html;charset=utf-8");
    } else if (filename.find(".ico") != std::string::npos) {
        resp->setContentType("image/x-icon");
    } else if (filename.find(".js") != std::string::npos) {
        resp->setContentType("yexy/javascript");
    } else if (filename.find(".css") != std::string::npos) {
        resp->setContentType("text/css");
    } else if (filename.find(".mp3") != std::string::npos) {
        resp->setContentType("audio/mp3");
    } else if (filename.find(".mp4") != std::string::npos) {
        resp->setContentType("audio/mp4");
    } else {
        resp->setContentType("text/plain");
    }
    filename = req.path().substr(1);
    if (filename == "index" || filename == "") {
        filename = "index.html";
        resp->setContentType("text/html");
    }
    if (filename.find(".") == std::string::npos) {
        filename = filename + "/index.html";
        resp->setContentType("text/html");
    }
    struct stat filestat;
    int ret = stat(filename.c_str(), &filestat);
    if (ret < 0 || !S_ISREG(filestat.st_mode)) {
        resp->setStatusCode(HttpResponse::k404NotFound);
        resp->setStatusMessage("Not Found");
        resp->setCloseConnection(true);
    } else {
        resp->setStatusCode(HttpResponse::k200Ok);
        resp->setStatusMessage("OK");
        std::string tmp;
        std::string buf;
        std::ifstream fin(filename.c_str());
        // fin.imbue(std::locale("en_US.UTF-8"));
        while (std::getline(fin, tmp)) {
            buf = buf + tmp;
            buf.append("\n");
        }
        fin.close();
        resp->setBody(buf);
    }

}


int main() {
    int numThreads = 0;
    EventLoop loop;
    HttpServer server(&loop, InetAddress(8888), "myblog");
    server.setHttpCallback(onRequest);
    server.setThreadNum(numThreads);
    server.start();
    loop.loop();
    return 0;
}
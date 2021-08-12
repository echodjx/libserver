//
// Created by echo-djx on 2021/8/5.
//

#include "HttpResponse.h"
#include <TcpConnection/Logger.h>
#include <TcpConnection/Buffer.h>
void HttpResponse::appendToBuffer(Buffer* output) const
{
    char buf[32];
    snprintf(buf, sizeof buf, "HTTP/1.1 %d ", statusCode_);
    LOG_INFO("HttpResponse=%s \n", buf);
    output->append(buf);
    output->append(statusMessage_);
    output->append("\r\n");

    if (closeConnection_)
    {
        output->append("Connection: close\r\n");
    }
    else
    {
        snprintf(buf, sizeof buf, "Content-Length: %zd\r\n", body_.size());
        output->append(buf);
        output->append("Connection: Keep-Alive\r\n");
    }

    for (const auto& header : headers_)
    {
        output->append(header.first);
        output->append(": ");
        output->append(header.second);
        output->append("\r\n");
    }

    output->append("\r\n");
    output->append(body_);
}
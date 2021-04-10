// epoll基于非阻塞I/O事件驱动
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ctype.h>
#include <sys/epoll.h>
#include <sys/errno.h>
int main(){
    int sockfd;
    socklen_t socklen;
    struct sockaddr_in svraddr;
    struct epoll_event ev;
    struct epoll_event events[1024];
    //创建socket
    //int socket(int domain, int type, int protocol);
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if(lfd<0)
    {
        perror("socket error");
        return -1;
    }
    //int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    //绑定
    struct sockaddr_in serv;
    bzero(&serv, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(8888);
    serv.sin_addr.s_addr = htonl(INADDR_ANY); //表示使用本地任意可用IP
    int ret = bind(lfd, (struct sockaddr *)&serv, sizeof(serv));
    if(ret<0)
    {
        perror("bind error");
        return -1;
    }
    //监听
    //int listen(int sockfd, int backlog);
    listen(lfd, 128);
    //创建一棵epoll树
    int epfd=epoll_create(1024);
    if(epfd<0){
        perror("create epoll error");
        return -1;
    }
    //讲lfd上epoll树
    ev.data.fd=lfd;
    ev.events=EPOLLIN;
    epoll_ctl(epfd,EPOLL_CTL_ADD,lfd,&ev);
    int nready;
    while(1){
        nready=epoll_wait(epfd,events,1024,-1);
        if(nready<0){
            perror("epoll_wait error");
            if(errno==EINTR){
                continue;
            }
            break;
        }
        for(int i=0;i<nready;i++){
            //有客户端连接请求
            sockfd=events[i].data.fd;
            if(sockfd=lfd){
                int cfd=accept(lfd,NULL,NULL);
                //将新的cfd上epoll树
                ev.data.fd=cfd;
                ev.events=EPOLLIN;
                epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&ev);
                continue;
            }
            //有客户端发送数据过来
            char buf[1024];
            memset(buf,0x00,sizeof(buf));
            int n=read(sockfd,buf,sizeof(buf));
            if(n<0){
                close(sockfd);
                //将epoll对应的事件就节点从epoll树上删除
                epoll_ctl(epfd,EPOLL_CTL_DEL,sockfd,NULL);
            }
            else{
               write(sockfd,buf,n);
            }
        }

    }
    close(epfd);
    close(lfd);
    return  0;
}
#include <iostream>
#include "test.pb.h"

using namespace fixbug;
//protoc test.proto --cpp_out=./
//g++ main.cc test.pb.cc -lprotobuf
int main(){
//    LoginResponse rsp;
//    ResultCode *rc = rsp.mutable_result();
//    rc->set_errcode(1);
//    rc->set_errmsg("登录处理失败");

      GetFriendListsResponse rsp;
      ResultCode *rc = rsp.mutable_result();
      rc->set_errcode(0);

      User *user1 = rsp.add_friend_list();
      user1->set_name("zhang san");
      user1->set_age(20);
      user1->set_sex(User::MAN);

      User *user2 = rsp.add_friend_list();
      user2->set_name("li si");
      user2->set_age(22);
      user2->set_sex(User::MAN);

      std::cout << rsp.friend_list_size() << std::endl;
/*    //封装login请求对象数据
    LoginRequest req;
    req.set_name("duan jia xin");
    req.set_pwd("123456");
    //对象数据序列化
    std::string send_str;
    if (req.SerializeToString(&send_str)){
        std::cout<<send_str.c_str()<<std::endl;
    }

    //反序列化一个login对象
    LoginRequest reqB;
    if(reqB.ParseFromString(send_str)){
        std::cout<< reqB.name()<<std::endl;
        std::cout<< reqB.pwd()<<std::endl;
    }*/
    return 0;
}
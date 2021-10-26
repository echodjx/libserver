callee :
	g++ -o consumer   ../user.pb.cc calluserservice.cpp   -lprotobuf -lsrpc -lTcpConnection -lpthread -g
	g++ -o consumer2   ../friend.pb.cc callfriendservice.cpp   -lprotobuf -lsrpc -lTcpConnection -lpthread -g
#-L./ -Wl,--copy-dt-needed-entrie
clean :
	rm -f userservice
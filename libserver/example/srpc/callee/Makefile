callee :
	g++ -o provider   ../user.pb.cc userservice.cpp   -lprotobuf -lsrpc -lTcpConnection -lpthread -g
	g++ -o provider2  ../friend.pb.cc  friendservice.cpp   -lprotobuf -lsrpc -lTcpConnection -lpthread -g
#-L./ -Wl,--copy-dt-needed-entrie
clean :
	rm -f userservice
	rm -f friendservice

callee :
	g++ -o provider   ../user.pb.cc userservice.cpp   -lsrpc -lprotobuf  -lTcpConnection -lpthread -g
	g++ -o provider2  ../friend.pb.cc  friendservice.cpp   -lsrpc -lprotobuf  -lTcpConnection -lpthread -g
#-L./ -Wl,--copy-dt-needed-entrie
clean :
	rm -f userservice
	rm -f friendservice

#include<iostream>
#include<winsock.h> // the definitions of the functions included here are defined in the lib wsock32.lib
using namespace std;

#define PORT 9909

struct sockaddr_in srv;
/* struct sockaddr_in{
	short sin_family;
	u_short sin_port;
	struct in_addr sin_addr; (machine addr)
	char sin_zero[8]; (initialize with zero)
}*/
typedef UINT_PTR SOCKET; // UINT_PTR is of type unsigned_int64
fd_set fr,fw,fe;
int nMaxFd;
/* typedef struct fd_set{
	u_int fd_count; (number of socket descriptors set for a particular socket descriptor)
	SOCKET fd_array[FD_SETSIZE]; (an array of SOCKETs; FD_SETSIZE is 64)
}fd_set;*/
// fr : for reading, fw : for writing , fe : for showing exceptions or any kinda network errors
// every object placed in the fd_set is a kernel object pointing to a particular location is kernel containg an open connection over the network, file descriptor

int main()
{
	int nRet = 0;
	// initialize the WSA variables; which are used for enabling the APIs which will support the environment for socket programming in Windows;unless it socket will not be opened
	WSADATA ws;
	if(WSAStartup(MAKEWORD(2,2), &ws) < 0) cout<<"initialization failed"<<endl;
	else cout<<"success"<<endl;
	
	
	//initialize the socket (which will be listening for the client request)
	
	int nsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // like a file descriptor, allows to read and write through network but it's a int not pointer
	// socket(addr_family for TCP,UDP, socket type, protocol) if used 0 as protocol that means underlined protocol, system protocol mostly HTTP protocolS
	// this is the socket descriptor on which we are going to read for any new incomming connection
	if(nsocket < 0) cout<<"the socket not opened"<<endl;
	else cout<<"the socket opened successfully"<<endl;
	
	// initialize the environment for the sockaddr structure
	
	srv.sin_family = AF_INET;
	srv.sin_port = htons(PORT); // host to network short (little endian to big endian byte order conversion)
	srv.sin_addr.s_addr = INADDR_ANY; // this macro provides the system IP address
	// another way to write this srv.sin_addr.s_addr = inet_addr("ip addr")
	memset(&(srv.sin_zero), 0, 8);
	
	// about blocking(IO) vs nonblocking(NIO) sockets
	// every socket by default is  ablocking socket
	// in case of blocking sockets the server is always multithreaded where one thread keeps listening to the incomming requests and collects them when 
	// the other threads works in fulfilling those requests i.e when one thread waits for the response to come other threads work for new requests
	u_long optval = 0; // 0 means blocking !=0 means nonblocking sockets
	
	nRet = ioctlsocket(nsocket, FIONBIO, &optval); // FIONBIO sets/clears nonblocking I/O
	// ioctlsocket system call sets the socket type as blocking or non blocking sockets
	if(nRet != 0) cout<<"ioctlsocket call failed"<<endl;
	else cout<<"ioctlsocket call passed"<<endl;
	
	
	
	// bind the socket to the local port (all the services running in the system will have the same addr, system addr, but the clients need to know
	// the specific addr of the services they ask for for that we need to connect through the local port)
	
	nRet = bind(nsocket, (sockaddr*)&srv, sizeof(sockaddr));
	
	if(nRet < 0) {
		cout<<"bind failed"<<endl;
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else cout<<"bind successful"<<endl;
	
	
	// listen the client request (queues the requents)
	nRet = listen(nsocket, 5) // no of backlog is 5, at a time server can pull 5 requests i.e. active queue size is 5
	if(nRet < 0){
		cout<<"failed to listen to local port"<<endl;
		WSACleanup();
		exit(Exit_FAILURE);
	}
	else cout<<"started listening"<<endl;
	nMaxFd = nsocket; // need for the select call
	struct timeval tv;
	tv.tv_sec = 1; //(1 sec)
	tv.tv_usec = 0; //(0 microsec)
	
	// select system call is used for wait on the socket descriptor, this call tells us how many socket descriptors are ready to read, or ready to write or throwing exceptions
	
	// keep waiting the new request and proceed as per the new request (this will work in a loop and signal handeling is also done in here)
	
	while(1)
	{
		FD_ZERO(&fr);
		FD_ZERO(&fw);
		FD_ZERO(&fe);
		FD_SET(nsocket,&fr);
		FD_SET(nsocket,&fe);
		
		cout<<"before select call: "<<fr.fd_count<<endl;
		
		nRet = select(nMaxFd+1, &fr, &fw, &fe, &tv); // from 0 to nMaxFd+1 it checks one by one wheather this file descriptor is ready or not
		// unsets all the file descriptor to see wheather there's any ready so we need to again set them after every select call otherwise it's going to fail
		// tv : timevalue, the ammount of time we spend to wait to see if any of these descriptors are ready
		if(nRet > 0){
			// when someone connects or communicates with a message over a dedicated connection
		}
		else if(nRet == 0){
			// no connection or any communication request made
			cout<<"nothing on port: "PORT<<endl;
		}
		else{
			// it failed and now your application should show some useful message
			WSACleanup();
			exit(EXIT_FAILURE);
		}
		cout<<"after select call: "<<fr.fd_count<<endl;
	}
		
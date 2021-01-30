#ifndef client_hpp
#define client_hpp

#include "../sudoku-utils/log.hpp"

#if defined LINUX && defined WINDOWS
#error "invalid parameters-Windows and Linux at the same time?"
#endif

#if !defined LINUX && !defined WINDOWS
#error "invalid parameters-nothing"
#endif

#ifdef WINDOWS
#define WIN32_LEAN_AND_MEAN
#define SHUT_RDWR SD_BOTH
#define SHUT_RD SD_RECEIVE
#define SHUT_WR SD_SEND
#define close closesocket
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#include <Windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#ifdef LINUX
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <netinet/in.h> //using to create struct
#endif

#include <cstring> 
#include <iostream>
#include <csignal>


class _connection
{
	protected:
		int *fd;//fd of client
		unsigned short type=0;//1=ipv4 2=ipv6
		struct sockaddr_in addr4; //server connection data
		struct sockaddr_in6 addr6; //server connection data
		_log *l;//logsystem for user
	public:
		_connection(int*, struct sockaddr_in, _log *);
		_connection(int*, struct sockaddr_in6, _log *);
		int reconnect(void);
		int recieve_data(char *, unsigned int);
		int send_data(char *, unsigned int);
		int recieve_data(char *, unsigned int, int);
		int send_data(char *, unsigned int, int);
};

class _client
{
	private:
		int fd = 0;
		struct sockaddr_in address4; 
		const int addrlen4 = sizeof(address4);
		struct sockaddr_in6 address6; 
		const int addrlen6 = sizeof(address6);
		int connect4(void(*)(_connection*), _log *);
		int connect6(void(*)(_connection*), _log *);
	public:
		int setup(const char[], unsigned short, void(*)(_connection*), _log *);//init connection, return 0 if successful
};
#endif
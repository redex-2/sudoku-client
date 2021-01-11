#ifndef client_hpp
#define client_hpp

#include "../sudoku-utils/log.hpp"
#include <netinet/in.h> //using to create struct

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
		int fd;
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
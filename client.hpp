#ifndef client_hpp
#define client_hpp

#include "../sudoku-utils/log.hpp"
#include <netinet/in.h> //using to create struct

class _connection
{
	protected:
		int *fd;//fd of client
	public:
		_connection(int*, _log*);
		int recieve_data(char *, unsigned int);
		int send_data(char *, unsigned int);
		int recieve_data(char *, unsigned int, int);
		int send_data(char *, unsigned int, int);
};

class _client
{
	private:
		int fd;
		struct sockaddr_in6 address6; 
		int addrlen6 = sizeof(address6);
		struct sockaddr_in address4; 
		int addrlen4 = sizeof(address4);
		int connect4(void(*)(_connection*), _log *);
		int connect6(void(*)(_connection*), _log *);
	public:
		int setup(const char[], unsigned short, void(*)(_connection*), _log *);//init connection, return 0 if successful
};
#endif
#include "client.hpp"


_connection::_connection(int* f, struct sockaddr_in addr, _log* logsystem)
{
	fd = f;
	l = logsystem;
	addr4 = addr;
	addr6 = { 0 };
	type = 1;
}

_connection::_connection(int *f, struct sockaddr_in6 addr, _log *logsystem)
{
	fd = f;
	l = logsystem;
	addr6 = addr;
	addr4 = { 0 };
	type = 2;
}

int _connection::reconnect(void)
{
	if(type==1)
		return connect( *fd, (struct sockaddr *)&addr4, sizeof(addr4));
	else
		return connect( *fd, (struct sockaddr *)&addr6, sizeof(addr6));
}

int _connection::recieve_data(char *result, unsigned int size)//recieve data from server
{
	return recv( *fd, result, size, 0);
}

int _connection::recieve_data(char *result, unsigned int size, int flag)//recieve data from server
{
	return recv( *fd, result, size, flag);
}

int _connection::send_data(char *data, unsigned int size)//recieve data from server
{
	return send( *fd, data, size, 0);
}

int _connection::send_data(char *data, unsigned int size, int flag)//recieve data from server
{
	return send( *fd, data, size, flag);
}

int _client::connect4(void(*f)(_connection*), _log* logsystem)
{
	if (connect(fd, (struct sockaddr *)&address4, sizeof(address4)) < 0) 
    { 
        logsystem->write("Connection Failed"); 
        return -1; 
    } 
	_connection conn(&fd, address4, logsystem);
	f(&conn);
	close(fd);
	return 1;
}

int _client::connect6(void(*f)(_connection*), _log* logsystem)
{
	if (connect(fd, (struct sockaddr *)&address6, sizeof(address6)) < 0) 
    { 
        logsystem->write("Connection Failed");
        return -1; 
    } 
	_connection conn(&fd, address6, logsystem);
	f(&conn);
	close(fd);
	return 1;
}

int _client::setup(const char addr[], unsigned short port, void(*f)(_connection*), _log* logsystem)
{
#ifdef WINDOWS
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		logsystem->write((std::string)"Error: WSAStartup failed");
		std::cout << "Fatal error - can't continue - check log" << std::endl;
		exit(EXIT_FAILURE);
	}
#endif

    address4.sin_family = AF_INET; 
    address4.sin_port = htons(port); 
    address6.sin6_family = AF_INET6; 
    address6.sin6_port = htons(port); 
#ifdef LINUX
	std::signal(SIGPIPE, SIG_IGN);
#endif	
	
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, addr, &address4.sin_addr)>0)  
    { 
		if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
		{ 
			logsystem->write("Socket creation error");
			return -1; 
		} 
		if(!connect4(f, logsystem))
			return -1;
    } 
	else if(inet_pton(AF_INET6, addr, &address6.sin6_addr)>0)
	{
		if ((fd = socket(AF_INET6, SOCK_STREAM, 0)) < 0) 
		{ 
			logsystem->write("Socket creation error");
			return -1; 
		} 
		if(!connect6(f, logsystem))
			return -1;
	}
	else
	{
		logsystem->write("Check adress: "+(std::string)addr);
		return -1;
	}
    return 0; 
}
#include "client.hpp"

#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <cstring> 
#include <iostream>
#include <csignal>

_connection::_connection(int *f, log * logsystem)
{
	l=logsystem;
	fd=f;
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

int _client::connect4(void(*f)(_connection*), log* logsystem)
{
	if (connect(fd, (struct sockaddr *)&address4, sizeof(address4)) < 0) 
    { 
        logsystem->write("Connection Failed"); 
        return -1; 
    } 
	_connection conn(&fd, logsystem);
	f(&conn);
	close(fd);
	return 1;
}

int _client::connect6(void(*f)(_connection*), log* logsystem)
{
	if (connect(fd, (struct sockaddr *)&address6, sizeof(address6)) < 0) 
    { 
        logsystem->write("Connection Failed");
        return -1; 
    } 
	_connection conn(&fd, logsystem);
	f(&conn);
	close(fd);
	return 1;
}

int _client::setup(const char addr[], unsigned short port, void(*f)(_connection*), log* logsystem)
{
   
    address4.sin_family = AF_INET; 
    address4.sin_port = htons(port); 
    address6.sin6_family = AF_INET6; 
    address6.sin6_port = htons(port); 
	
    signal(SIGPIPE, SIG_IGN);//ignore SIGPIPE - you must change return value and errno, program do not exit when signal
	
	
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
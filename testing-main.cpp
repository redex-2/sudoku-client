#include <iostream>
#include <csignal>
#include "../sudoku-utils/log.hpp"
#include "client.hpp"
#include <cstring>

_log logsystem;
_client cli;

void test (_connection * connection)
{
	char buff[1024]="";
	std::cout<<connection->send_data((char*)"hello from client", strlen("hello from client"))<<"\t"<<connection->recieve_data(buff, 1024)<<"\t"<<buff<<std::endl;
}

void signalHandler( int signum ) {
	std::cout << "\n\nInterrupt signal (" << signum << ") received.\nTurning off...\n";

	logsystem.write("turned off by interrupt");
	exit(signum);
}

int main()
{
	signal(SIGINT, signalHandler);
	logsystem.setup("client.txt");
	logsystem.write("main-client started");
	cli.setup("::1", 8888, test, &logsystem);
	return 1;
}
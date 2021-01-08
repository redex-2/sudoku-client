#include <iostream>
#include <csignal>
#include "log.hpp"
#include "client.hpp"
#include <cstring>

log logsystem;
_client cli;
void test (_connection * connection)
{
	std::cout<<"test\n";
	char buff[1024]="";
	std::cout<<connection->send_data("hello from client", strlen("hello from client"))<<"\t"<<connection->recieve_data(buff, 1024)<<"\t"<<buff<<std::endl;
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
	cli.setup("127.1.1.1", 8888, test, &logsystem);
	return 1;
}
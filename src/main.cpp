#include <iostream>

#include "Server.h"
#include "Client.h"

using namespace std;

void server(const char* host, const char* port) {
	try{
		Server s(host, port);
		s.start();
	}
	catch (std::string& e) { // catch errors thrown as strings
		cerr << e << endl;
	}
	catch (const std::exception& e) { // catch other exceptions
		cerr << e.what();
	}
	catch (...) {
		cerr << "Caught and exception of unknown type ..";
	}
}

void client(const char* host, const char* port) {
	try {
		Client c(host, port);
		c.start();
	}
	catch (std::string& e) { // catch errors thrown as strings
		cerr << e << endl;
	}
	catch (const std::exception& e) { // catch other exceptions
		cerr << e.what();
	}
	catch (...) {
		cerr << "Caught and exception of unknown type ..";
	}
}

int main(int argc, char **argv) {

	if (argc == 4 && strcmp(argv[1], "server") == 0) {
		server(argv[2], argv[3]); // start in server mode
	}
	else if (argc == 4 && strcmp(argv[1], "client") == 0) {
		client(argv[2], argv[3]); // start in client mode
	}
	else {
		cout << "Usage: " << endl;
		cout << "  " << argv[0] << " server host port " << endl;
		cout << "  " << argv[0] << " client host port" << endl;
		cout << endl;
		cout << "Example:" << endl;
		cout << "  " << argv[0] << " server localhost 2000" << endl;
		cout << "  " << argv[0] << " client 127.0.0.1 2000" << endl;
	}

	return 0;
}

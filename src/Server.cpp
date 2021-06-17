#include "Server.h"

#include "ServerMsg.h"
#include "ClientMsg.h"

#include <assert.h>
#include <iostream>

using namespace std;

constexpr float timeWait = 1000.0f/60.0f;

Server::Server(const char* address, const char* port) :
	socket(address, port),
	clientSocket(nullptr)
{
	socket.bind();
	game = new Game();
}

Server::~Server() {
	
}

void Server::inputThread() {
	while(true){
		ClientMsg::InputMsg msg;

		Socket* clientSocket = (Socket*)1;
		socket.recv(msg, clientSocket);

		cout << msg.input << flush << endl;

		if(msg.input == ClientMsg::InputId::_READY_){
			//empezar partida (de momento)
			game->start(&socket, clientSocket);
		}
		else if(msg.input == ClientMsg::InputId::_LOGOUT_){
			//parar partida (de momento)
			delete game; game = nullptr;
			break;
		}
		else {
			//Borro la memoria auxiliar
			game->setPlayerInput(msg.input);
		}
	}
}

void Server::start() {
	//Make InputThread
	inputThread();
}


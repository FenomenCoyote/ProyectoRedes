#include "Server.h"

#include "ServerMsg.h"
#include "ClientMsg.h"

#include <assert.h>
#include <iostream>

using namespace std;

constexpr float timeWait = 1000.0f/60.0f;

Server::Server(const char* address, const char* port) :
	socket(address, port),
	game(nullptr)
{
	socket.bind();
}

Server::~Server() {
	
}

void Server::start() {
	while(true){
		ClientMsg::InputMsg msg(ClientMsg::InputId::_AHEAD_);

		Socket* clientSocket = (Socket*)1;
		socket.recv(msg, clientSocket);
		
		if(msg.input == ClientMsg::InputId::_READY_ && game == nullptr){
			//empezar partida (de momento)
			game = new Game(socket);
			game->start(clientSocket);
		}
		else if(msg.input == ClientMsg::InputId::_LOGOUT_){
			delete game; game = nullptr;
		}
	}
}


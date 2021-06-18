#include "Server.h"

#include "ServerMsg.h"
#include "ClientMsg.h"

#include <assert.h>
#include <iostream>

using namespace std;

constexpr float timeWait = 1000.0f/60.0f;

Server::Server(const char* address, const char* port) :
	socket(address, port),
	clients(),
	game(nullptr)
{
	socket.bind();
}

Server::~Server() {
	
}

void Server::start() {
	ClientMsg::InputMsg msg(ClientMsg::InputId::_AHEAD_);

	while(true){
		Socket* clientSocket = (Socket*)1;
		socket.recv(msg, clientSocket);
		
		if(msg.input == ClientMsg::InputId::_READY_ && game == nullptr && clients.size() < 2){
			clients.push_back(std::unique_ptr<Socket>(std::move(clientSocket)));
			if(clients.size() == 2){
				game = new Game(socket, clients[0], clients[1]);
				game->start();
			}
			clients.clear();
		}
		else if(msg.input == ClientMsg::InputId::_LOGOUT_){
			delete game; game = nullptr;
		}
	}
}


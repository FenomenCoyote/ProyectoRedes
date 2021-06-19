#include "Server.h"

#include "ServerMsg.h"
#include "ClientMsg.h"
#include "ServerSDLGame.h"

#include <assert.h>
#include <iostream>
#include <mutex>

using namespace std;

constexpr float timeWait = 1000.0f/60.0f;

mutex serverMutex;

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
	ServerSDLGame::init(_WINDOW_WIDTH_, _WINDOW_HEIGHT_);

	//2 thread to satisfy each player
	//net thread 1
	std::thread([this](){
		this->netThread();
	}).detach();

	//net thread 2
	std::thread([this](){
		this->netThread();
	}).detach();

	while(true){
		serverMutex.lock();
		if(clients.size() >= 2){	
			serverMutex.unlock();
			game = new Game(socket, clients[0], clients[1]);
			game->start();

			serverMutex.lock();
			delete game; game = nullptr;	
			clients.pop_front();
			clients.pop_front();
			serverMutex.unlock();
		}
		else {
			serverMutex.unlock();
		}
	}
}
	
void Server::netThread(){

	ClientMsg::InputMsg msg(ClientMsg::InputId::_AHEAD_);
	while(true){
		Socket* sd = (Socket*)1;

		socket.recv(msg, sd);

		serverMutex.lock();
		switch (msg.input)
		{
			case ClientMsg::InputId::_READY_:
				clients.push_back(std::unique_ptr<Socket>(std::move(sd)));
				break;
			case ClientMsg::InputId::_LOGOUT_: {
				auto it = clients.begin();
				while( it != clients.end() ){
					if( it->get() == sd ){
						clients.erase(it);
						break;
					}
					++it;
				}
				break;
			}
			default:{
				if(game != nullptr){
					int player = (*sd == *clients[0].get()) ? 0 : 1;
					game->setPlayerInput(msg.input, player);
				}
				break;
			}
		}	
		serverMutex.unlock();
	}
}

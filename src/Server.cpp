#include "Server.h"

#include "ServerMsg.h"
#include "ClientMsg.h"
#include "ServerSDLGame.h"

#include <assert.h>
#include <iostream>
#include <mutex>
#include <thread>

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

	//2 thread for each player
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
		//If i can start a game
		if(clients.size() == 2){	
			serverMutex.unlock();
			//Creates and runs game (only in this thread)
			game = new Game(socket, clients.front(), clients.back());
			game->start();
			//Game finished
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
				cout << "Player conected ..." << endl;
				clients.push_back(std::unique_ptr<Socket>(std::move(sd)));
				break;
			case ClientMsg::InputId::_LOGOUT_: {
				if(game != nullptr){
					cout << "Player disconected ..." << endl;
					int player = (*sd == *(clients.front()).get()) ? 0 : 1;
					game->playerDied(player);
				}
				else {
					auto it = clients.begin();
					while( it != clients.end() ){
						if( it->get() == sd ){
							clients.erase(it);
							break;
						}
						++it;
					}
				}
				break;
			}
			default:{
				if(game != nullptr){
					int player = (*sd == *(clients.front()).get()) ? 0 : 1;
					game->setPlayerInput(msg.input, player);
				}
				break;
			}
		}	
		serverMutex.unlock();
	}
}

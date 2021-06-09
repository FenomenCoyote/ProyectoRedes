#include "ServerMsg.h"

namespace ServerMsg
{
    void StartingGameMSg::to_bin() 
    {
        alloc_data(size);

        memset(_data, 0, size);

        char* pointer = _data;

        memcpy(pointer, &type, sizeof(ServerMsgId));
    }
    
    int StartingGameMSg::from_bin(char * data) 
    {
        alloc_data(size);

        char* pointer = bobj;

        memcpy(&type, pointer, sizeof(ServerMsgId));

        return 0;
    }

    void WorldStateMSg::to_bin() 
    {
        size_t totalSize = 0;
        uint8_t numOfAsteroids = asPool->getNumOfAsteroid();
        uint8_t numOfBullets = buPool->getNumOfBullets();

        totalSize += sizeof(uint8_t) + numOfAsteroids * objectInfoSize; //Asteroids
        totalSize += sizeof(uint8_t) + numOfBullets * objectInfoSize; //Bullets
        totalSize += objectInfoSize; //Ship
        totalSize += sizeof(uint8_t); //Health

        alloc_data(totalSize);

        memset(_data, 0, size);

        char* pointer = _data;

        //Asteroids
        memcpy(pointer, &numOfAsteroids, sizeof(uint8_t));
        pointer += sizeof(uint8_t);  
        for (Asteroid* a : asPool->getPool()) {
            if(a->getInUse()){
                to_bin_object(pointer, a->getPos(), a->getWidth(), a->getHeight(), a->getRot());
                pointer += objectInfoSize;
            }
        }

        //Bullets
        memcpy(pointer, &numOfBullets, sizeof(uint8_t));
        pointer += sizeof(uint8_t);
        for (Bullet* b : buPool->getPool()) {
            if(b->getInUse()){
                to_bin_object(pointer, b->getPos(), b->getWidth(), b->getHeight(), b->getRot());
                pointer += objectInfoSize;
            }
        }

        //Ship
        to_bin_object(pointer, ship->getPos(), ship->getWidth(), ship->getHeight(), ship->getRot());
        pointer += objectInfoSize;

        //Health
        memcpy(pointer, &health->getHp(), sizeof(uint8_t));
    }

    void WorldStateMSg::to_bin_object(char* pointer, Vector2D pos, int width, int height, double rot) 
    {
        //Pos
        memcpy(pointer, &pos.getX(), sizeof(double));
        pointer += sizeof(double);
        memcpy(pointer, &pos.getY(), sizeof(double));
        pointer += sizeof(double);
        //Size
        memcpy(pointer, &width, sizeof(int));
        pointer += sizeof(int);
        memcpy(pointer, &height, sizeof(int));
        pointer += sizeof(int);
        //Rot
        memcpy(pointer, &rot, sizeof(double));
        pointer += sizeof(double);
    }
    
    int WorldStateMSg::from_bin(char * data) 
    {
        return -1;
    }
}


// void ChatMessage::to_bin()
// {
//     alloc_data(MESSAGE_SIZE);

//     memset(_data, 0, MESSAGE_SIZE);

//     //Serializar los campos type, nick y message en el buffer _data

//     char* pointer = _data;

//     memcpy(pointer, &type, sizeof(uint8_t));
//     pointer += sizeof(uint8_t);

//     memcpy(pointer, nick.c_str(), 8);
//     pointer += 8;
    
//     memcpy(pointer, message.c_str(), 80);

// }

// int ChatMessage::from_bin(char * bobj)
// {
//     alloc_data(MESSAGE_SIZE);

//     //Reconstruir la clase usando el buffer _data

//     char* pointer = bobj;

//     memcpy(&type, pointer, sizeof(uint8_t));
//     pointer += sizeof(uint8_t);

//     char _nick_[8];

//     memcpy(&_nick_, pointer, 8);
//     pointer += 8;
//     nick = _nick_;
    
//     char _msg_[80];

//     memcpy(&_msg_, pointer,80);
//     message = _msg_;

//     return 0;
// }
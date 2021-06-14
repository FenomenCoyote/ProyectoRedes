#include "ServerMsg.h"

namespace ServerMsg
{
    void StartingGameMsg::to_bin() 
    {
        alloc_data(size);

        memset(_data, 0, size);

        char* pointer = _data;

        memcpy(pointer, &type, sizeof(ServerMsgId));
    }
    
    int StartingGameMsg::from_bin(char * bobj) 
    {
        alloc_data(size);

        char* pointer = bobj;

        memcpy(&type, pointer, sizeof(ServerMsgId));

        return 0;
    }

    void EndingGameMsg::to_bin() 
    {
        alloc_data(size);

        memset(_data, 0, size);

        char* pointer = _data;

        memcpy(pointer, &type, sizeof(ServerMsgId));
    }
    
    int EndingGameMsg::from_bin(char * bobj) 
    {
        alloc_data(size);

        char* pointer = bobj;

        memcpy(&type, pointer, sizeof(ServerMsgId));

        return 0;
    }

    void WorldStateMsg::to_bin() 
    {
        size_t totalSize = sizeof(size_t);
        uint8_t numOfAsteroids = asPool->getNumOfAsteroid();
        uint8_t numOfBullets = buPool->getNumOfBullets();

        totalSize += sizeof(uint8_t) + numOfAsteroids * objectInfoSize; //Asteroids
        totalSize += sizeof(uint8_t) + numOfBullets * objectInfoSize; //Bullets
        totalSize += objectInfoSize; //Ship
        totalSize += sizeof(uint8_t); //Health
        totalSize += sizeof(SoundId); //Sound

        alloc_data(totalSize);

        memset(_data, 0, size);

        char* pointer = _data;

        memcpy(pointer, &totalSize, sizeof(size_t));
        pointer += sizeof(size_t);

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
        memcpy(pointer, &(uint8_t)health->getHp(), sizeof(uint8_t));
        pointer += sizeof(uint8_t);

        //Sound
        memcpy(pointer, &sound, sizeof(SoundId));
    }

    void WorldStateMsg::to_bin_object(char* pointer, Vector2D pos, int width, int height, double rot) 
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
    }
    
    ObjectInfo WorldStateMsg::from_bin_object(char* pointer) 
    {
        ObjectInfo obj;

        //Pos
        memcpy(&obj.posX, pointer, sizeof(double));
        pointer += sizeof(double);
        memcpy(&obj.posY, pointer, sizeof(double));
        pointer += sizeof(double);
        //Size
        memcpy(&obj.width, pointer,sizeof(int));
        pointer += sizeof(int);
        memcpy(&obj.height, pointer, sizeof(int));
        pointer += sizeof(int);
        //Rot
        memcpy(&obj.rot, pointer,sizeof(double));

        return obj;
    }
    
    int WorldStateMsg::from_bin(char * bobj) 
    {
        size_t totalSize;
        memcpy(&totalSize, bobj, sizeof(size_t));

        alloc_data(totalSize - sizeof(size_t));

        char* pointer = bobj + sizeof(size_t);

        uint8_t numOfAsteroids;
        uint8_t numOfBullets;

        //Asteroids
        memcpy(&numOfAsteroids, pointer, sizeof(uint8_t));
        pointer += sizeof(uint8_t);  
        for (int i = 0; i < numOfAsteroids; ++i) {
            asteroids.push_back(from_bin_object(pointer));
            pointer += objectInfoSize;
        }

        //Bullets
        memcpy(&numOfBullets, pointer, sizeof(uint8_t));
        pointer += sizeof(uint8_t);  
        for (int i = 0; i < numOfBullets; ++i) {
            bullets.push_back(from_bin_object(pointer));
            pointer += objectInfoSize;
        }

        //Ship
        ship = from_bin_object(pointer);
        pointer += objectInfoSize;

        //Health
        memcpy(&(uint8_t)health->getHp(), pointer, sizeof(uint8_t));
        pointer += sizeof(uint8_t);

        //Sound
        memcpy(&sound, pointer, sizeof(SoundId));

        return 0;
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
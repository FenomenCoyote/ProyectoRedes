#include "ServerMsg.h"

namespace ServerMsg
{
    void ServerMsg::to_bin() 
    { 
        size_t totalSize = sizeof(size_t); //totalSize
        uint8_t numOfAsteroids = (asPool == nullptr) ? 0 : asPool->getNumOfAsteroid();
        uint8_t numOfBullets = (buPool == nullptr) ? 0 :buPool->getNumOfBullets();

        totalSize += sizeof(ServerMsgId); //type
        totalSize += sizeof(uint8_t) + numOfAsteroids * objectInfoSize; //Asteroids (changes)
        totalSize += sizeof(uint8_t) + numOfBullets * objectInfoSize; //Bullets (changes)
        totalSize += 2 * objectInfoSize; //Ship1 + Ship2
        totalSize += sizeof(SoundId); //Sound
        totalSize += sizeof(bool); //won

        alloc_data(totalSize);

        memset(_data, 0, totalSize);

        char* pointer = _data;

        //Write totalSize (needed to reconstruct this msg because the lenght depends on the state of the world)
        memcpy(pointer, &totalSize, sizeof(size_t));
        pointer += sizeof(size_t);

        memcpy(pointer, &type, sizeof(size_t));
        pointer += sizeof(ServerMsgId);

        //Asteroids
        memcpy(pointer, &numOfAsteroids, sizeof(uint8_t));
        pointer += sizeof(uint8_t);  
        if(numOfAsteroids > 0){
            for (Asteroid* a : asPool->getPool()) {
                if(a->getInUse()){
                    to_bin_object(pointer, a->getPos(), a->getWidth(), a->getHeight(), a->getRot());
                    pointer += objectInfoSize;
                }
            }
        }

        //Bullets
        memcpy(pointer, &numOfBullets, sizeof(uint8_t));
        pointer += sizeof(uint8_t);
        if(numOfBullets > 0){
            for (Bullet* b : buPool->getPool()) {
                if(b->getInUse()){
                    to_bin_object(pointer, b->getPos(), b->getWidth(), b->getHeight(), b->getRot());
                    pointer += objectInfoSize;
                }
            }
        }

        //Ship
        if(shipTr1 == nullptr)
            to_bin_object(pointer, Vector2D(), -1, -1, 0);
        else
            to_bin_object(pointer, shipTr1->getPos(), shipTr1->getW(), shipTr1->getH(), shipTr1->getRot());

        pointer += objectInfoSize;

        //Ship2
        if(shipTr2 == nullptr)
            to_bin_object(pointer, Vector2D(), -1, -1, 0);
        else
            to_bin_object(pointer, shipTr2->getPos(), shipTr2->getW(), shipTr2->getH(), shipTr2->getRot());

        pointer += objectInfoSize;

        //Sound
        memcpy(pointer, &sound, sizeof(SoundId));
        pointer += sizeof(SoundId);

        //Won
        memcpy(pointer, &won, sizeof(bool));
    }

    void ServerMsg::to_bin_object(char* pointer, Vector2D pos, int width, int height, double rot) 
    {
        //Pos
        double pos_ = pos.getX();
        memcpy(pointer, &pos_, sizeof(double));
        pointer += sizeof(double);
        pos_ = pos.getY();
        memcpy(pointer, &pos_, sizeof(double));
        pointer += sizeof(double);
        //Size
        memcpy(pointer, &width, sizeof(int));
        pointer += sizeof(int);
        memcpy(pointer, &height, sizeof(int));
        pointer += sizeof(int);
        //Rot
        memcpy(pointer, &rot, sizeof(double));
    }
    
    ServerMsg::ObjectInfo ServerMsg::from_bin_object(char* pointer) 
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
    
    int ServerMsg::from_bin(char * bobj) 
    {
        size_t totalSize;
        //First i read the totalSize of the msg
        memcpy(&totalSize, bobj, sizeof(size_t));

        //Alloc totalSize - sizeof(totalSize) because i dont need to store totalSize variable
        alloc_data(totalSize - sizeof(size_t));

        char* pointer = bobj + sizeof(size_t);

        //Type
        memcpy(&type, pointer, sizeof(size_t));
        pointer += sizeof(ServerMsgId);

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

        //Ships
        ship1 = from_bin_object(pointer);
        pointer += objectInfoSize;

        ship2 = from_bin_object(pointer);
        pointer += objectInfoSize;

        //Sound
        memcpy(&sound, pointer, sizeof(SoundId));
        pointer += sizeof(SoundId);

        //Won
        memcpy(&won, pointer, sizeof(bool));

        return 0;
    }
}

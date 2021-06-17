#include "ServerMsg.h"

namespace ServerMsg
{
    void ServerMsg::to_bin() 
    { 
        size_t totalSize = sizeof(size_t);
        uint8_t numOfAsteroids = asPool->getNumOfAsteroid();
        uint8_t numOfBullets = buPool->getNumOfBullets();

        if(asPool == nullptr) numOfAsteroids = 0;
        if(buPool == nullptr) numOfBullets = 0;

        totalSize += sizeof(uint8_t) + numOfAsteroids * objectInfoSize; //Asteroids
        totalSize += sizeof(uint8_t) + numOfBullets * objectInfoSize; //Bullets
        totalSize += objectInfoSize; //Ship
        totalSize += sizeof(SoundId); //Sound

        alloc_data(totalSize);

        memset(_data, 0, size);

        char* pointer = _data;

        memcpy(pointer, &totalSize, sizeof(size_t));
        pointer += sizeof(size_t);

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
        if(shipTr == nullptr)
            to_bin_object(pointer, Vector2D(), -1, -1, 0);
        else
            to_bin_object(pointer, shipTr->getPos(), shipTr->getWidth(), shipTr->getHeight(), shipTr->getRot());

        pointer += objectInfoSize;

        //Sound
        memcpy(pointer, &sound, sizeof(SoundId));
    }

    void ServerMsg::to_bin_object(char* pointer, Vector2D pos, int width, int height, double rot) 
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
    
    ObjectInfo ServerMsg::from_bin_object(char* pointer) 
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

        //Sound
        memcpy(&sound, pointer, sizeof(SoundId));

        return 0;
    }
}

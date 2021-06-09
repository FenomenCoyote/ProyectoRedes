#pragma once

#include <SDL.h>
#include "Serializable.h"
#include "AsteroidPool.h"
#include "BulletsPool.h"
#include "Health.h"
#include "Transform.h"

namespace ServerMsg
{
    enum ServerMsgId : uint8_t {
        _STARTING_GAME,
        _WORLD_STATE
    };

    class Msg : public Serializable {
        protected:
            Msg(ServerMsgId type) : type(type) {}
            ServerMsgId type;
    };

    class StartingGameMSg : public Msg{
        public:
            StartingGameMSg(): Msg(ServerMsgId::_STARTING_GAME){}

            void to_bin() override;
            int from_bin(char * data) override;

        private:

            const size_t size = sizeof(type);
    };

    constexpr size_t objectInfoSize = 3 * sizeof(double) + 2 * sizeof(int);

    class WorldStateMSg : public Msg{
        public:
            struct ObjectInfo{
                double posX, posY;
                int width, height;
                double rotation;
            };

        public:
            WorldStateMSg(): Msg(ServerMsgId::_WORLD_STATE),
                asPool(nullptr), buPool(nullptr), ship(nullptr), health(nullptr) {}

            WorldStateMSg(AsteroidPool* asPool, BulletsPool* buPool, Transform* ship, Health* health): Msg(ServerMsgId::_WORLD_STATE),
                asPool(asPool), buPool(buPool), ship(ship), health(health) {}

            void to_bin() override;
            int from_bin(char* data) override;
            
        private:
            void to_bin_object(char* pointer, Vector2D pos, int width, int height, double rot);

            AsteroidPool* asPool;
            BulletsPool* buPool;
            Transform* ship;  
            Health* health; 
    };
}

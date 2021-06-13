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
        public:
            ServerMsgId type;
        protected:
            Msg(ServerMsgId type) : type(type) {}
    };

    class StartingGameMSg : public Msg{
        public:
            StartingGameMSg(): Msg(ServerMsgId::_STARTING_GAME){}

            void to_bin() override;
            int from_bin(char * bobj) override;

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
            WorldStateMSg(): WorldStateMSg(nullptr, nullptr, nullptr, nullptr) {}

            WorldStateMSg(AsteroidPool* asPool, BulletsPool* buPool, Transform* ship, Health* health): Msg(ServerMsgId::_WORLD_STATE),
                asteroids(), bullets(), ship(), health(),
                asPool_(asPool), buPool_(buPool), ship_(ship), health_(health) {}

            void to_bin() override;
            int from_bin(char* bobj) override;
            
            std::vector<ObjectInfo> asteroids, bullets;
            ObjectInfo ship;
            uint8_t health;

        private:
            void to_bin_object(char* pointer, Vector2D pos, int width, int height, double rot);
            ObjectInfo from_bin_object(char* pointer);

            AsteroidPool* asPool_;
            BulletsPool* buPool_;
            Transform* ship_;  
            Health* health_; 
    };
}

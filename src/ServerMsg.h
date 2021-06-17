#pragma once

#include <SDL.h>
#include "Serializable.h"
#include "AsteroidPool.h"
#include "BulletsPool.h"
#include "Transform.h"

namespace ServerMsg
{
    enum ServerMsgId : uint8_t {
        _STARTING_GAME,
        _WORLD_STATE,
        _ENDING_GAME
    };

    enum SoundId : uint8_t {
        _NONE,
        _ASTEROID_COLLISION_
    };

    class Msg : public Serializable {
        public:
            ServerMsgId type;
        protected:
            Msg(ServerMsgId type) : type(type) {}
    };

    constexpr size_t objectInfoSize = 3 * sizeof(double) + 2 * sizeof(int);

    class ServerMsg : public Msg{
        public:
            struct ObjectInfo{
                double posX, posY;
                int width, height;
                double rot;
            };

        public:
            ServerMsg(): ServerMsg(nullptr, nullptr, nullptr) {}

            ServerMsg(AsteroidPool* asPool, BulletsPool* buPool, Transform* ship): Msg(ServerMsgId::_WORLD_STATE),
                asteroids(), bullets(), ship(), sound(SoundId::_NONE),
                asPool(asPool), buPool(buPool), shipTr(ship) {}

            void to_bin() override;
            int from_bin(char* bobj) override;

            void setSound(SoundId sound_) { sound = sound_;}
            
            std::vector<ObjectInfo> asteroids, bullets;
            ObjectInfo ship;
            SoundId sound;

        private:
            void to_bin_object(char* pointer, Vector2D pos, int width, int height, double rot);
            ObjectInfo from_bin_object(char* pointer);

            AsteroidPool* asPool;
            BulletsPool* buPool;
            Transform* shipTr;  
    };
}

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
            ServerMsg(): ServerMsg(nullptr, nullptr, nullptr, nullptr) {}

            ServerMsg(AsteroidPool* asPool, BulletsPool* buPool, Transform* ship1, Transform* ship2): Msg(ServerMsgId::_WORLD_STATE),
                asteroids(), bullets(), ship1(), ship2(), sound(SoundId::_NONE), won(false),
                asPool(asPool), buPool(buPool), shipTr1(ship1), shipTr2(ship2) {}

            void to_bin() override;
            int from_bin(char* bobj) override;

            void setSound(SoundId sound_) { sound = sound_;}
            void setWinner(bool w) { won = w;}
            
            std::vector<ObjectInfo> asteroids, bullets;
            ObjectInfo ship1, ship2;
            SoundId sound;
            bool won;

        private:
            void to_bin_object(char* pointer, Vector2D pos, int width, int height, double rot);
            ObjectInfo from_bin_object(char* pointer);

            AsteroidPool* asPool;
            BulletsPool* buPool;
            Transform* shipTr1, *shipTr2;  
    };
}

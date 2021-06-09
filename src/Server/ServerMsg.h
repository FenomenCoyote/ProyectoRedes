#pragma once

#include <SDL.h>

namespace ServerMsg
{
    enum ServerMsgId : uint8_t {
        _STARTING_GAME,
        _WORLD_STATE
    };

    class Msg {
        protected:
            Msg(ServerMsgId type) : type(type) {}
            ServerMsgId type;
    };

    class StartingGameMSg : public Msg{
        public:
            StartingGameMSg(): Msg(ServerMsgId::_STARTING_GAME){}
    };

    class WorldStateMSg : public Msg{
        public:
            WorldStateMSg(): Msg(ServerMsgId::_WORLD_STATE){}
            
    };
}

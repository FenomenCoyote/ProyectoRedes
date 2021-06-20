#pragma once

#include <SDL.h>
#include "Serializable.h"
#include <string.h>
#include <string>
#include <assert.h>

namespace ClientMsg
{
    enum InputId : uint8_t {
        _READY_,
        _AHEAD_,
        _LEFT_,
        _RIGHT_,
        _SHOOT_,
        _LOGOUT_
    };

    //Simple Msg that only carries the instruction
    class InputMsg : public Serializable {
        public:
            InputMsg(InputId input_): input(input_){}

            void to_bin() override;
            int from_bin(char * bobj) override;

            InputId input;
    };

}

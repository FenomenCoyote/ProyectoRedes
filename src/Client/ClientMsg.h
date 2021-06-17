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

    class InputMsg : public Serializable {
        public:
            InputMsg(InputId input = InputId::_READY_): input(input){}

            void to_bin() override;
            int from_bin(char * bobj) override;

            InputId input;
    };

}

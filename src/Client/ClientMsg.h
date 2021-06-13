#pragma once

#include <SDL.h>
#include "Serializable.h"
#include <string.h>
#include <string>
#include <assert.h>

namespace ClientMsg
{
    enum ClientMsgId : uint8_t {
        _LOGIN_,
        _READY_,
        _INPUT_,
        _LOGOUT_
    };

    enum InputId : uint8_t {
        _AHEAD_,
        _LEFT_,
        _RIGHT_,
        _SHOOT_
    };

    class Msg : public Serializable {
        public:
            ClientMsgId type;

        protected:
            Msg(ClientMsgId type) : type(type) {}
    };

    class LoginMsg : public Msg {
        public:
            LoginMsg(const char* nick_): Msg(ClientMsgId::_LOGIN_), nick(nick_){
                assert(nick.length() < 16);
            }

            void to_bin() override;
            int from_bin(char* bobj) override;

        private:

            std::string nick;

            const size_t size = sizeof(ClientMsgId) + 16;
    };

    class ReadyMsg : public Msg {
        public:
            ReadyMsg(): Msg(ClientMsgId::_READY_){}

            void to_bin() override;
            int from_bin(char * bobj) override;

        private:

            const size_t size = sizeof(ClientMsgId);
    };

    class InputMsg : public Msg {
        public:
            InputMsg(InputId input): Msg(ClientMsgId::_INPUT_), input(input){}

            void to_bin() override;
            int from_bin(char * bobj) override;

        private:
            InputId input;
            const size_t size = sizeof(ClientMsgId) + sizeof(InputId);
    };

    class LogoutMsg : public Msg {
        public:
            LogoutMsg(): Msg(ClientMsgId::_LOGOUT_){}

            void to_bin() override;
            int from_bin(char * bobj) override;

        private:

            const size_t size = sizeof(ClientMsgId);
    };

}

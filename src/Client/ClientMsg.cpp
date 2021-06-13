#include "ClientMsg.h"

namespace ClientMsg
{
    void LoginMsg::to_bin() 
    {
        alloc_data(size);

        memset(_data, 0, size);

        char* pointer = _data;

        memcpy(pointer, &type, sizeof(ClientMsgId));
        pointer += sizeof(ClientMsgId);

        memcpy(pointer, nick.c_str(), nick.length());
    }
    
    int LoginMsg::from_bin(char * bobj) 
    {
        alloc_data(size);

        char* pointer = bobj;

        memcpy(&type, pointer, sizeof(ClientMsgId));

        char _nick_[16];
        memcpy(&_nick_, pointer, 16);
        nick = _nick_;

        return 0;
    }
    
    void ReadyMsg::to_bin() 
    {
        alloc_data(size);

        memset(_data, 0, size);

        char* pointer = _data;

        memcpy(pointer, &type, sizeof(ClientMsgId));
    }
    
    int ReadyMsg::from_bin(char * bobj) 
    {
        alloc_data(size);

        char* pointer = bobj;

        memcpy(&type, pointer, sizeof(ClientMsgId));

        return 0;
    }
    
    void InputMsg::to_bin() 
    {
        alloc_data(size);

        memset(_data, 0, size);

        char* pointer = _data;

        memcpy(pointer, &type, sizeof(ClientMsgId));
        pointer += sizeof(ClientMsgId);

        memcpy(pointer, &input, sizeof(InputId));
    }
    
    int InputMsg::from_bin(char * bobj) 
    {
        alloc_data(size);

        char* pointer = bobj;

        memcpy(&type, pointer, sizeof(ClientMsgId));
        pointer += sizeof(ClientMsgId);

        memcpy(&input, pointer, sizeof(InputId));

        return 0;
    }

    void LogoutMsg::to_bin() 
    {
        alloc_data(size);

        memset(_data, 0, size);

        char* pointer = _data;

        memcpy(pointer, &type, sizeof(ClientMsgId));
    }


    int LogoutMsg::from_bin(char * bobj) 
    {
        alloc_data(size);

        char* pointer = bobj;

        memcpy(&type, pointer, sizeof(ClientMsgId));

        return 0;
    }
}

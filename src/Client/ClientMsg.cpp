#include "ClientMsg.h"

namespace ClientMsg
{
    void InputMsg::to_bin() 
    {
        alloc_data(sizeof(InputId));

        memset(_data, 0, sizeof(InputId));

        char* pointer = _data;

        memcpy(pointer, &input, sizeof(InputId));
    }
    
    int InputMsg::from_bin(char * bobj) 
    {
        alloc_data(sizeof(InputId));

        char* pointer = bobj;

        memcpy(&input, pointer, sizeof(InputId));

        return 0;
    }  
}

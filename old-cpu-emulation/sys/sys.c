#include "sys.h"

i16 find_str_in_list(cstr str, cstr* const list, size_t const listLength) {
    for( size_t i = 0; i < listLength; i++ ) {
        if(strcmp(str, list[i]) == 0) return i;
    }
    return -1;
}

u16 str_to_int(cstr str) {
    u16 ret = 0;
    u16 fac = 1;

    for( size_t i = 0; str[i] != 0; i++ ) 
    {
        ASSERT( '0' <= str[i] && str[i] <= '9' );
        u16 x = str[i] - '0';
        ret += x * fac;
        fac *= 10;
    }

    return ret;
}

u16 hexStrToInt(cstr hexStr) {
    u16 ret = 0;
    u16 fac = 1;

    for( size_t i = 0; hexStr[i] != 0; i++ ) 
    {
        ASSERT( '0' <= hexStr[i] && hexStr[i] <= '9' || 'a' <= hexStr[i] && hexStr[i] <= 'f' );
        u16 x = hexStr[i] - '0';
        if(x > '9') {
            x = hexStr[i] - 'a';
        }
        ret += x * fac;
        fac *= 16;
    }
    return ret;    
}

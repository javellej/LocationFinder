#include <stdio.h>
#include "error.h"

int errCodeToMessage( int i_errCode, char *o_message)
{
    switch ( i_errCode )
    {
        case ERROR_CURL_INITIALIZE:
            sprintf( o_message, "error while initializing curl");
            break;
        case ERROR_CURL_ERROR:
            sprintf( o_message, "curl error");
            break;
        case ERROR_WRONG_FORMAT:
            sprintf( o_message, "wrong format");
            break;
        case ERROR_UNEXPECTED_VALUE:
            sprintf( o_message, "unexpected value");
            break;
        case ERROR_INVALID_ARGUMENT:
            sprintf( o_message, "invalid argument");
            break;
    }
    return 0;
}

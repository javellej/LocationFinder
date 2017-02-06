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
        case ERROR_FILE_OPEN:
            sprintf( o_message, "can't open file");
            break;
        case ERROR_FILE_FORMAT:
            sprintf( o_message, "wrong file format");
            break;
        case ERROR_IMAGE_WIDTH:
            sprintf( o_message, "bad image width");
            break;
        case ERROR_IMAGE_HEIGHT:
            sprintf( o_message, "bad image height");
            break;
        case ERROR_TYPE_SIZE:
            sprintf( o_message, "incompatible type size");
            break;
        case ERROR_COLOR_TYPE:
            sprintf( o_message, "only supported color type is RGBA");
            break;
        case ERROR_BIT_DEPTH:
            sprintf( o_message, "only supported bit depth is 8");
            break;
        case ERROR_DIMENSIONS:
            sprintf( o_message, "wrong dimensions");
            break;
    }
    return 0;
}

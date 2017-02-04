#ifndef _ERROR_H_
#define _ERROR_H

#define CHECK( EXP) do { retCode = (EXP); if ( retCode ) { goto ERROR; } } while ( 0 )

#define ERROR_CURL_INITIALIZE   1
#define ERROR_CURL_ERROR        2
#define ERROR_WRONG_FORMAT      3
#define ERROR_UNEXPECTED_VALUE  4
#define ERROR_INVALID_ARGUMENT  5

int errCodeToMessage( int i_errCode, char *o_message);

#endif

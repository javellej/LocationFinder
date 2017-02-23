#ifndef _CONTEXT_H_
#define _CONTEXT_H_

#include <curl/curl.h>

typedef struct { // TODO : add conversion coefficients
    CURL *curl;
}t_context;

int context_init( t_context *io_context);
int context_term( t_context *io_context);

#endif // _CONTEXT_H_

#include "context.h"
#include "error.h"
#include <curl/curl.h>

int context_init( t_context *io_context) {
    int retCode;

    // initialize curl
    io_context->curl = curl_easy_init();
    if ( NULL == io_context->curl ) { CHECK( ERROR_CURL_INITIALIZE); }

    return 0;

ERROR:
    return retCode;
}

int context_term( t_context *io_context) {
    // cleanup curl
    curl_easy_cleanup( io_context->curl);
    io_context->curl = NULL;

    return 0;
}

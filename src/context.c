#include "context.h"
#include "structs.h"
#include "error.h"
#include <curl/curl.h>

int context_init( t_context *io_context, t_point i_center, int i_zoom_level) {
    int retCode;

    // initialize curl
    io_context->curl = curl_easy_init();
    if ( NULL == io_context->curl ) { CHECK( ERROR_CURL_INITIALIZE); }

    io_context->center = i_center;
    io_context->zoom = i_zoom_level;

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

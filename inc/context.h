#ifndef _CONTEXT_H_
#define _CONTEXT_H_

#include "structs.h"
#include <curl/curl.h>

typedef struct { // TODO : add conversion coefficients
    CURL *curl; // CURL handler for data retrieval
    t_point center; // center of the map image
    int zoom; // zoom level of the map
}t_context;

int context_init( t_context *io_context, t_point i_center, int i_zoom_level);
int context_term( t_context *io_context);

#endif // _CONTEXT_H_

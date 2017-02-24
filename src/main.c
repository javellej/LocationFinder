#include <stdio.h>
#include <string.h>
#include <time.h>

#include "image.h"
#include "overlay.h"
#include "structs.h"
#include "tools.h"
#include "coordinates.h"
#include "error.h"
#include "definitions.h"
#include "context.h"

int printUsage( )
{
    printf( "Usage : ./locationFinder -lat <latitude> -lon <longitude>\n");

    return 0;
}

int retrieveArguments( int i_argc, char **i_argv, float *o_lng, float *o_lat)
{
    float lng=0, lat=0;

    while ( i_argc )
    {
        if ( (i_argc >= 2) && !strcmp( *i_argv, "-lon") )
        {
            lng = atof( *(i_argv+1));
            i_argc-=2;
            i_argv+=2;
        } else if ( (i_argc >= 2) && !strcmp( *i_argv, "-lat") ) {
            lat = atof( *(i_argv+1));
            i_argc-=2;
            i_argv+=2;
        } else {
            printUsage( );
            return ERROR_INVALID_ARGUMENT;
        }
    }

    *o_lng = lng;
    *o_lat = lat;

    return 0;
}

int sampleImage( t_rgb_image *io_image) {
    t_rgb_image image;
    int i, j;

    image.width = IMAGE_WIDTH;
    image.height = IMAGE_HEIGHT;
    image.pixels = (t_pixel **) malloc( image.height * sizeof( t_pixel *));
    for ( i=0; i<image.height; i++ ) {
        image.pixels[i] = (t_pixel *) malloc( image.width * sizeof( t_pixel));
    }

    // fill image contents
    for ( i=0; i<image.height; i++ ) {
        for ( j=0; j<image.width; j++ ) {
            t_pixel pixel;
            pixel.R = 0xff;
            pixel.G = 0;
            pixel.B = 0;
            image.pixels[i][j] = pixel;
        }
    }

    *io_image = image;

    return 0;
}

int main( int argc, char **argv) {
    int retCode;

    t_context context;
    t_point center;
    char errorMessage[512] = {0};
    t_overlay overlay;

    // get command arguments
    CHECK( retrieveArguments( argc-1, argv+1, &(center.lng), &(center.lat)));
    printf( "lon %f, lat %f\n", center.lng, center.lat);

    // initialize context
    CHECK( context_init( &context, center, 14));

    // get map centered on input point
    CHECK( getMap( &context));

    // initialize overlay
    overlay_init( &overlay);

    // create grid queries
    t_point *candidates = NULL;
    size_t num_candidates;
    CHECK( candidates_grid( &context, 20, &candidates, &num_candidates));

    // create image and add overlay to it
    t_rgb_image image;
    CHECK( pngToRgb( "simple_map.png", &image));
    CHECK( addOverlay( image, overlay));
    CHECK( rgbToPng( image, "map_overlay.png"));

    // TODO : free memory -> init + term functions
    CHECK( overlay_release( &overlay));
    CHECK( context_term( &context));

    return 0;

ERROR:
    errCodeToMessage( retCode, errorMessage);
    printf( "Error detected -> code %d : %s\n", retCode, errorMessage);
    return retCode;
}

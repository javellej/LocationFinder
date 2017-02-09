#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <time.h>

#include "image.h"
#include "overlay.h"
#include "structs.h"
#include "tools.h"
#include "coordinates.h"
#include "error.h"
#include "definitions.h"

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

    CURL *curl;
    t_point start;//, end;
    //int maxDistance = 50000;
    //int maxTime = 45*60;
    //int maxRadius = maxDistance*(0xffffffff/(float)EARTH_CIRCUMFERENCE);
    int i;
    char errorMessage[512] = {0};
    t_overlay overlay;

    // get command arguments
    CHECK( retrieveArguments( argc-1, argv+1, &(start.lng), &(start.lat)));
    printf( "lon %f, lat %f\n", start.lng, start.lat);

    // test mercator conversion
    int x, y;
    //CHECK( conv_spherical_to_mercator( 180, 85.05115, 14, &x, &y));
    CHECK( conv_spherical_to_image( start, 14, start.lng + 0.01, start.lat + 0.01, &x, &y));
    printf( "x = %d, y=%d\n", x, y);

    // initialize curl
    curl = curl_easy_init();
    if ( curl == NULL ) { CHECK( ERROR_CURL_INITIALIZE); }

    // get map centered on input point
    CHECK( getMap( curl, start));

    // cleanup curl
    curl_easy_cleanup( curl);

    // test overlay
    t_rgb_image image;
    CHECK( pngToRgb( "simple_map.png", &image));
    overlay.width = IMAGE_WIDTH;
    overlay.height = IMAGE_HEIGHT;
    overlay.overlay = (unsigned char **) malloc( overlay.height * sizeof( unsigned char *));
    for ( i=0; i<overlay.height; i++ ) {
        overlay.overlay[i] = (unsigned char *) malloc( overlay.width * sizeof( unsigned char));
        memset( overlay.overlay[i], 0, overlay.width * sizeof( unsigned char));
    }
    /*addSquare( overlay, 12, 4, 67);
    addCircle( overlay, 333, 172, 18);
    addSquare( overlay, 402, 444, 111);*/
    addCircle( overlay, x, y, 10);
    CHECK( addOverlay( image, overlay));
    CHECK( rgbToPng( image, "map_overlay.png"));

    // TODO : free memory -> init + term functions

    return 0;

ERROR:
    errCodeToMessage( retCode, errorMessage);
    printf( "Error detected -> code %d : %s\n", retCode, errorMessage);
    return retCode;
}

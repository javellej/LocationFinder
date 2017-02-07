#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <time.h>

#include "image.h"
#include "overlay.h"
#include "structs.h"
#include "tools.h"
#include "error.h"
#include "definitions.h"

int printUsage( )
{
    printf( "Usage : ./locationFinder -lat <latitude> -lon <longitude>\n");

    return 0;
}

int retrieveArguments( int i_argc, char **i_argv, int *o_longitude, int *o_latitude)
{
    int longitude=0, latitude=0;

#if 0
    printf( "argc : %d\n", i_argc);
    printf( "*argv : %s\n", *i_argv);
#endif

    while ( i_argc )
    {
        if ( (i_argc >= 2) && !strcmp( *i_argv, "-lon") )
        {
            longitude = atoi( *(i_argv+1));
            i_argc-=2;
            i_argv+=2;
        } else if ( (i_argc >= 2) && !strcmp( *i_argv, "-lat") ) {
            latitude = atoi( *(i_argv+1));
            i_argc-=2;
            i_argv+=2;
        } else {
            printUsage( );
            return ERROR_INVALID_ARGUMENT;
        }
    }

    *o_longitude = longitude;
    *o_latitude = latitude;

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
    int distance_m; /* distance in m */
    int time_s; /* time in s */
    t_point start, end;
    int maxDistance = 50000;
    int maxTime = 45*60;
    int maxRadius = maxDistance*(0xffffffff/(float)EARTH_CIRCUMFERENCE);
    int i;
    char errorMessage[512] = {0};
    t_overlay overlay;

    // get command arguments
    CHECK( retrieveArguments( argc-1, argv+1, &(start.m_longitude), &(start.m_latitude)));
    printf( "lon %d, lat %d\n", start.m_longitude, start.m_latitude);

    // initialize curl
    curl = curl_easy_init();
    if ( curl == NULL ) { CHECK( ERROR_CURL_INITIALIZE); }

    // get map centered on input point
    CHECK( getMap( curl, start));

    // get list of distances 

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
    addSquare( overlay, 12, 4, 67);
    addCircle( overlay, 333, 172, 18);
    addSquare( overlay, 402, 444, 111);
    CHECK( addOverlay( image, overlay));
    CHECK( rgbToPng( image, "simple_map_2.png"));

    // TODO : free memory -> init + term functions

    return 0;

ERROR:
    errCodeToMessage( retCode, errorMessage);
    printf( "Error detected -> code %d : %s\n", retCode, errorMessage);
    return retCode;
}

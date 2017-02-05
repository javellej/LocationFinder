#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <time.h>

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

int main( int argc, char **argv) {
    int retCode;

    CURL *curl;
    int distance_m; /* distance in m */
    int time_s; /* time in s */
    t_point start, end;
    int maxDistance = 50000;
    int maxTime = 45*60;
    int maxRadius = maxDistance*(0xffffffff/(float)EARTH_CIRCUMFERENCE);
    t_point candidates[NUMPOINTS] = {{0}};
    t_point *convexHullCandidates = NULL;
    int numVertices = 0;
    int numCandidates = 0;
    int i;

    // get command arguments
    CHECK( retrieveArguments( argc-1, argv+1, &(start.m_longitude), &(start.m_latitude)));
    printf( "lon %d, lat %d\n", start.m_longitude, start.m_latitude);

    // initialize curl
    curl = curl_easy_init();
    if ( curl == NULL ) { CHECK( ERROR_CURL_INITIALIZE); }

    // get map centered on input point
    CHECK( getMap( curl, start));

    /*
    // find several candidates for good locations
    //srand( time( NULL));
    for ( i=0; i<NUMPOINTS; i++ ) {
        int r_lat = ( rand() % (2 * maxRadius) ) - maxRadius;
        int r_lon = ( rand() % (2 * maxRadius) ) - maxRadius;
        end.m_latitude = start.m_latitude + r_lat;
        end.m_longitude = start.m_longitude + r_lon;
        CHECK( getDistance( curl, start, end, &distance_m, &time_s));
#if 0
        printf( "step %d : distance %d m, time %d s\n", i, distance_m, time_s);
#endif
//        if ( distance_m < maxDistance )
        if ( time_s < maxTime ) {
            candidates[numCandidates++] = end;
        }
    }
    printf( "num candidates : %d\n", numCandidates);

    // compute convex hull
    //CHECK( convexHull( candidates, numCandidates, &convexHullCandidates, &numVertices));

    // get corresponding map
    //CHECK( getAreaMap( curl, start, convexHullCandidates, numVertices));
    */

    /* cleanup curl */ 
    curl_easy_cleanup( curl);
    return 0;

ERROR:
    printf( "Error detected -> code %d\n", retCode);
    return retCode;
}

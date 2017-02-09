#include "tools.h"
#include "query.h"
#include "error.h"
#include "definitions.h"
#include <curl/curl.h>
#include <string.h>

/* callback used by curl to write data from a webpage into a structure */
size_t write_callback( char *i_ptr, size_t i_size, size_t i_nmemb, t_buffer *io_dstBuffer) {
    int size = i_size*i_nmemb;
    t_buffer dstBuffer = *io_dstBuffer;

#if 0
    printf( "DEBUG : \"write_callback\" function called\n");
#endif

    /* reallocate enough space in initial buffer */
    dstBuffer.m_data = realloc( dstBuffer.m_data, dstBuffer.m_size+size+1);

    /* copy data */
    memcpy( &(dstBuffer.m_data[dstBuffer.m_size]), i_ptr, size);
    dstBuffer.m_size += size;
    dstBuffer.m_data[dstBuffer.m_size] = 0;

    /* update output value */
    *io_dstBuffer = dstBuffer;

    return size;
}

/* PRE : "curl_easy_init" has been called */
int getDistance( CURL *i_curl, t_point i_start, t_point i_end, int *o_distance, int *o_time) {
    int retCode;

    int curlRetCode;
    char queryUrl[DIRECTIONQUERY_LENGTH+1];
    t_buffer pageContent;
    char *ptr;
    //int *foo;
    int distance, time;

    /* create query url to google maps */
    queryUrl[DIRECTIONQUERY_LENGTH] = 0;
    directionQuery( i_start, i_end, queryUrl);

    /* get contents */
#if 1
    printf( "query url = %s\n", queryUrl);
#endif
    initBuffer( &pageContent, 1);
    pageContent.m_size = 0;
    curl_easy_setopt( i_curl, CURLOPT_URL, queryUrl);
    curl_easy_setopt( i_curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt( i_curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt( i_curl, CURLOPT_WRITEDATA, &pageContent);
    curlRetCode = curl_easy_perform( i_curl);
    if ( curlRetCode ) { CHECK( ERROR_CURL_ERROR); }
#if 0
    printf( "page content : %s\n", pageContent.m_data);
#endif

    /* retrieve distance and time values */
    /*  locate distance unit */
    ptr = strstr( pageContent.m_data, " km\"");
    if ( ptr == NULL ) { ptr = strstr( pageContent.m_data, " m\""); }
    if ( ptr == NULL ) { ptr = strstr( pageContent.m_data, " miles\""); }
    if ( ptr == NULL ) { CHECK( ERROR_WRONG_FORMAT); }
    /*  place pointer at the beginning of the interesting data */
    while ( *(--ptr) != '[' ) {
        if ( ptr < pageContent.m_data ) { CHECK( ERROR_WRONG_FORMAT); }
    }
#if 0
    ptr[100] = 0;
    printf( "ptr : %s\n", ptr);
#endif
    /*  scan disance value */
    sscanf( ptr, "[%d,", &distance);
    /*  scan time value */
    ptr = strstr( ptr+1, "[");
    sscanf( ptr, "[%d,", &time);

    /* free memory and return value */
    freeBuffer( &pageContent);
    *o_distance = distance;
    *o_time = time;

    return 0;

ERROR:
    return retCode;
}

/*
 * retrieve map centered on the given point
 */
int getMap( CURL *i_curl, t_point i_center) {
    int retCode;
    FILE *f = fopen( "simple_map.png", "w");
    char query_url[512];

    memset( query_url, 0, 512*sizeof( char));

    // get query string
    pointQuery( i_center, query_url);
    printf( "map query : %s\n", query_url);

    // get map centered on input point
    CHECK( curl_easy_setopt( i_curl, CURLOPT_URL, query_url));
    CHECK( curl_easy_setopt( i_curl, CURLOPT_FOLLOWLOCATION, 1L));
    CHECK( curl_easy_setopt( i_curl, CURLOPT_WRITEFUNCTION, NULL));
    CHECK( curl_easy_setopt( i_curl, CURLOPT_WRITEDATA, f));
    CHECK( curl_easy_perform( i_curl));

    fclose( f);

    return 0;

ERROR:
    return retCode;
}

/*
 * retrieve map centered on the requested point with a highlighted polygonal area
 */
int getAreaMap( CURL *i_curl, t_point i_center, t_point *i_polygon, int i_numVertices) {
    int retCode;
    FILE *f = fopen( "map.png", "w");
    char *queryUrl = (char *) malloc( 205+26*(i_numVertices+1));

    memset( queryUrl, 0, 205+26*(i_numVertices+1));

    /* get query string */
    polygonQuery( i_center, i_polygon, i_numVertices, queryUrl);
    printf( "url polygon: %s\n", queryUrl);

    /* get image */
    CHECK( curl_easy_setopt( i_curl, CURLOPT_URL, queryUrl));
    CHECK( curl_easy_setopt( i_curl, CURLOPT_FOLLOWLOCATION, 1L));
    CHECK( curl_easy_setopt( i_curl, CURLOPT_WRITEFUNCTION, NULL));
    CHECK( curl_easy_setopt( i_curl, CURLOPT_WRITEDATA, f));
    CHECK( curl_easy_perform( i_curl));

    /* free memory */
    free( queryUrl);
    queryUrl = NULL;

    fclose( f);

    return 0;

ERROR:
    return retCode;
}

/* evaluation function for sort : monotonic according to angles */
int evalPoint( t_point i_pointToEval, t_point i_refPoint, float *o_value) {
    float value;
    float adj = (float) i_pointToEval.lng - i_refPoint.lng;
    float opp = (float) i_pointToEval.lat - i_refPoint.lat;
    float hyp2 = adj*adj + opp*opp;

    if ( !hyp2 ) /* same point */ {
        value = 0;
    } else {
        value = adj*adj / hyp2;
        if ( adj > 0 ) { value = -value; }
    }
#if 0
    printf( "value : adj %f, opp %f, value %f\n", adj, opp, value);
#endif

    *o_value = value;

    return 0;
}

/* in place quicksort of an array of points according to angle with  */
int sortAngle( t_point *i_points, int i_numPoints, t_point i_refPoint) {
    int i;
    int pivotFinalIndex = 0;
    float pivotValue, currValue;

    /* terminal case */
    if ( (i_numPoints == 1) || (i_numPoints == 0) ) { return 0; }

    /* place elements according to pivot */
    evalPoint( i_points[0], i_refPoint, &pivotValue);
    for ( i=1; i<i_numPoints; i++ ) {
        evalPoint( i_points[i], i_refPoint, &currValue);
        if ( currValue < pivotValue ) {
            swapPoints( i_points, i, ++pivotFinalIndex);
        }
    }

    /* put pivot at right place */
    swapPoints( i_points, 0, pivotFinalIndex);

    /* recursive call */
    sortAngle( i_points, pivotFinalIndex, i_refPoint);
    sortAngle( i_points+pivotFinalIndex+1, i_numPoints-pivotFinalIndex-1, i_refPoint);

    return 0;
}

/* Graham scan
 * IN : array of "t_points"
 * OUT : arry of "t_point" that form the convex hull polygon */
int convexHull( t_point *i_points, int i_numPoints, t_point **o_convexHull, int *o_numVertices) {
    int retCode;
    int lowLatIndex = 0;
    char *hullPoints;
    int prev;
    size_t hullSize = 2;
    int hullIndex = 0;
    t_point *convexHull = NULL;
    int i;

    if ( (i_points == NULL) ||
            (o_convexHull == NULL) ||
            ((*o_convexHull) != NULL) ||
            (o_numVertices == NULL) ||
            (i_numPoints < 3) )
    {
        CHECK( ERROR_UNEXPECTED_VALUE);
    }

    /* step 1 : find element that is on convex hull -> lower latitude */
    for ( i=0; i<i_numPoints; i++ ) {
        if ( i_points[i].lat < i_points[lowLatIndex].lat ) {
            lowLatIndex = i;
        }
    }
    swapPoints( i_points, 0, lowLatIndex);

    /* step 2 : sort other points according to angle with first point */
    CHECK( sortAngle( i_points+1, i_numPoints-1, i_points[0]));
    
    /* step 3 : determine which points to keep (left turns) */
    hullPoints = (char *) malloc( i_numPoints);
    memset( hullPoints, 0, i_numPoints);
    hullPoints[0] = 1;
    hullPoints[i_numPoints-1] = 1;
    prev = 0;
    for ( i=1; i<i_numPoints-1; i++ ) {
        long long xA = (long long) i_points[prev].lng;
        long long yA = (long long) i_points[prev].lat;
        long long xB = (long long) i_points[i+1].lng;
        long long yB = (long long) i_points[i+1].lat;
        long long xCurr = (long long) i_points[i].lng;
        long long yCurr = (long long) i_points[i].lat;
        long long eval = (xA-xCurr)*(yB-yCurr) - (xB-xCurr)*(yA-yCurr);
        printf( "eval : %lld\n", eval);
        if ( eval < 0 ) {/* add point to convex hull */
            hullPoints[i] = 1;
            hullSize++;
            prev = i;
        }
    }
    /*  build convex hull array */
    convexHull = (t_point *) malloc( hullSize*sizeof( t_point));
    for ( i=0; i<i_numPoints; i++ ) {
        if ( hullPoints[i] ) {
            convexHull[hullIndex++] = i_points[i];
        }
    }

    /* free local memory */
    free( hullPoints);
    hullPoints = NULL;

    /* output values */
    *o_convexHull = convexHull;
    *o_numVertices = hullSize;

    return 0;

ERROR:
    return retCode;
}

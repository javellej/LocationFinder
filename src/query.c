#include "tools.h"
#include "query.h"
#include "definitions.h"
#include <string.h>
#include <stdio.h>

int coordToStr( int i_coord, char *io_string)
{
    if ( i_coord < 0 )
    {
        *(io_string++) = '-';
        i_coord = -i_coord;
    }

    sprintf( io_string, "%03d.%.7d", i_coord/10000000, i_coord%10000000);

    return 0;
}

/*
 * output query URL for single point
 * PRE : the query string "o_query" is already allocated
 */
int pointQuery( t_point i_center, char *o_query) {
   char latitude_str[16]; 
   char longitude_str[16]; 

    coordToStr( i_center.m_latitude, latitude_str);
    coordToStr( i_center.m_longitude, longitude_str);
    sprintf( o_query, "http://www.google.com/maps/api/staticmap?center=%s,%s&size=%dx%d&zoom=10", latitude_str, longitude_str, IMAGE_WIDTH, IMAGE_HEIGHT);

    return 0;
}

/* 
 * output query string to google maps
 * PRE : the query string "o_query" is already allocated
 */
int directionQuery( t_point i_start, t_point i_end, char *o_query)
{
    char startLatitude[16];
    char startLongitude[16];
    char endLongitude[16];
    char endLatitude[16];

    coordToStr( i_start.m_latitude, startLatitude);
    coordToStr( i_start.m_longitude, startLongitude);
    coordToStr( i_end.m_latitude, endLatitude);
    coordToStr( i_end.m_longitude, endLongitude);
    sprintf( o_query, "http://www.google.com/maps/dir/%s,%s/%s,%s/", startLatitude, startLongitude, endLatitude, endLongitude);
    //sprintf( o_query, "http://www.google.com/maps/dir/%03d.%.7d,%03d.%.7d/%03d.%.7d,%03d.%.7d/", i_start.m_latitude/10000000, i_start.m_latitude%10000000, i_start.m_longitude/10000000, i_start.m_longitude%10000000, i_end.m_latitude/10000000, i_end.m_latitude%10000000, i_end.m_longitude/10000000, i_end.m_longitude%10000000);

    return 0;
}

/* output query string to google maps for polygon area drawing 
 * PRE : the query string "o_query" is already allocated and set to '0'
 */
int polygonQuery( t_point i_center, t_point *i_polygon, int i_numPoints, char *o_query)
{
    int i;
    char *tmp = o_query;
    char pointLatitude[16];
    char pointLongitude[16];

    coordToStr( i_center.m_latitude, pointLatitude);
    coordToStr( i_center.m_longitude, pointLongitude);

    /* write beginning of query */
    sprintf( o_query, "http://maps.googleapis.com/maps/api/staticmap?center=%s,%s&zoom=10&size=%dx%d&maptype=plan&markers=color:red%%7C%s,%s&path=color%%3ablack|weight:3|fillcolor%%3agreen", pointLatitude, pointLongitude, IMAGE_WIDTH, IMAGE_HEIGHT, pointLatitude, pointLongitude);
    tmp += strlen( o_query);

    /* write points */
    for ( i=0; i<i_numPoints; i++ )
    {
        coordToStr( i_polygon[i].m_latitude, pointLatitude);
        coordToStr( i_polygon[i].m_longitude, pointLongitude);
        sprintf( tmp, "|%s,%s", pointLatitude, pointLongitude);
        tmp = o_query + strlen( o_query);
    }
    /*  add first point to close polygon */
    coordToStr( i_polygon[0].m_latitude, pointLatitude);
    coordToStr( i_polygon[0].m_longitude, pointLongitude);
    sprintf( tmp, "|%s,%s", pointLatitude, pointLongitude);

    return 0;
}

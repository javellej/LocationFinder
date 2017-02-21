#include "tools.h"
#include "query.h"
#include "definitions.h"
#include <string.h>
#include <stdio.h>

/*
 * output query URL for single point
 * PRE : the query string "o_query" is already allocated
 */
int pointQuery( t_point i_center, char *o_query) {
    sprintf( o_query, "http://www.google.com/maps/api/staticmap?center=%f,%f&size=%dx%d&zoom=14", i_center.lat, i_center.lng, IMAGE_WIDTH, IMAGE_HEIGHT);

    return 0;
}

/* 
 * output query string to google maps
 * PRE : the query string "o_query" is already allocated
 */
int directionQuery( t_point i_start, t_point i_end, char *o_query)
{
    sprintf( o_query, "http://www.google.com/maps/dir/%f,%f/%f,%f/", i_start.lat, i_start.lng, i_end.lat, i_end.lng);

    return 0;
}

/* output query string to google maps for polygon area drawing 
 * PRE : the query string "o_query" is already allocated and set to '0'
 */
int polygonQuery( t_point i_center, t_point *i_polygon, int i_numPoints, char *o_query)
{
    int i;
    char *tmp = o_query;

    /* write beginning of query */
    sprintf( o_query, "http://maps.googleapis.com/maps/api/staticmap?center=%f,%f&zoom=10&size=%dx%d&maptype=plan&markers=color:red%%7C%f,%f&path=color%%3ablack|weight:3|fillcolor%%3agreen", i_center.lat, i_center.lng, IMAGE_WIDTH, IMAGE_HEIGHT, i_center.lat, i_center.lng);
    tmp += strlen( o_query);

    /* write points */
    for ( i=0; i<i_numPoints; i++ )
    {
        sprintf( tmp, "|%f,%f", i_polygon[i].lat, i_polygon[i].lng);
        tmp = o_query + strlen( o_query);
    }
    /*  add first point to close polygon */
    sprintf( tmp, "|%f,%f", i_polygon[0].lat, i_polygon[0].lng);

    return 0;
}

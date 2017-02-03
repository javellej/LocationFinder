#ifndef _TOOLS_H_
#define _TOOLS_H_

#include <stdlib.h>
#include <curl/curl.h>

#include "structs.h"

#define DEBUG 1

#define IMAGE_WIDTH             600
#define IMAGE_HEIGHT            600
#define DIRECTIONQUERY_LENGTH   69 
#define EARTH_CIRCUMFERENCE     40075160
#define NUMPOINTS               100

static inline int initBuffer( t_buffer *i_buffer, size_t i_size)
{
    i_buffer->m_data = (char *) malloc( i_size);
    i_buffer->m_size = i_size;
    return 0;
}

static inline int freeBuffer( t_buffer *io_buffer)
{
    io_buffer->m_size = 0;
    free( io_buffer->m_data);
    return 0;
}

static inline int swapPoints( t_point *i_points, int i_posA, int i_posB)
{
    t_point tmpPoint = i_points[i_posA];
    i_points[i_posA] = i_points[i_posB];
    i_points[i_posB] = tmpPoint;
    return 0;
}

size_t write_callback( char *i_ptr, size_t i_size, size_t i_nmemb, t_buffer *o_dstBuffer);
int getDistance( CURL *i_curl, t_point i_start, t_point i_end, int *o_distance, int *o_time);
int getAreaMap( CURL *i_curl, t_point i_center, t_point *i_polygon, int i_numVertices);
int evalPoint( t_point i_pointToEval, t_point i_refPoint, float *o_value);
int sortAngle( t_point *i_points, int i_numPoints, t_point i_refPoint);
int convexHull( t_point *i_points, int i_numPoints, t_point **o_convexHull, int *o_numVertices);

#endif
#ifndef _QUERY_H_
#define _QUERY_H_

#include "structs.h"

int pointQuery( t_point i_center, char *o_query);
int directionQuery( t_point i_start, t_point i_end, char *o_query);
int polygonQuery( t_point i_center, t_point *i_polygon, int i_numPoints, char *o_query);

#endif

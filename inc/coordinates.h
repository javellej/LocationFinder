#ifndef _COORDINATES_H_
#define _COORDINATES_H_

#include "structs.h"
#include "context.h"

int conv_spherical_to_mercator( float i_lng, float i_lat, int i_zoom, int *o_x, int *o_y);
int conv_mercator_to_spherical( int i_x, int i_y, int i_zoom, float *o_lng, float *o_lat);
int conv_spherical_to_image( t_context *i_context, float i_lng, float i_lat, int *o_x, int*o_y);
int conv_image_to_spherical( t_context *i_context, int i_x, int i_y, float *o_lng, float *o_lat);

#endif /* _COORDINATES_H_ */

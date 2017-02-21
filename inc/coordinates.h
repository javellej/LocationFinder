#ifndef _COORDINATES_H_
#define _COORDINATES_H_

#include "structs.h"

int conv_spherical_to_mercator( float i_lng, float i_lat, int i_zoom, int *o_x, int *o_y);
int conv_spherical_to_image( t_point i_center, int i_zoom, float i_lng, float i_lat, int *o_x, int*o_y);

#endif /* _COORDINATES_H_ */

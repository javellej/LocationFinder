#include "coordinates.h"
#include "error.h"
#include "definitions.h"
#include <math.h>
#include <stdio.h>

/*
 * convert longitude/latitude to coordinates
 * on the Mercator projection map
 */
int conv_spherical_to_mercator( float i_lng, float i_lat, int i_zoom, int *o_x, int *o_y) {
    int retCode;
    float lng_rd, lat_rd;
    float mer_x, mer_y;

    if ( ( i_lng > MAX_LONGITUDE ) ||
         ( i_lng <= - MAX_LONGITUDE ) ||
         ( i_lat > MAX_LATITUDE ) ||
         ( i_lat <= - MAX_LATITUDE ) ) {
        CHECK( ERROR_OUT_OF_RANGE);
    }
    
    // convert from degree to radian
    lng_rd = i_lng * PI / 180;
    lat_rd = i_lat * PI / 180;

    // compute Mercator coordinates ( source : Wikipedia Mercator projection )
    mer_x = lng_rd; // reference longitude 0 is Greenwich meridian
    mer_y = log( tan( PI / 4 + lat_rd / 2));

    // convert to global map coordinates in pixels depending on zoom level
    // conversion is linear : PI -> 2^(z+7), map is a square
    float rad_to_px = pow( 2, i_zoom + 7) / PI;
    *o_x = (int) ( mer_x * rad_to_px );
    *o_y = (int) ( mer_y * rad_to_px );

    return 0;

ERROR:
    return retCode;
}

/*
 * convert Mercator coordinates to longitude/latitude
 */
int conv_mercator_to_spherical( int i_x, int i_y, int i_zoom, float *o_lng, float *o_lat) {
    float lng_rd, lat_rd;
    float mer_x, mer_y;

    // convert from pixels to radian
    float px_to_rad = PI * pow( 2, - i_zoom - 7); // TODO compute only once
    mer_x = i_x * px_to_rad;
    mer_y = i_y * px_to_rad;
    
    // inverse Mercator formula
    lng_rd = mer_x;
    lat_rd = 2 * atan( exp( mer_y)) - PI / 2;

    // convert from radian to degree
    float rad_to_deg = 180 / PI; // TODO compute only once
    *o_lng = lng_rd * rad_to_deg;
    *o_lat = lat_rd * rad_to_deg;

    return 0;
}

/*
 * convert spherical coordinates to image coordinates where the center is specified
 */
int conv_spherical_to_image( t_context *i_context, float i_lng, float i_lat, int *o_x, int*o_y) {
    // TODO : check query point is inside image (after mercator conversion)
    int retCode;
    int x_map, y_map, x_center, y_center;
    t_point center = i_context->center;
    int zoom = i_context->zoom;

    // convert to Mercator coordinates
    CHECK( conv_spherical_to_mercator( i_lng, i_lat, zoom, &x_map, &y_map));
    CHECK( conv_spherical_to_mercator( center.lng, center.lat, zoom, &x_center, &y_center));

    // change referential
    *o_x = x_map - x_center + IMAGE_WIDTH / 2;
    *o_y = - y_map + y_center + IMAGE_HEIGHT / 2;

    return 0;

ERROR:
    return retCode;
}

/*
 * convert image coordinates to spherical coordinates
 */
int conv_image_to_spherical( t_context *i_context, int i_x, int i_y, float *o_lng, float *o_lat) {
    int retCode;
    int x_map, y_map, x_center, y_center;
    t_point center = i_context->center;
    int zoom = i_context->zoom;

    // compute coordinates of center in whole map
    CHECK( conv_spherical_to_mercator( center.lng, center.lat, zoom, &x_center, &y_center)); // TODO : only once in point structure

    // change referential from image to whole map
    x_map = i_x + x_center - IMAGE_WIDTH / 2;
    y_map = - i_y + y_center + IMAGE_HEIGHT / 2;

    // convert back to spherical coordinates
    CHECK( conv_mercator_to_spherical( x_map, y_map, zoom, o_lng, o_lat));

    return 0;

ERROR:
    return retCode;
}

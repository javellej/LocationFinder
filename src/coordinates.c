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
    
    // convert to radian
    lng_rd = i_lng * PI / 180;
    lat_rd = i_lat * PI / 180;

    // compute Mercator coordinates ( source : Wikipedia Mercator projection )
    mer_x = lng_rd; // reference longitude 0 is Greenwich meridian
    mer_y = log( tan( PI / 4 + lat_rd / 2));

    // convert to global map coordinates in pixels depending on zoom level
    // conversion is linear : PI -> 2^(z+7), map is a square
    *o_x = (int) ( mer_x * pow( 2, i_zoom + 7) / PI );
    *o_y = (int) ( mer_y * pow( 2, i_zoom + 7) / PI );

    return 0;

ERROR:
    return retCode;
}

/*
 * convert spherical coordinates to image coordinates where the center is specified
 */
int conv_spherical_to_image( t_point i_center, int i_zoom, float i_lng, float i_lat, int *o_x, int*o_y) {
    // TODO : check query point is inside image (after mercator conversion)
    int retCode;
    int x_map, y_map, x_center, y_center;

    // convert to Mercator coordinates
    CHECK( conv_spherical_to_mercator( i_lng, i_lat, i_zoom, &x_map, &y_map));
    CHECK( conv_spherical_to_mercator( i_center.lng, i_center.lat, i_zoom, &x_center, &y_center));

    // change referential
    *o_x = x_map - x_center + IMAGE_WIDTH / 2;
    *o_y = - y_map + y_center + IMAGE_HEIGHT / 2;

    return 0;

ERROR:
    return retCode;
}

/*
 * convert image pixel coordinates to maps coordinates
 */
int conv_image_to_map( t_point i_center, int i_x, int i_y, int zoom, float *o_lng, float *o_lat) {
    double lambda_lng = 180 * pow( 2, -7-zoom); // TODO : optimize power of 2
    double lambda_lat = 85 * pow( 2, -7-zoom); // TODO : optimize power of 2
    printf( "lambda lng = %lf lambda lat = %lf\n", lambda_lng, lambda_lat);
    double A = lambda_lng;
    double B = i_center.lng - A * IMAGE_WIDTH / 2;
    double C = - lambda_lat;
    double D = i_center.lat - C * IMAGE_HEIGHT / 2;

    *o_lng = A * i_x + B;
    *o_lat = C * i_y + D;
    return 0;
}

/*
 * convert maps coordinates to image pixel coordinates
 */
// TODO compute coefficients only once (initialize context)
int conv_map_to_image( t_point i_center, float i_lng, float i_lat, int zoom, int *o_x, int *o_y) {
    double lambda_lng = 180 * pow( 2, -7-zoom); // TODO : optimize power of 2
    double lambda_lat = 85 * pow( 2, -7-zoom); // TODO : optimize power of 2
    printf( "lambda lng = %lf lambda lat = %lf\n", lambda_lng, lambda_lat);
    double A = 1 / lambda_lng;
    double B = - A * i_center.lng + IMAGE_WIDTH / 2;
    double C = - 1 / lambda_lat;
    double D = - C * i_center.lat + IMAGE_HEIGHT / 2;

    *o_x = A * i_lng + B;
    *o_y = C * i_lat + D;

    return 0;
}

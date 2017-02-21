#include "overlay.h"
#include "image.h"
#include "error.h"
#include "definitions.h"
#include <string.h>

/*
 * initialize overlay structure
 * must be released afterwards
 */
int overlay_init( t_overlay *io_overlay) {
    int retCode;
    t_overlay overlay;
    int i;

    overlay.width = IMAGE_WIDTH;
    overlay.height = IMAGE_HEIGHT;
    overlay.overlay = (unsigned char **) malloc( overlay.height * sizeof( unsigned char *));
    if ( NULL == overlay.overlay ) { CHECK( ERROR_MEMORY_ALLOCATION); }
    for ( i=0; i<overlay.height; i++ ) {
        overlay.overlay[i] = (unsigned char *) malloc( overlay.width * sizeof( unsigned char));
        if ( NULL == overlay.overlay[i] ) { CHECK( ERROR_MEMORY_ALLOCATION); }
        memset( overlay.overlay[i], 0, overlay.width * sizeof( unsigned char));
    }

    *io_overlay = overlay;

    return 0;

ERROR:
    return retCode;
}

/*
 * release overlay structure and free memory
 */
int overlay_release( t_overlay *io_overlay) {
    int i;

    for ( i=0 ; i<io_overlay->height; i++ ) {
        free( io_overlay->overlay[i]);
        io_overlay->overlay[i] = NULL;
    }
    free( io_overlay->overlay);
    io_overlay->overlay = NULL;

    return 0;
}

/*
 * add an overlay to an existing image
 * the overlay acts like a semi-transparent mask
 */
int addOverlay( t_rgb_image io_image, t_overlay i_overlay) {
    int retCode;
    int x, y;

    // check dimensions
    if ( ( io_image.width != i_overlay.width ) ||
         ( io_image.height != i_overlay.height ) ) {
        CHECK( ERROR_DIMENSIONS);
    }

    // add stronger blue component to pixels when overlay pixel is present
    for ( y=0; y<io_image.height; y++ ) {
        for ( x=0; x<io_image.width; x++ ) {
            unsigned char curr_red = io_image.pixels[y][x].R;
            unsigned char new_red = ( curr_red <= 0xa0 ) ? 0x00 : curr_red - 0xa0;
            unsigned char curr_green = io_image.pixels[y][x].G;
            unsigned char new_green = ( curr_green <= 0xa0 ) ? 0x00 : curr_green - 0xa0;
            unsigned char curr_blue = io_image.pixels[y][x].B;
            unsigned char new_blue = ( curr_blue >= 0xa0 ) ? 0xff : curr_blue + 0xa0;
            if ( i_overlay.overlay[y][x] ) {
               io_image.pixels[y][x].R = new_red;
               io_image.pixels[y][x].G = new_green;
               io_image.pixels[y][x].B = new_blue;
            }
        }
    } 

    return 0;

ERROR:
    return retCode;
}

int addCircle( t_overlay io_overlay, int x_coord, int y_coord, int radius) {
    int x, y;
    int y_min, y_max, x_min, x_max;

    y_min = ( y_coord <= radius ) ? 0 : ( y_coord - radius );
    y_max = ( io_overlay.height - y_coord <= radius ) ? io_overlay.height : ( y_coord + radius );
    x_min = ( x_coord <= radius ) ? 0 : ( x_coord - radius );
    x_max = ( io_overlay.width - x_coord <= radius ) ? io_overlay.width : ( x_coord + radius );

    for ( y=y_min; y<y_max; y++ ) {
        for ( x=x_min; x<x_max; x++ ) { // TODO optimize : interior of square is simpler
            float x_dist = x - x_coord;
            float y_dist = y - y_coord;
            float square_dist = x_dist * x_dist + y_dist * y_dist;
            if ( square_dist <= ( radius * radius ) ) {
                io_overlay.overlay[y][x] = 1;
            }
        }
    }

    return 0;
}

int addSquare( t_overlay io_overlay, int x_coord, int y_coord, int side) {
    int x, y;
    int y_min, y_max, x_min, x_max;
    int half_side = side / 2;

    y_min = ( y_coord <= half_side ) ? 0 : ( y_coord - half_side );
    y_max = ( io_overlay.height - y_coord <= half_side ) ? io_overlay.height : ( y_coord + half_side );
    x_min = ( x_coord <= half_side ) ? 0 : ( x_coord - half_side );
    x_max = ( io_overlay.width - x_coord <= half_side ) ? io_overlay.width : ( x_coord + half_side );

    for ( y=y_min; y<y_max; y++ ) {
        for ( x=x_min; x<x_max; x++ ) {
            io_overlay.overlay[y][x] = 1;
        }
    }

    return 0;
}

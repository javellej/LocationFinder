#include "image.h"
#include "structs.h"
#include "definitions.h"
#include "error.h"
#include <png.h>

/*
 * read png file and output an RGB image
 */
int pngToRgb( char *i_png_file_name, t_rgb_image *o_image) {
    int retCode;
    //unsigned char header[8];
    png_structp png_ptr;
    png_infop info_ptr;
    int color_type, num_channels;
    t_rgb_image image;
    int x, y;

    // check png_byte type is actually a char
    if ( sizeof( png_byte) != sizeof( unsigned char) ) {
        CHECK( ERROR_TYPE_SIZE);
    }

    // open file and check if png format
    FILE *f = fopen( i_png_file_name, "rb");
    if ( NULL == f ) {
        CHECK( ERROR_FILE_OPEN);
    }
    /*fread( header, 1, 8, f);
    if ( png_sig_cmp( header, 0, 8) ) {
        CHECK( ERROR_FILE_FORMAT);
    }*/

    // initialize png structures
    png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    info_ptr = png_create_info_struct( png_ptr);
    if ( setjmp( png_jmpbuf( png_ptr)) ) { CHECK( ERROR_IMAGE_CREATION); }
    png_init_io( png_ptr, f);
    png_read_info( png_ptr, info_ptr);
    if ( IMAGE_WIDTH != png_get_image_width( png_ptr, info_ptr) ) {
        CHECK( ERROR_IMAGE_WIDTH);
    }
    if ( IMAGE_HEIGHT != png_get_image_height( png_ptr, info_ptr) ) {
        CHECK( ERROR_IMAGE_HEIGHT);
    }
    color_type = png_get_color_type( png_ptr, info_ptr);
    if ( PNG_COLOR_TYPE_PALETTE == color_type ) {
        num_channels = 4;
    } else if ( PNG_COLOR_TYPE_RGB == color_type ) {
        num_channels = 3;
    } else {
        CHECK( ERROR_COLOR_TYPE);
    }
    png_set_palette_to_rgb( png_ptr);
    png_set_filler( png_ptr, 0xFF, PNG_FILLER_AFTER);
    if ( 8 != png_get_bit_depth( png_ptr, info_ptr) ) {
        CHECK( ERROR_BIT_DEPTH);
    }
    png_read_update_info( png_ptr, info_ptr);


    // read png image to rgb image struct
    if ( setjmp( png_jmpbuf( png_ptr)) ) { CHECK( ERROR_IMAGE_CREATION); }
    image.width = IMAGE_WIDTH;
    image.height = IMAGE_HEIGHT;
    image.pixels = (t_pixel **) malloc( image.height * sizeof( t_pixel *));
    for ( y=0; y<image.height; y++ ) {
        image.pixels[y] = (t_pixel *) malloc( image.width * sizeof( t_pixel));
        png_bytep row = (png_bytep) malloc( png_get_rowbytes( png_ptr, info_ptr));
        png_read_row( png_ptr, row, NULL);
        for ( x=0; x<image.width; x++ ) {
            image.pixels[y][x].R = row[num_channels*x];
            image.pixels[y][x].G = row[num_channels*x+1];
            image.pixels[y][x].B = row[num_channels*x+2];
        }
    }
    //png_read_image( png_ptr, rows);

    *o_image = image;

    fclose( f);

    return 0;
 
ERROR:
        return retCode;
}

/*
 * write RGB image to png file
 */
int rgbToPng( t_rgb_image i_image, char *png_file_name) {
    int retCode;
    png_struct *png_ptr;
    png_info *info_ptr;

    FILE *f = fopen( png_file_name, "wb");
    if ( NULL == f ) {
        CHECK( ERROR_FILE_OPEN);
    }

    // initialize png structures
    png_ptr = png_create_write_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if ( NULL == png_ptr ) { CHECK( ERROR_IMAGE_CREATION); }
    info_ptr = png_create_info_struct( png_ptr);
    if ( NULL == info_ptr ) { CHECK( ERROR_IMAGE_CREATION); }
    if ( setjmp( png_jmpbuf( png_ptr)) ) { CHECK( ERROR_IMAGE_CREATION); }
    png_init_io( png_ptr, f);

    // write header
    png_set_IHDR( png_ptr, info_ptr, i_image.width, i_image.height,
                  8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                  PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    png_write_info( png_ptr, info_ptr);

    // write bytes
    png_bytep row = (png_bytep) malloc( 3 * i_image.width * sizeof(png_byte));
    int x, y;
    for ( y=0; y<i_image.height; y++ ) {
        for ( x=0; x<i_image.width; x++ ) {
            row[3*x] = i_image.pixels[y][x].R;
            row[3*x+1] = i_image.pixels[y][x].G;
            row[3*x+2] = i_image.pixels[y][x].B;
        }
        png_write_row( png_ptr, row);
    }
    png_write_end( png_ptr, NULL);

    fclose( f);

    // TODO : free image data

    return 0;

ERROR:
    return retCode;
}

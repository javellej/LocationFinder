#include "image.h"
#include "structs.h"
#include "definitions.h"
#include "error.h"
#include <png.h>

/*
 * read png file and output an RGBA image
 */
int pngToRgb( char *i_png_file_name, t_rgb_image *o_image) {
    int retCode;
    unsigned char header[8];
    png_structp png_ptr;
    png_infop info_ptr;
    png_bytep *rows;
    int width, height, color_type, bit_depth;
    t_rgb_image image;
    int i;

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
    setjmp( png_jmpbuf( png_ptr));
    png_init_io( png_ptr, f);
    //png_set_sig_bytes( png_ptr, 8);
    png_read_info( png_ptr, info_ptr);
    if ( IMAGE_WIDTH != ( width = png_get_image_width( png_ptr, info_ptr) ) ) {
        printf( "got image of width %d\n", width);
        CHECK( ERROR_IMAGE_WIDTH);
    }
    if ( IMAGE_HEIGHT != ( height = png_get_image_height( png_ptr, info_ptr) ) ) {
        printf( "got image of height %d\n", height);
        CHECK( ERROR_IMAGE_HEIGHT);
    }
    if ( PNG_COLOR_TYPE_PALETTE != ( color_type = png_get_color_type( png_ptr, info_ptr) ) ) {
        printf( "got color type %d\n", color_type);
        CHECK( ERROR_COLOR_TYPE);
    }
    png_set_palette_to_rgb( png_ptr);
    png_set_filler( png_ptr, 0xFF, PNG_FILLER_AFTER);
    if ( 8 != png_get_bit_depth( png_ptr, info_ptr) ) {
        CHECK( ERROR_BIT_DEPTH);
    }
    //number_of_passes = png_set_interlace_handling(png_ptr);
    png_read_update_info( png_ptr, info_ptr);


    // read png image
    setjmp( png_jmpbuf( png_ptr));
    rows = (png_bytep *) malloc( sizeof( png_bytep) * IMAGE_HEIGHT);
    for ( i=0; i<IMAGE_HEIGHT; i++ ) {
        rows[i] = (png_byte *) malloc( png_get_rowbytes( png_ptr, info_ptr));
    }
    png_read_image( png_ptr, rows);

    // translate png image to rgba image struct
    image.width = IMAGE_WIDTH;
    image.height = IMAGE_HEIGHT;
    image.pixels = (t_pixel **) rows;

    *o_image = image;

    fclose( f);

    return 0;
 
ERROR:
        return retCode;
}

/*
 * write RGBA image to png file
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
    info_ptr = png_create_info_struct(png_ptr);
    setjmp( png_jmpbuf( png_ptr));
    png_init_io( png_ptr, f);

    // write header
    setjmp( png_jmpbuf( png_ptr));
    png_set_IHDR( png_ptr, info_ptr, i_image.width, i_image.height,
                  8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
                  PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    png_write_info( png_ptr, info_ptr);


    // write bytes
    setjmp( png_jmpbuf( png_ptr));
    png_write_image( png_ptr, (png_byte **) i_image.pixels);
    setjmp( png_jmpbuf( png_ptr));
    png_write_end( png_ptr, NULL);

    fclose( f);

    // TODO : free image data

    return 0;

ERROR:
    return retCode;
}

/*
 * add an overlay to an existing image
 * the overlay acts like a semi-transparent mask
 */
int addOverlay( t_rgb_image io_image, t_overlay i_overlay) {
    int retCode;
    int i, j;

    // check dimensions
    if ( ( io_image.width != i_overlay.width ) ||
         ( io_image.height != i_overlay.height ) ) {
        CHECK( ERROR_DIMENSIONS);
    }

    // add stronger blue component to pixels when overlay pixel is present
    for ( i=0; i<io_image.height; i++ ) {
        for ( j=0; j<io_image.width; j++ ) {
            unsigned char new_blue_component = ( 0xff + io_image.pixels[i][j].B ) << 1;
            if ( i_overlay.overlay[i][j] ) {
               io_image.pixels[i][j].B = new_blue_component;
            }
        }
    } 

    return 0;

ERROR:
    return retCode;
}

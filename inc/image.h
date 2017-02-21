#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "structs.h"

int pngToRgb( char *i_png_file_name, t_rgb_image *o_image);
int rgbToPng( t_rgb_image i_image, char *png_file_name);

#endif /* _IMAGE_H_ */

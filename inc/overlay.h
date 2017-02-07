#ifndef _OVERLAY_H_
#define _OVERLAY_H_

#include "image.h"
#include "structs.h"

int addOverlay( t_rgb_image io_image, t_overlay i_overlay);
int addSquare( t_overlay io_overlay, int x_coord, int y_coord, int side);
int addCircle( t_overlay io_overlay, int x_coord, int y_coord, int radius);

#endif /* _OVERLAY_H_ */

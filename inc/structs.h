#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#include <stdlib.h>

typedef struct {
    int m_longitude;
    int m_latitude;
}t_point;

typedef struct {
    char *m_data;
    size_t m_size;
}t_buffer;

typedef struct {
    unsigned char R;
    unsigned char G;
    unsigned char B;
}t_pixel;

typedef struct {
    int width;
    int height;
    t_pixel **pixels;
}t_rgb_image;

typedef struct {
    int width;
    int height;
    unsigned char **overlay;
}t_overlay;

#endif

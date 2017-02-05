#ifndef _STRUCTS_H_
#define _STRUCTS_H_

typedef struct {
    int m_longitude;
    int m_latitude;
}t_point;

typedef struct {
    char *m_data;
    size_t m_size;
}t_buffer;

#endif

#ifndef TYPES_H
#define TYPES_H

typedef struct {
    float x;
    float y;
    float theta;
}coordinates_t;
typedef enum {
	left_t = 0,
	right_t = 1
}wheel_t;
#endif
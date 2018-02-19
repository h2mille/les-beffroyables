#ifndef ENCODER_H
#define ENCODER_H
#include <stdint.h>
#include "position.h"
#include "types.h"

#include "../libraries/rc_usefulincludes.h"
#include "../libraries/roboticscape.h"
class encoder{
    
    public:
    static void get_move(coordinates_t* coordinates_move,coordinates_t* speed_coordinates, float* speed, float* distance);
    private:

    
};

#endif
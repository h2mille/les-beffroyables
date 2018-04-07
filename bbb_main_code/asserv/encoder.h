#ifndef ENCODER_H
#define ENCODER_H
#include <stdint.h>
#include "position.h"
#include "types.h"

#include "../../Robotics_Cape_Installer/libraries/rc_usefulincludes.h"
#include "../../Robotics_Cape_Installer/libraries/roboticscape.h"
class encoder{
    
    public:
    static void get_move(coordinates_t* coordinates_move,coordinates_t* speed_coordinates, float* speed, float* distance);
    private:

    
};

#endif

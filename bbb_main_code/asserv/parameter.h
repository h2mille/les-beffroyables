#ifndef PARAMETER_H
#define PARAMETER_H
#include <stdint.h>


class parameter{
    public:
        parameter();
        float wheel_distance();
        float left_wheel_diameter();
        float right_wheel_diameter();
        int dotperturn();
        int period();
        float pastille_angle_depart();
        float pastille_distance();
        float pastille_angle_fin();
        void update_wheel_distance(float value);
        void update_left_wheel_diameter(float value);
        void update_right_wheel_diameter(float value);
        void update_dotperturn(int value);
        void update_period(int value);
        void update_pastille_angle_depart(float value);
        void update_pastille_distance(float value);
        void update_pastille_angle_fin(float value);
        void load_values();
        void save_values();
        void print_parameter();
 
    private:
        float pastille_angle_depart_v ;
        float pastille_distance_v     ;
        float pastille_angle_fin_v    ;
        void applySetting(char* settingName, char* settingValue);
        float wheel_distance_v        ;
        float left_wheel_diameter_v   ;
        float right_wheel_diameter_v  ;
        int   dotperturn_v            ;
        int   period_v                ;

};



#endif
#include "parameter.h"
#include <iostream>
#include <fstream>
#include <string.h>
using namespace std;
#include <unistd.h>

parameter robot_parameter;

parameter::parameter():wheel_distance_v(0),left_wheel_diameter_v(0),right_wheel_diameter_v(0),dotperturn_v(0){};

float parameter::wheel_distance()
{
    return wheel_distance_v;
}
float parameter::left_wheel_diameter()
{
    return left_wheel_diameter_v;
}
float parameter::right_wheel_diameter()
{
    return right_wheel_diameter_v;
}
int parameter::dotperturn()
{
    return dotperturn_v;
}
int parameter::period()
{
    return period_v;
}

float parameter::pastille_angle_depart()
{
    return pastille_angle_depart_v;
}
float parameter::pastille_distance()
{
    return pastille_distance_v;
}
float parameter::pastille_angle_fin()
{
    return pastille_angle_fin_v;
}
void  parameter::update_wheel_distance(float value)
{
    wheel_distance_v=value;
}
void  parameter::update_left_wheel_diameter(float value)
{
    left_wheel_diameter_v=value;
}
void  parameter::update_right_wheel_diameter(float value)
{
    right_wheel_diameter_v=value;
}
void  parameter::update_dotperturn(int value)
{
    dotperturn_v=value;
}
void  parameter::update_period(int value)
{
    period_v=value;
}
void parameter::update_pastille_angle_depart(float value)
{
    pastille_angle_depart_v=value;
}
void parameter::update_pastille_distance(float value)
{
    pastille_distance_v=value;
}
void parameter::update_pastille_angle_fin(float value)
{
    pastille_angle_fin_v=value;
}

void  parameter::load_values()
{
    char character='c';
    char settingName[50];
    char settingValue[15];
    std::string line,temp1,temp2;
    std::ifstream myFile ("asserv/parameter.txt", std::ios::in);
    if (myFile.is_open()) {
        while (!myFile.eof()) {
            char line[50],setting[50],value[50];
            myFile.getline(line,50);
            cout << line <<'\n';
            if(sscanf(line, "%s = %s", setting, value)!=2)
            {
                cout <<sscanf(line, "%s = %s", setting, value)<<'\n';
                cout<< "break\n";
            }
            else
            {
                if (strncmp(setting, "wheel_distance",14 )==0) {
                    sscanf(value, "%f", &wheel_distance_v);
                }
                if (strncmp(setting,"left_wheel_diameter",19)==0) {
                    sscanf(value, "%f", &left_wheel_diameter_v);
                }
                if (strncmp(setting, "right_wheel_diameter",20)==0) {
                    sscanf(value, "%f", &right_wheel_diameter_v);
                }
                if (strncmp(setting, "dotperturn",10)==0) {
                    sscanf(value, "%d", &dotperturn_v);
                }
                if (strncmp(setting, "period",6)==0) {
                    sscanf(value, "%d", &period_v);
                }
                if (strncmp(setting, "pastille_angle_depart",6)==0) {
                    sscanf(value, "%f", &pastille_angle_depart_v);
                }
                if (strncmp(setting, "pastille_distance",6)==0) {
                    sscanf(value, "%f", &pastille_distance_v);
                }
                if (strncmp(setting, "pastille_angle_fin",6)==0) {
                    sscanf(value, "%f", &pastille_angle_fin_v);
                }
            }
        }
    }

    // close the file:
    myFile.close();
}
void  parameter::save_values()
{
  std::ofstream myfile ("asserv/parameter.txt", std::ios::out | std::ios::trunc);
  myfile << "wheel_distance = "<<wheel_distance_v<< "\n";
  myfile << "left_wheel_diameter = "<<left_wheel_diameter_v<< "\n";
  myfile << "right_wheel_diameter = "<<right_wheel_diameter_v<< "\n";
  myfile << "dotperturn = "<<dotperturn_v<< "\n";
  myfile << "period = "<<period_v<< "\n";
  myfile << "pastille_angle_depart = "<<pastille_angle_depart_v<< "\n";
  myfile << "pastille_distance = "<<pastille_distance_v<< "\n";
  myfile << "pastille_angle_fin = "<<pastille_angle_fin_v<< "\n";
  myfile.close();
  return ;
}
void parameter::print_parameter()
{
  std::cout << "wheel_distance "<<wheel_distance()<< "\n";
  std::cout << "left_wheel_diameter "<<left_wheel_diameter()<< "\n";
  std::cout << "right_wheel_diameter "<<right_wheel_diameter()<< "\n";
  std::cout << "dotperturn "<<dotperturn()<< "\n";
  std::cout << "period "<<period()<< "\n";
  std::cout << "pastille_angle_depart "<<pastille_angle_depart()<< "\n";
  std::cout << "pastille_distance "<<pastille_distance()<< "\n";
  std::cout << "pastille_angle_fin "<<pastille_angle_fin()<< "\n";
}
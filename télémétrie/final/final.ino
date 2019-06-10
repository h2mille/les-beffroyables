
#include <Wire.h>
#include <VL53L0X.h>

#define SENSOR_NUMBER 3

#define NO_DETECT_THRETHOLD 5
#define DETECT_THRETHOLD 3

float capteur[SENSOR_NUMBER][3];
char incomingBytes[100];

int FIFO[SENSOR_NUMBER][10];
uint8_t FIFO_position[SENSOR_NUMBER];
uint8_t no_detect[SENSOR_NUMBER];
uint8_t detect[SENSOR_NUMBER];
//uint8_t FIFO_coef[10]={10,8,6,5,4,3,2,1,1,1};
uint8_t FIFO_coef[10]  ={10,6,4,3,2,2,1,1,1,1};
uint8_t FIFO_sum;

VL53L0X sensor[SENSOR_NUMBER];


float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void setup() {
  uint8_t i;
  Wire.setClock(400000);
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("boot");
  capteur[0][0]=80;
  capteur[0][1]=30;
  capteur[0][2]=PI;

  capteur[1][0]=-80;
  capteur[1][1]=30;
  capteur[1][2]=PI;
 
  capteur[2][0]=15;
  capteur[2][1]=160;
  capteur[2][2]=0;

  Serial.println("set i2c");
  Wire.begin();

  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  digitalWrite(7,LOW);

  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);

  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  digitalWrite(7,LOW);




  delay(100);
  pinMode(5,INPUT);
  delay(100);
  sensor[0].init();
  sensor[0].setAddress(12);
  Serial.println("done.");
  pinMode(7,INPUT);
  delay(100);
  sensor[1].init();
  sensor[1].setAddress(10);
  pinMode(6,INPUT);
  delay(100);
  sensor[2].init();
  sensor[2].setAddress(18);

  for(i=0;i<SENSOR_NUMBER;i++)
  {
    sensor[i].setTimeout(500);
    sensor[i].setSignalRateLimit(0.25);
    sensor[i].setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
    sensor[i].setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
    FIFO_position[i]=0;
    no_detect[i]=0;
    detect[i]=0;
  }
  Serial.println("done.");
  for(i=0;i<SENSOR_NUMBER;i++)
  {
    Serial.print("sensor: ");
    Serial.println(i);
    Serial.println(sensor[i].getAddress());
    Serial.println(sensor[i].getSignalRateLimit());
    Serial.println(sensor[i].readRangeSingleMillimeters());
    sensor[i].startContinuous();
  }
  Serial.println("end setup");

}


void parse_float(float* values,char* incomingBytes)
{
  char* c=incomingBytes;
  int8_t i,j;

  for(j=0;j<3;j++)
  {
    values[j]=0;
    i=-1;
    while((c[0]!=' ')&&(c[0]!='\0')&&(c[0]!='\r')&&(c[0]!='\n'))
    {
      if(c[0]=='.')
      {
        i=0;
      }
      else
      {
        values[j]*=10;
        values[j]+=(*c-'0');
        if(i!=-1)
          i++;
      }
      c++;
    }
    while(i>0)
    {
      values[j]/=10;
      i--;
    }
    while((c[0]==' '))
      c++;
  }
}

void add_coordinates(float* fisrt_coordinates,float* second_coordinates, float* coordinates_output){
    coordinates_output[0]=fisrt_coordinates[0] - second_coordinates[0] * sin(fisrt_coordinates[2]) + second_coordinates[1] * cos(fisrt_coordinates[2]);
    coordinates_output[1]=fisrt_coordinates[1] + second_coordinates[0] * cos(fisrt_coordinates[2]) + second_coordinates[1] * sin(fisrt_coordinates[2]);
    coordinates_output[2]=fisrt_coordinates[2] + second_coordinates[2];

}

int quickReadByteUntil(char c,char* char_array, int size)
{                   
  char d;
  int e=0;
  do{
    d=Serial.read();
    char_array[e]=d;
    e++;
    delay(1);
  }while((Serial.available()> 0)&&(c!=d) && (e<size));
  return e;  
}

void add_fifo(){
  uint8_t i;
  int temp[SENSOR_NUMBER];
  for(i=0;i<SENSOR_NUMBER;i++)
  {
      temp[i]=sensor[i].readRangeContinuousMillimeters();
  }
  for(i=0;i<SENSOR_NUMBER;i++)
  {
//    Serial.print("I got something");
    if(temp[i]>2500)
    {
      if(no_detect[i]<NO_DETECT_THRETHOLD)
        {
          no_detect[i]++;   
      }
      else
      {
        FIFO_position[i]= (FIFO_position[i]+1)%10;
        FIFO[i][FIFO_position[i]]= 2500;
//        Serial.println("nothing detected for too long");
        detect[i]=0;
      }
    }
    else
    {
      if(detect[i]<DETECT_THRETHOLD)
      {
        detect[i]++;
      }
      else
      {    
        FIFO_position[i]= (FIFO_position[i]+1)%10;
        FIFO[i][FIFO_position[i]]= temp[i];
        no_detect[i]=0;
      }
    }
  }
}

void read_fifo(int32_t tab[]){
  uint8_t i,j;
  for(i=0;i<SENSOR_NUMBER;i++)
    tab[i]=FIFO[i][FIFO_position[i]];
//  tab[0]=0;tab[1]=0;tab[2]=0;
//  for(j=0;j<3;j++){
////    Serial.println();
////    Serial.print(j);
////    Serial.print(" ");
//    for(i=0;i<10;i++)
//    {
////    Serial.print(FIFO[j][i]);
////    Serial.print("\t");
////    Serial.print(FIFO[j][i]*FIFO_coef[(FIFO_position[j]-i+10)%10]);
////    Serial.print("\t");
//      tab[j]+=(uint32_t)FIFO[j][i]*FIFO_coef[(FIFO_position[j]-i+10)%10];
//    }
//  }
//
//  tab[0]/=FIFO_sum;
//  tab[1]/=FIFO_sum;
//  tab[2]/=FIFO_sum;

}

void print_coordinates(float* value)
{
     Serial.print(value[0]);
     Serial.print(" ");
     Serial.print(value[1]); 
     Serial.print(" ");
     Serial.println(value[2]); 
}

unsigned long time;
void loop() {
  // put your main code here, to run repeatedly:
  float coord[3];
  float robot_coord[SENSOR_NUMBER][3];
  float coord_temp[3];
  int temp;
  int32_t tab[3];


  add_fifo();
  read_fifo(tab);
  capteur[2][2]=mapfloat(analogRead(A0),216,600,-0.7853,0.7853);
  
//    Serial.print(tab[0]);
//    Serial.print(" ");
//    Serial.print(tab[1]);
//    Serial.print(" ");
//    Serial.println(tab[2]);

  if (Serial.available() > 0)
  {
    time=millis();
//    Serial.print("Time: "); 
//    Serial.println(millis()-time);
    uint8_t result_sum=0;
    uint8_t result=0;
    bool check;

    Serial.print("analog value: ");
    Serial.println(analogRead(A0));

    temp=quickReadByteUntil('\0', incomingBytes, sizeof(incomingBytes) / sizeof(char) );
    incomingBytes[temp]='\0';
    parse_float(coord,incomingBytes);
    for(temp=0;temp<SENSOR_NUMBER;temp++)
    {
//      Serial.print("robot coordinates: ");
//      print_coordinates(coord);

      add_coordinates(coord,capteur[temp],robot_coord[temp]);
//      Serial.print("sensor coordinates coordinates: ");
//      print_coordinates(robot_coord[temp]);
      coord_temp[0]=0;
      coord_temp[1]=tab[temp];
      coord_temp[2]=0;
       add_coordinates(robot_coord[temp],coord_temp,robot_coord[temp]);
//      Serial.print("object coordinates: ");
//      print_coordinates(robot_coord[temp]);

      if((robot_coord[temp][0]>-1400) && (robot_coord[temp][0]<1400) && (robot_coord[temp][1]>100) && (robot_coord[temp][1]<1900))
      {
        result_sum++;
        result |= 1<<temp;
        check=true;
      }
      else
        check=false;
      if ((check == true) && ( tab[temp]<400))
        Serial.print("1 ");
      else
        Serial.print("0 ");
    }
    
    Serial.println(result_sum);
    
    if((result&1)!=0)
    {
        Serial.print(robot_coord[0][0]);
        Serial.print(" ");
        Serial.println(robot_coord[0][1]);
    }
    if((result&2)!=0)
    {
        Serial.print(robot_coord[1][0]);
        Serial.print(" ");
        Serial.println(robot_coord[1][1]);
    }
    if((result&4)!=0)
    {
        Serial.print(robot_coord[2][0]);
        Serial.print(" ");
        Serial.println(robot_coord[2][1]);
    }

//    Serial.print("Time: ");
//    Serial.println(millis()-time);
//    Serial.println();

  }
}

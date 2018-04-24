
#include <Wire.h>
#include <VL53L0X.h>

#define SENSOR_NUMBER 4

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

void setup() {
  uint8_t i;
  // put your setup code here, to run once:
  Serial.begin(115200);
  capteur[0][0]=-200;
  capteur[0][1]=200;
  capteur[0][2]=PI/2;

  capteur[1][0]=200;
  capteur[1][1]=200;
  capteur[1][2]=PI/2;
 
  capteur[2][0]=-200;
  capteur[2][1]=-200;
  capteur[2][2]=-PI/2;

  capteur[3][0]=200;
  capteur[3][1]=-200;
  capteur[3][2]=-PI/2;
  Wire.begin();
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  digitalWrite(3,HIGH);
  digitalWrite(4,LOW);
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  sensor[0].init();
  sensor[0].setAddress(0X10);
  digitalWrite(4,HIGH);
  sensor[1].init();
  sensor[1].setAddress(0X12);
  digitalWrite(5,HIGH);
  sensor[2].init();
  sensor[2].setAddress(0X14);
  digitalWrite(6,HIGH);
  sensor[3].init();
  sensor[3].setAddress(0X16);
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
    coordinates_output[0]=fisrt_coordinates[0] - second_coordinates[0]*sin(fisrt_coordinates[2])+second_coordinates[0]*cos(fisrt_coordinates[2]);
    coordinates_output[1]=fisrt_coordinates[1] + second_coordinates[1]*cos(fisrt_coordinates[2])+second_coordinates[1]*sin(fisrt_coordinates[2]);
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
    temp[i]=sensor[i].readRangeContinuousMillimeters();
  for(i=0;i<SENSOR_NUMBER;i++)
  {
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
        Serial.println("nothing detected for too long");
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
  if (Serial.available() > 0)
  {
    time=millis();
    Serial.print("Time: "); 
    Serial.println(millis()-time);

    temp=quickReadByteUntil('\0', incomingBytes, sizeof(incomingBytes) / sizeof(char) );
    incomingBytes[temp]='\0';
    parse_float(coord,incomingBytes);
    for(temp=0;temp<SENSOR_NUMBER;temp++)
    {
      add_coordinates(coord,capteur[temp],robot_coord[temp]);
      coord_temp[0]=0;
      coord_temp[1]=tab[temp];
      coord_temp[2]=0;
      add_coordinates(robot_coord[temp],coord_temp,robot_coord[temp]);
      Serial.print("robot ");
      Serial.println(temp);
      Serial.println(robot_coord[temp][0]);
      Serial.println(robot_coord[temp][1]);
      Serial.println();
    }
    Serial.print("Time: ");
    Serial.println(millis()-time);
    Serial.println();
  }

}

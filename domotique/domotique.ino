#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>
 
#define PIN      4
#define N_LEDS  55

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);

#define LARGEUR  11
#define HAUTEUR  5
#define LARGEUR_LETTRE  (3-1)
#define HAUTEUR_LETTRE  (5-1)

int valeur = 0;
int valeur2 = 0;
int valeur3 = 0;

uint8_t tableau_lettre[10][5]= {{0b111,0b101,0b101,0b101,0b111},\
                               {0b010,0b110,0b010,0b010,0b111},\
                               {0b111,0b001,0b111,0b100,0b111},\
                               {0b111,0b001,0b111,0b001,0b111},\
                               {0b101,0b101,0b111,0b001,0b001},\
                               {0b111,0b100,0b111,0b001,0b111},\
                               {0b111,0b100,0b111,0b101,0b111},\
                               {0b111,0b001,0b001,0b001,0b001},\
                               {0b111,0b101,0b111,0b101,0b111},\
                               {0b111,0b101,0b111,0b001,0b111}};

//0-3 => todo

int delayval = 500; // delay for half a second

const char* ssid = "benaur";
const char* password = "benjaminaurelie";

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {
  strip.begin();
  delay(10);
  // prepare GPIO4 Interupteur front
  pinMode(4, OUTPUT);
  digitalWrite(4, 0);
  WiFi.begin(ssid, password);
  // Start the server
  server.begin();
//  cc();
}

void display(int val,uint8_t  pos, byte red, byte green, byte blue){
int8_t digit=0;
int8_t i;
int8_t j;
for(i=0;i<3;i++)
  {
  for(j=0;j<5;j++)
    {

//4 est la largeur d'une lettre
      uint8_t led_position=(pos)+(LARGEUR_LETTRE-i)+j*LARGEUR;
      if(((tableau_lettre[val][j]) & (1<<i)) !=0)
        strip.setPixelColor(led_position  ,red, green, blue);
      else
        strip.setPixelColor(led_position  ,0, 0, 0);
    }
  strip.show();
  }
}

void loop() 
{
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while(!client.available()){
    delay(1);
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  client.flush();
  
  // Match the request
  bool val0, val4, val5, val16;
  val0=digitalRead(0);
  val4=digitalRead(4);
  val5=digitalRead(5);
  val16=digitalRead(16);

  String a = req.substring(5,6);
  String b = req.substring(6,7);
  String c = req.substring(7,8);

  if (a="c"){
      for(uint16_t i=0; i<strip.numPixels()+4; i++) {
            strip.setPixelColor(i  ,0, 0, 0); // Draw new pixel
            strip.show();
      }
  
  }
  
  if(a!="e")
  valeur = a.toInt();
  valeur2 = b.toInt();
  valeur3 = c.toInt();
  
  display(valeur,0, 255, 0, 0);
  display(valeur2,4, 255, 0, 0);
  display(valeur3,8, 255, 0, 0);

  //display(valeur,4, 255, 0, 0);
  //display(valeur,7, 255, 0, 0);
  client.flush();
    // Prepare the response 
 String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";
  if (req.indexOf("/etat/") != -1 ){

int8_t digit=0;
int8_t i;
int8_t j;
for(i=0;i<3;i++)
  {
  for(j=0;j<5;j++)
    {

//4 est la largeur d'une lettre
      uint8_t led_position=0+(LARGEUR_LETTRE-i)+(j*LARGEUR);
        s +="<br>";
        s+=led_position;
        s +=" ";
        s+=i;
        s +=" ";
        s+=j;
        s +=" ";
        s+=(tableau_lettre[valeur][j]) & (1<<i);
      if(((tableau_lettre[valeur][j]) & (1<<i)) !=0)
      {
        s +=" All<br>";
        }
      else
      { 
        s +=" Zero<br>";
      }
    }
  }
    s += "</html>\n";

    for(uint16_t i=0; i<strip.numPixels(); i++) {
            strip.setPixelColor(i  ,0, 0, 0); // Draw new pixel
            strip.show();
      }
  }
  // Send the response to the client
  client.print(s);
  delay(1);
  
} 

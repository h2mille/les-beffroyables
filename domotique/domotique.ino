#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>

 
#define PIN      4
#define N_LEDS  110

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);

#define LARGEUR  22
#define HAUTEUR  5
#define LARGEUR_LETTRE  (3-1)
#define HAUTEUR_LETTRE  (5-1)

int valeur = 0;
int valeur2 = 0;
int valeur3 = 0;

int aa=0;
int dd=22;
int ee=44;
int gg=66;
int hh=88;

uint8_t tableau_lettre[14][5]= {{0b111,0b101,0b101,0b101,0b111},\
                               {0b001,0b001,0b001,0b001,0b001},\
                               {0b111,0b001,0b111,0b100,0b111},\
                               {0b111,0b001,0b111,0b001,0b111},\
                               {0b101,0b101,0b111,0b001,0b001},\
                               {0b111,0b100,0b111,0b001,0b111},\
                               {0b111,0b100,0b111,0b101,0b111},\
                               {0b111,0b001,0b001,0b001,0b001},\
                               {0b111,0b101,0b111,0b101,0b111},\
                               {0b111,0b101,0b111,0b001,0b111},\
                               {0b111,0b100,0b110,0b100,0b111},\
                               {0b100,0b100,0b100,0b100,0b111},\
                               {0b111,0b111,0b111,0b111,0b111},\
                               {0b010,0b010,0b010,0b010,0b010}};

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
void ccc(){
    for(uint16_t i=0; i<strip.numPixels(); i++) {
            strip.setPixelColor(i  ,0, 0, 0); // Draw new pixel
            strip.show();
      }
}
void display(int val,uint8_t  pos, byte red, byte green, byte blue){
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
  String d = req.substring(8,9);

  valeur = a.toInt();
  valeur2 = b.toInt();
  valeur3 = c.toInt();
  int  couleur = d.toInt();

  if (d == "v"){
  display(valeur,0, 0, 255, 0);
  display(valeur2,4, 0, 255, 0);
  display(valeur3,8, 0, 255, 0);
  }
else  if (d == "b")
  { 
  display(valeur,0, 0, 0, 255);
  display(valeur2,4, 0, 0, 255);
  display(valeur3,8, 0, 0, 255);
  } 
else  if (d == "j")
  { 
  display(valeur,0, 255, 255, 0);
  display(valeur2,4, 255, 255, 0);
  display(valeur3,8, 255, 255, 0);
  }
  else  if (d == "o")
  { 
  display(valeur,0, 255, 69, 0);
  display(valeur2,4, 255, 69, 0);
  display(valeur3,8, 255, 69, 0);
  }
  else if (d == "r")
  {
  display(valeur,0, 255, 0, 0);
  display(valeur2,4, 255, 0, 0);
  display(valeur3,8, 255, 0, 0);    
  }
  else if (d == "p")
  {
  display(valeur,0, 255, 0, 255);
  display(valeur2,4, 255, 0, 255);
  display(valeur3,8, 255, 0, 255);    
  }
  else if (d == "w")
  {
  display(valeur,0, 42, 62, 80);
  display(valeur2,4, 42, 62, 80);
  display(valeur3,8, 42, 62, 80);    
  }
  client.flush();
    // Prepare the response 
 String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";

 
  if (req.indexOf("cc") != -1 ){
    aa=0;
    dd=22;
    ee=44;
    gg=66;
    hh=88;
    ccc();
  }

if (req.indexOf("aa") != -1 ){
  aa++;
  for(uint16_t i=0; i<aa; i++) {
      strip.setPixelColor(i  ,255, 0, 0); // Draw new pixel
      strip.show();
  }
  if ( aa == 11){
   for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i  ,255, 0, 0); // Draw new pixel
      strip.show();
  }
  delay(3000);
  ccc();     
}
}
if (req.indexOf("dd") != -1 ){
  dd++;
  for(uint16_t i=22; i<dd; i++) {
      strip.setPixelColor(i  ,0, 0, 255); // Draw new pixel
      strip.show();
  }      
  if ( dd == 33){
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i  ,0, 0, 255); // Draw new pixel
      strip.show();
  }
  delay(3000);
  ccc();
  }
}
if (req.indexOf("ee") != -1 ){
  ee++;
  for(uint16_t i=44; i<ee; i++) {
      strip.setPixelColor(i  ,0, 255, 0); // Draw new pixel
      strip.show();
  }
  if ( ee == 55){
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i  ,0, 255, 0); // Draw new pixel
      strip.show();
  }
  delay(3000);
  ccc();
  }
}
if (req.indexOf("gg") != -1 ){
  gg++;
  for(uint16_t i=66; i<gg; i++) {
      strip.setPixelColor(i  ,255, 255, 0); // Draw new pixel
      strip.show();
  }      
  if ( gg == 77 ){
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i  ,255, 255, 0); // Draw new pixel
      strip.show();
  }
  delay(3000);
  ccc();
  }
}
if (req.indexOf("hh") != -1 ){
  hh++;
  for(uint16_t i=88; i<hh; i++) {
      strip.setPixelColor(i  ,42, 62, 80); // Draw new pixel
      strip.show();
  }      
  if ( hh == 99 ){
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i  ,42, 62, 80); // Draw new pixel
      strip.show();
  }
    delay(3000);
    ccc();    
  }
}
//drapeaux 

if (req.indexOf("ff") != -1 ){
  display(12,0, 0, 0, 255);
  display(12,4, 255, 255, 255);
  display(12,8, 255, 0, 0);
  } 

 if (req.indexOf("lil") != -1 ){
  display(11,0, 255, 255, 0);
  display(13,4, 42, 62, 80);
  display(11,8, 255, 255, 0);
  }  

 if (req.indexOf("jam") != -1 ){
  display(12,0, 0, 255, 0);
     delay(1000);   
  display(12,4, 255, 255, 0);
    delay(1000);   
  display(12,8, 255, 0, 0);
  }  
  
  if (req.indexOf("/etat/") != -1 ){
    s += req;
    s +=" --<br>";
        s += a;
    s +=" --<br>";
        s += b;
    s +=" --<br>";
        s += req;
    s +=" --<br>";
    s +=" Couleur<br>";
    s += d;
    s += "</html>\n";


  }
  // Send the response to the client
  client.print(s);
  delay(1);
  
} 

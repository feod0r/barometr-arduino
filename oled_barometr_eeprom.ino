#include <DHT.h>
#include <DHT_U.h>

#include <OLED_I2C.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <EEPROM.h>

#define DHTPIN 3         //define as DHTPIN the Pin 3 used to connect the Sensor
#define DHTTYPE DHT11    //define the sensor used(DHT11)


OLED  myOLED(SDA, SCL, 8);

DHT dht(DHTPIN, DHTTYPE);//create an instance of DHT

//константы
#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11 
#define BMP_CS 10
extern uint8_t SmallFont[];
Adafruit_BMP280 bme(BMP_CS);

//byte logs[40];
int counter=0;

//Инициализация
void setup()
{
  
  myOLED.begin();
  myOLED.setFont(SmallFont);
  bme.begin();
  delay (1000);
  Update();
  /*for (byte i=0; i<40; i++){
    Push(random(0,55));
    //logs[i] = random(0,55);
  }*/
}

//главный цикль
void loop()
{
  myOLED.clrScr();
  char pressure[6] = "";
  char temperature[6] = "";
  String hyd = String((int) dht.readHumidity());
  dtostrf(bme.readPressure()*0.00750062,5,2,pressure);
  dtostrf(bme.readTemperature(),4,1,temperature);
  //dtoint(dht.readHumidity(),4,hyd);
  myOLED.print(hyd, CENTER, 0);
  myOLED.print("%", 70, 0);
  myOLED.print(pressure, LEFT, 0);
  //String count = String((int) ((bme.readPressure()*0.00750062-727)*2 + 0.51) );
  //myOLED.print(count, 10, 8);
  myOLED.print(F("mm"), 36, 0);
  myOLED.print(temperature, RIGHT, 0);
  myOLED.print(F("C~"), 128-38, 0);
  if(counter == 3600){
    Update();
  }
  DrawXY();
  DrawGraph();
  myOLED.update();
  delay (1000);
  counter++;
}

//рисовка кординатной сетки
void DrawXY(){
  myOLED.drawLine(0, 8, 0, 63);
  myOLED.drawLine(0, 63, 127, 63);
  myOLED.setPixel(1,9);
  myOLED.setPixel(125,62);
  myOLED.drawLine(1,10,3,10);
  myOLED.drawLine(124,63,124,61);
  myOLED.drawLine(67,63,67,60);
  //myOLED.print("Atmosphere pressure", CENTER, 16);
  //myOLED.print("NOPQRSTUVWXYZ", CENTER, 24);
}

void DrawGraph(){
  for(byte i = 0; i<40; i++){
    myOLED.drawLine((i)*3+2,63-EEPROM[i],(i+1)*3+2,63-EEPROM[i+1]);
    //myOLED.setPixel((i)*3+2,63-EEPROM[i]);
   // myOLED.update();
  }
}

void Push(int buff){
  EEPROM[40]=buff;
  for(byte i=0; i<40; i++){
    
    EEPROM[i]=EEPROM[i+1];
  }
}
void Update(){
  Push((int) ((bme.readPressure()*0.00750062-727)*2 + 0.51) );   
  //randomSeed(analogRead(0));
  //Push(random(20,55));
  counter = 0;
}


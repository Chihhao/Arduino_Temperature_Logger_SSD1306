// https://github.com/jarzebski/Arduino-DS3231
// https://github.com/Marzogh/SPIMemory
// https://github.com/adafruit/Adafruit_Sensor
// https://github.com/greiman/SSD1306Ascii
// https://github.com/adafruit/DHT-sensor-library

#include <Wire.h>              //for OLED
#include <SSD1306Ascii.h>      //for OLED
#include <SSD1306AsciiWire.h>  //for OLED

#include <DHT.h>
#include <DS3231.h>

#include <SPIMemory.h>
#define CAPA 134217728   //128M
#define ARRSZ 32
uint32_t gAdr=0;

#define DHTPIN 2
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);
SPIFlash flash;

#define I2C_ADDRESS 0x3C
#define RST_PIN -1
SSD1306AsciiWire oled;

double hum;  //Stores humidity value
double temp; //Stores temperature value

DS3231 clock;
RTCDateTime dt;

char text[ARRSZ]; 
char textAdr[32];

int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
void setup() {
  Serial.begin(115200);  //115200
  delay(100);
  
  Wire.begin();            //for OLED
  Wire.setClock(400000L);  //for OLED
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.clear();
  oled.setFont(ZevvPeep8x16);  
  oled.println("Ver: 2021/5/23");
  oled.println("START.....");
  
  
  dht.begin();  
  clock.begin();
  digitalWrite(LED_BUILTIN, LOW); 
  
  Serial.print(F("START: free memory:"));
  Serial.println(freeRam());
  
  flash.begin(MB(128));
  gAdr = findIdxOfFlash();
  delay(100);  
}
void loop() {  
  dt = clock.getDateTime();
  int yy = dt.year;
  int mm = dt.month;
  int dd = dt.day;
  int hh = dt.hour;
  int mn = dt.minute;
  int ss = dt.second;

  oled.home();
  oled.setFont(ZevvPeep8x16);
  sprintf(text, " %02d/%02d %02d:%02d:%02d ", mm, dd, hh, mn, ss);  
  oled.println(text);  
  if(ss%4!=0)return;
  
  oled.println("----------------");  
   
  hum = dht.readHumidity() - 5.0;
  temp= dht.readTemperature() - 0.4;
  char humFS[10], tempFS[10];
  dtostrf(hum,2,1,humFS);
  dtostrf(temp,2,1,tempFS);

  oled.setFont(fixed_bold10x15);
  sprintf(text, "Temp: %sc", tempFS);
  oled.println(text);   
  sprintf(text, "Humi: %s%%", humFS);
  oled.println(text);   

  //20191126 112233 23.5 50.4
  sprintf(text, "%4d%02d%02d %02d%02d%02d %s %s", 
                yy, mm, dd, hh, mn, ss, tempFS, humFS);
  writeToFlash(text);

  delay(800);
}

void writeToFlash(char* str){
  if(gAdr>(CAPA-ARRSZ)){   
    myPrintHex(gAdr);
    Serial.println(" reset gAdr=0"); 
    gAdr=0; 
  }
  if(gAdr%4096==0){ erase4K(gAdr); }
  
  if (flash.writeCharArray(gAdr, str, ARRSZ, true)) {
    myPrintHex(gAdr);
    Serial.print(F(" W: ")); Serial.println(str);

    /*
    Serial.print(textAdr); 
    Serial.print(F(" R: ")); 
    if (flash.readCharArray(gAdr, textAdr, ARRSZ)) {
      Serial.println(textAdr); }     
    else{
      Serial.println("read fail!"); }   
    */
         
    gAdr+=ARRSZ;
  }
}

uint32_t findIdxOfFlash(){
  unsigned long adr=0;
  bool gotIndex=false;
  
  for(adr=0; adr<CAPA; adr+=ARRSZ){
    if(flash.readByte(adr)==0xFF){
      gotIndex=true;
      Serial.print(F("findIdx: "));
      myPrintHex(adr); Serial.println();      
      break;
    }
  }
  if(gotIndex==false){
    Serial.print("gotIndex==false");
    adr=0;    
  }
  return adr;  
}

void erase4K(unsigned long addr){
  myPrintHex(addr);
  if (flash.eraseSector(addr)) {
    Serial.println(F(" erase 4KB"));
  }
  else {
    Serial.println(F("Erasing sector failed"));
  } 
  //delay(10);
}

void myPrintHex(uint32_t inputInt32){
  if(inputInt32>0x0FFFFFFF){
    Serial.print("0x");
    Serial.print(inputInt32, HEX);
  }
  else if (inputInt32>0x00FFFFFF){
    Serial.print("0x0");
    Serial.print(inputInt32, HEX);
  }
  else if (inputInt32>0x000FFFFF){
    Serial.print("0x00");
    Serial.print(inputInt32, HEX);
  }
  else if (inputInt32>0x0000FFFF){
    Serial.print("0x000");
    Serial.print(inputInt32, HEX);
  }
  else if (inputInt32>0x00000FFF){
    Serial.print("0x0000");
    Serial.print(inputInt32, HEX);
  }
  else if (inputInt32>0x000000FF){
    Serial.print("0x00000");
    Serial.print(inputInt32, HEX);
  }
  else if (inputInt32>0x0000000F){
    Serial.print("0x000000");
    Serial.print(inputInt32, HEX);
  }
  else {
    Serial.print("0x0000000");
    Serial.print(inputInt32, HEX);
  }
  
}

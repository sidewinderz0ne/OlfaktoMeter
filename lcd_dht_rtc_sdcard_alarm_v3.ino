 const long alarmTime = 60000; //Mili detik
int buzzerDuration = 1000; //Mili Detik
int buzzerSound = 4000; //KHz
long alarm;
int mill;
int onTime;
int trigTime;

int buzzer = 33;

bool setWaktu = false;
int jam = 14;                                                                                                                                                                                                                                                                                    
int menit = 31;
int detik = 30;
int tanggal = 19;
int bulan = 6;
int tahun = 2020;

#include "U8glib.h"
U8GLIB_ST7920_128X64_1X u8g(5, 6, 8, 9, 10, 11, 12, 13, 4, 2, 3);// 8Bit Com: D0..D7: 5,6,8,9,10,11,12,13 Enable=4, RS=2,R/W=3
#include <DS3231.h>
DS3231  rtc(SDA, SCL);
int pinCS  = 53;
int chk;

#include <DHT.h>;
DHT dht(30, DHT22);
DHT dht2(31, DHT22);
float hum;  
float hum2;
float temp;
float temp2;

char h [5];
char h2[5];
char t [5];
char t2 [5];
char str1 [20];

#include <SD.h> 
#include <SPI.h>
File myFile;

void draw(void) {
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 0, 60, "www.SRS-SSMS.com");
  char* waktu = "RTC ERROR";
  if (rtc.getTimeStr() !=  '\0'){
    waktu = rtc.getTimeStr();
  }
  u8g.drawStr( 32, 15, waktu);
  //buzz();
  u8g.drawStr( 0, 30, "IN :");
  u8g.drawStr( 0, 45, "OUT:");
  u8g.drawStr( 66, 30, "%");
  u8g.drawStr( 66, 45, "%");
  u8g.drawStr( 117, 30, "C");
  u8g.drawStr( 117, 45, "C");
  u8g.drawStr( 35, 30, (h) );
  u8g.drawStr( 35, 45, (h2) );
  u8g.drawStr( 84, 30, (t) );
  u8g.drawStr( 84, 45, (t2) );
}

void buzz(void){
 tone(buzzer, buzzerSound);
 delay(buzzerDuration);
 noTone(buzzer);
}

void drawSdFailed(void) {
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 0, 10, "Memori tdk Terbaca");
  u8g.drawStr( 0, 25, "Silahkan reset");
  delay(50);
}

void setup(void) {
  //Serial.begin(115200);
  alarm = alarmTime;
  pinMode(buzzer, OUTPUT);
  pinMode(pinCS, OUTPUT);
  
  rtc.begin();
  if (setWaktu){
  rtc.setDate(tanggal, bulan, tahun); //hapus
  rtc.setTime(jam, menit, detik);    
  }
  dht.begin();
  dht2.begin();
  // flip screen, if required
  // u8g.setRot180();
  
  // set SPI backup if required
  //u8g.setHardwareBackup(u8g_backup_avr_spi);

  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }

    // SD Card Initialization
  if (SD.begin())
  {
  } else
  { 
      u8g.firstPage();  
  do {
    drawSdFailed();
  } while( u8g.nextPage() );
    return;
  }

  myFile = SD.open("hasil.txt", FILE_WRITE);
      if (myFile) {    
        myFile.print("Tanggal, Jam, Humidity IN, Humdity OUT, Temperature IN, Temperature OUT");
        myFile.println();
        myFile.close(); // close the file
        Serial.println("sd berhasil");
      }
      // if the file didn't open, print an error:
      else {
        Serial.println("sd gagal");
        u8g.firstPage();  
        do {
          drawSdFailed();
        } while( u8g.nextPage() );
      }

  logging();
}

int timeToInt(void){
  int hh = (rtc.getHourStr()-0);
  int h = hh*3600;
  int mm = (rtc.getMinStr()-0);
  int m = mm*60;
  int s = rtc.getSecStr()-0;
  int convert = h + m + s;
  return convert;
}

void logging(void){
  Serial.println("loop if jalan");
      alarm = alarm + alarmTime;
      myFile = SD.open("hasil.txt", FILE_WRITE);
      if (myFile) {    
        String waktu = "RTC ERROR";
        String tanggal = "RTC ERROR";
        if (rtc.getTimeStr() !=  '\0'){
          waktu = rtc.getTimeStr();
          tanggal = rtc.getDateStr();
        } 
        myFile.print(tanggal);
        myFile.print(", "); 
        myFile.print(waktu);
        myFile.print(", ");    
        myFile.print(h);
        myFile.print(", ");
        myFile.print(h2);
        myFile.print(", ");
        myFile.print(t);
        myFile.print(", ");
        myFile.print(t2);
        myFile.print("");
        myFile.println();
        myFile.close(); // close the file
        Serial.println("sd tulis berhasil");
      }
      // if the file didn't open, print an error:
      else {
        Serial.println("sd tulis gagal");
        u8g.firstPage();  
        do {
          drawSdFailed();
        } while( u8g.nextPage() );
      }
    buzz();
}

void loop(void) {
  Serial.println("loop started");
  hum = dht.readHumidity();
  hum2 = dht2.readHumidity();
  temp = dht.readTemperature();
  temp2 = dht2.readTemperature();

  dtostrf(hum, 3, 1, h);
  dtostrf(hum2, 3, 1, h2);
  dtostrf(temp, 3, 1, t);
  dtostrf(temp2, 3, 1, t2);

  // picture loop
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
  Serial.println(millis());
  char st1 = rtc.getTimeStr();
  char str1[8];
  strcpy(str1, st1);
  trigTime = (rtc.getTime().hour + rtc.getTime().min + rtc.getTime().sec);
  unsigned long currentMillis = millis();
  if (currentMillis >= alarm){
    logging();
  }
  // rebuild the picture after some delay
  delay(1);
}

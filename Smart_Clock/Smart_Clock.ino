/*
Arduino Smart Clock Block code using Arduino ProMini, DS3231 RTC module and Adafruit 128*64 Monochrome OLED
By Jamiu Adegoke (G.ONE)
Created: 16/6/2019
Edited: 12/11/2021
Library needed to install
Adafrui GFX Library -->   https://www.arduino.cc/reference/en/libraries/adafruit-gfx-library/
Adafruit_SSD1306 library --> https://www.arduino.cc/reference/en/libraries/ssd1306/
DS3231 RTC Library --> https://www.arduino.cc/reference/en/libraries/ds3231/

*/
#include <DS3231.h>
#define TONE_PITCH 432

#include <SoftwareSerial.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include<EEPROM.h>
#include <Pitch.h>


#define TONE_USE_INT
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels



// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
RTClib rtc;
DS3231 rtclock;

//************Button*****************//
int P1 = A0; // Button Enable/Disable Alarm' 
int P3 = 11; // Button - 
int P2 = 10; // Button +
int P4 = 9; // Button Set  Date & Time Menu
int P5 = 12; // Button Set  Alarm Menu

//************I/O Pins*****************//
int LED_ON = A1;
int LED_OFF = A2;
int buzzer = A3;
int relay = 8;

//************Variables**************//
int menu = 0;
int set_date = 0;
int ap = 0;
String tz;
int pz = 0, onhr = 0, offhr = 1, onmin = 2, offmin = 3;
uint8_t onhrval = 0, offhrval = 0, onminval = 0, offminval = 0, addval = 0, onval = 0;
String pm;
int hourupg;
int minupg;
int yearupg;
int monthupg;
int dayupg, tm;
bool alarm_State = false;
bool rtime = false;
uint8_t alarmHour1 = 0, alarmMinute1 = 0, alarmHour2 = 0, alarmMinute2 = 0;  // Holds the current alarm time
uint8_t daysOfTheWeek[7][12] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
String text;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
  pinMode(P1,INPUT_PULLUP); 
  pinMode(P2,INPUT_PULLUP);
  pinMode(P3,INPUT_PULLUP);
  pinMode(P4,INPUT_PULLUP);
  pinMode(P5,INPUT_PULLUP);

  pinMode(LED_ON,OUTPUT);
  pinMode(LED_OFF,OUTPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(relay,OUTPUT);

   digitalWrite(buzzer, LOW);
   digitalWrite(relay, LOW);
  digitalWrite(LED_ON,LOW);
  digitalWrite(LED_OFF,LOW);
}
void loop() {
  // put your main code here, to run repeatedly:
DateTime now = rtc.now();
// check if you press the SET button and increase the menu index
 if(digitalRead(P5)== LOW && menu >= 0 && set_date == 0) 
  {
    delay(120);
    menu = menu+1;
    }
  if(digitalRead(P4)== LOW && menu == 0 && set_date >= 0) 
  {
    delay(120);
   set_date = set_date+1;
    }
  if(digitalRead(P1) == LOW && menu == 0 && set_date == 0 && alarm_State == false)                                                                                                                                                                     
  {
    delay(120);
 if(digitalRead(P1) == LOW && menu == 0 && set_date == 0 && alarm_State == false)                                                                                                                                                                     
  {
    delay(120);
    for(int i = 0; i < 5; i++)
    {
    digitalWrite(LED_ON, HIGH);
    delay(150);  
    digitalWrite(LED_ON, LOW);
    delay(100);
    }
    tone(buzzer, NOTE_C0, 500);
    EEPROM.write(5,1);
    Display_Alarm_Enabled();
    delay(1000);
    alarm_State = true;
    menu = 0;
    set_date = 0;
  }
   }
  if(digitalRead(P1) == LOW && menu == 0 && set_date == 0 && alarm_State == true)                                                                                                                                                                     
  {
    delay(120);
 if(digitalRead(P1) == LOW && menu == 0 && set_date == 0 && alarm_State == true)                                                                                                                                                                     
  {
    delay(120);
    for(int i = 0; i < 5; i++)
    {
    digitalWrite(LED_OFF, HIGH);
    delay(150);  
    digitalWrite(LED_OFF, LOW);
    delay(100);
    }
    EEPROM.write(5,0);
    tone(buzzer, NOTE_D1, 500);
    Display_Alarm_Disabled();
    delay(1000);
    alarm_State = false;
    menu = 0;
    set_date = 0;
  }
   }
  if(digitalRead(P1) == LOW && set_date >= 1)                                                                                                                                                                     
  {
    delay(100);
    menu = 0;
    set_date = 0;
  }
  if(digitalRead(P1) == LOW && menu >= 1)                                                                                                                                                                     
  {
    delay(100);
    menu = 0;
    set_date = 0;
  }
if(menu == 0 && set_date == 0 )
   {
    INIT();
    delay(500);
     }
    /////Alarm Settings////
if (menu == 1)
    {
    Display_On_Alarm_hour(); //set On Alarm hour
    }
if (menu==2)
    {
    Display_On_Alarm_minute();
    }
if (menu == 3)
    {
    Display_Off_Alarm_hour(); //set On Alarm hour
    }
if (menu==4)
    {
    Display_Off_Alarm_minute();
    }
 if(menu == 5){
    //Alarm();
      menu = 0;
    set_date = 0;
    }
               
/////Time and Date Settings////
 if (set_date == 1)
    {
    DisplaySetHour();
    }
  if (set_date == 2)
    {
    DisplaySetMinute();
    }
  if (set_date == 3)
    {
    DisplaySetYear();
    }
  if (set_date == 4)
    {
    DisplaySetMonth();
    }
  if (set_date == 5)
    {
    DisplaySetDay();
    }
  if (set_date == 6)
    {
    display.clearDisplay();
  display.setTextColor(WHITE);
display.setTextSize(2);
 display.setCursor(7, 0);
  display.print(" Date & Time");
  display.setCursor(7,45);
  display.print("  Set Done");
  display.display();
       // rtclock.setClockMode(true); // set to 12h      
        rtclock.setYear(yearupg);
        rtclock.setMonth(monthupg);
        rtclock.setDate(dayupg);
        rtclock.setHour(hourupg);
        rtclock.setMinute(minupg);
   delay(700);
    menu = 0;
    set_date = 0;
    }
  addval = EEPROM.read(4);
  onval = EEPROM.read(5);
 if(addval == 1)                                                                                                                                                                     
  {
    if((now.hour() >= onhrval && now.minute() <=onminval) || (now.hour() >= onhrval && now.minute() >= onminval))
  {
   digitalWrite(relay, HIGH);
    digitalWrite(LED_ON, HIGH);
    digitalWrite(LED_OFF, LOW);
    }
  }
  else
  {
   if((now.hour() >= offhrval && now.minute() <= offminval) || (now.hour() >= offhrval && now.minute() >= offminval))
  {
    digitalWrite(relay, LOW);
    digitalWrite(LED_OFF, HIGH);
    digitalWrite(LED_ON, LOW);
  }
    }
  if(onval == 1)                                                                                                                                                                     
  {
    alarm_State = true;
  }
  else
  {
   alarm_State = false; 
    }

if(digitalRead(P3) == LOW && addval == 0 && menu == 0 && set_date == 0)                                                                                                                                                                     
  {
    delay(120);
 if(digitalRead(P3) == LOW && addval == 0 && menu == 0 && set_date == 0)                                                                                                                                                                     
  {
    delay(120);
    EEPROM.update(4,1);
  }
   }
 if(digitalRead(P2) == LOW && addval == 1 && menu == 0 && set_date == 0)                                                                                                                                                                     
  {
    delay(120);
 if(digitalRead(P2) == LOW && addval == 1 && menu == 0 && set_date == 0)                                                                                                                                                                     
  {
    delay(120);
    EEPROM.update(4,0);
  }
   }
    ////Alarm Check and Validity Operation\\\\\\
     
    if(now.hour() == onhrval && now.minute() == onminval)
   {
    rtime = true;
    EEPROM.write(4,1);
      if(alarm_State == true)
    {
    digitalWrite(buzzer,HIGH);
    delay(150);
    digitalWrite(buzzer,LOW);
    delay(100);
    }
    
    else
    {
  digitalWrite(buzzer, LOW);
      }
    digitalWrite(LED_ON,HIGH);
    digitalWrite(LED_OFF,LOW);
    digitalWrite(relay,HIGH);
    }
if(now.hour() == offhrval && now.minute() == offminval)
{
  EEPROM.write(4,0);
  rtime = false;
  digitalWrite(buzzer, LOW);
  digitalWrite(LED_ON,LOW);
  digitalWrite(relay,LOW);
  digitalWrite(LED_OFF,HIGH);
  }

}

void INIT()
{
 Display_Date_and_Time();
}
void Display_Date_and_Time()
{
// We show the current date and time
  DateTime now = rtc.now();
  ap = now.hour();
if(ap < 12)
{
tz = " A";
}
else
{
tz =" P";
}
//*******Operations on OLED*******\\
   Serial.println("Current Date & Time: ");
   display.clearDisplay();
   display.setTextColor(WHITE);
display.setTextSize(1);
 display.setCursor(0, 0);
 display.print("T");
 display.setCursor(0, 20);
display.print("D");
  display.setCursor(0, 38);
  display.setTextSize(1);
 display.print("  ON @:");
 display.print("      OFF @:");
 
 display.setCursor(0, 48);
  display.setTextSize(2);
  onhrval = EEPROM.read(onhr);
  if (onhrval <= 9)
  {
   display.print("0"); 
    }
 display.print(onhrval);
 display.print(":");
 onminval = EEPROM.read(onmin);
 if (onminval <= 9)
  {
   display.print("0"); 
    }
 display.print(onminval);
///////////////////////////////
display.setTextSize(1);
display.print("|");
display.setTextSize(2);
offhrval = EEPROM.read(offhr);
 if (offhrval <= 9)
  {
   display.print("0"); 
    }
 display.print(offhrval);
 display.print(":");
 offminval = EEPROM.read(offmin);
 if (offminval <= 9)
  {
   display.print("0"); 
    }
 display.print(offminval);
 
   display.setTextSize(2);
   display.setCursor(8, 20);
   display.print(now.day(), DEC);
    Serial.print(now.day(), DEC);
    if(now.day() == 3 || now.day() == 23)
    {
      display.setTextSize(1);
     display.print("rd"); 
     Serial.print("rd ");
      }
    else if(now.day() == 1 || now.day() == 21  || now.day() == 31)
    {
      display.setTextSize(1);
     display.print("st");
     Serial.print("st "); 
      }
      else if(now.day() == 2 || now.day() == 22)
    {
      display.setTextSize(1);
    display.print("nd");
     Serial.print("nd "); 
      }
      else
      {
        display.setTextSize(1);
        display.print("th");
        Serial.print("th "); 
        }
    //month_set();
    display.setTextSize(2);
    display.print(now.month(), DEC);
    Serial.print(now.month());
    //display.print(pm);
    //Serial.print(pm);
    display.print(",");
    Serial.print(", ");
    Serial.print(now.year(), DEC);
    display.print(now.year(), DEC);
   if(now.hour() == 13){
    tm = 1;
      }
   else if(now.hour() == 14){
    tm = 2;
      }
    else if(now.hour() == 15){
    tm = 3;
      }
    else if(now.hour() == 16){
    tm = 4;
      }
    else if(now.hour() == 17){
    tm = 5;
      }
    else if(now.hour() == 18){
    tm = 6;
      }
   else if(now.hour() == 19){
    tm = 7;
      }
   else if(now.hour() == 20){
    tm = 8;
      }
    else if(now.hour() == 21){
    tm = 9;
      }
    else if(now.hour() == 22){
    tm = 10;
      }
   else if(now.hour() == 23){
    tm = 11;
      }
  else if(now.hour() == 0){
    tm = 12;
      }
      else{
    tm = now.hour();
      }
      Serial.print("----");
   display.setTextSize(2);
   display.setCursor(8, 0);
   if(now.hour() <= 9 && now.hour() >= 1){
      Serial.print("0");
     display.print(0, DEC);
      }
    Serial.print(tm, DEC);
    display.print(tm, DEC);
    Serial.print(':');
    display.print(':');
    if(now.minute() <= 9){
      Serial.print("0");
     display.print("0");
      }
    Serial.print(now.minute(), DEC);
    display.print(now.minute(), DEC);
    Serial.print(':');
    display.print(':');
    if(now.second() <= 9){
     display.print("0");
      Serial.print("0");
      }
    Serial.print(now.second(), DEC);
    display.print(now.second(), DEC);
    Serial.print(tz);
    display.print(tz);
    Serial.println();
    display.display();
}
void DisplaySetHour()
{
// time setting
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  DateTime now = rtc.now();
  if(digitalRead(P2) == LOW)
  {
    if(hourupg==23)
    {
      hourupg=0;
    }
    else
    {
      hourupg=hourupg+1;
    }
  }
   if(digitalRead(P3) == LOW)
  {
    if(hourupg==0)
    {
      hourupg=23;
    }
    else
    {
      hourupg=hourupg-1;
    }
  }
  display.setCursor(5,0);
  display.print(" Set Hour:");
  display.setTextSize(4);
  display.setCursor(40,35);
  display.print(hourupg,DEC);
  display.display();
  delay(100);
}
void DisplaySetMinute()
{
// Setting the minutes
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  if(digitalRead(P2) == LOW)
  {
    if (minupg==59)
    {
      minupg=0;
    }
    else
    {
      minupg=minupg+1;
    }
  }
   if(digitalRead(P3) == LOW)
  {
    if (minupg==0)
    {
      minupg=59;
    }
    else
    {
      minupg=minupg-1;
    }
  }
  display.setCursor(0,0);
  display.print("Set Minute");
  display.setTextSize(4);
  display.setCursor(40,35);
  display.print(minupg,DEC);
  display.display();
  delay(100);
}
void DisplaySetYear()
{
// setting the year
 display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  if(digitalRead(P2) == LOW)
  {    
    yearupg=yearupg+1;
  }
   if(digitalRead(P3) == LOW)
  {
    yearupg=yearupg-1;
  }
  display.setCursor(3,0);
  display.print("Set Year:");
  display.setTextSize(4);
  display.setCursor(40,35);
  display.print(yearupg,DEC);
  display.display();
  delay(100);
}
void DisplaySetMonth()
{
// Setting the month
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  if(digitalRead(P2) == LOW)
  {
    if (monthupg==12)
    {
      monthupg=1;
    }
    else
    {
      monthupg=monthupg+1;
    }
  }
   if(digitalRead(P3) == LOW)
  {
    if (monthupg==1)
    {
      monthupg=12;
    }
    else
    {
      monthupg=monthupg-1;
    }
  }
  display.setCursor(3,0);
  display.print("Set Month:");
  display.setTextSize(4);
  display.setCursor(40,35);
  display.print(monthupg,DEC);
  display.display();
  delay(100);
}
void DisplaySetDay()
{
// Setting the day
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  if(digitalRead(P2) == LOW)
  {
    if (dayupg==31)
    {
      dayupg=1;
    }
    else
    {
      dayupg=dayupg+1;
    }
  }
   if(digitalRead(P3) == LOW)
  {
    if (dayupg==1)
    {
      dayupg=31;
    }
    else
    {
      dayupg=dayupg-1;
    }
  }
  display.setCursor(3,0);
  display.print("Set Day:");
  display.setTextSize(3);
  display.setCursor(40,35);
  display.print(dayupg,DEC);
  display.display();
  delay(100);
  }

  void Display_Alarm_Enabled()
{
  display.clearDisplay();
  display.setTextColor(WHITE);
display.setTextSize(2);
 display.setCursor(7, 0);
  display.print(" Alarm ");
  display.setCursor(7,45);
  display.print(" Enabled");
  display.display();
  }
  void Display_Alarm_Disabled()
{
  display.clearDisplay();
  display.setTextColor(WHITE);
display.setTextSize(2);
 display.setCursor(7, 0);
  display.print(" Alarm ");
  display.setCursor(7,45);
  display.print(" Disabled");
  display.display();
  }
void Display_On_Alarm_hour() //Setting On Alarm hour
{
  alarmHour1 = EEPROM.read(onhr);

  if(digitalRead(P2)== LOW)
  {
    if(alarmHour1==23)
    {
      alarmHour1=0;
    }
    else
    {
      alarmHour1=alarmHour1+1;
    }
  }
   if(digitalRead(P3)== LOW)
  {
    if(alarmHour1==0)
    {
      alarmHour1=23;
    }
    else
    {
      alarmHour1=alarmHour1-1;
    }
  }
  EEPROM.update(onhr, alarmHour1);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(3,0);
  display.print("ON HOUR:");
  display.setTextSize(4);
  display.setCursor(40,35);
  display.print(alarmHour1,DEC);
  display.display();
  delay(100);
    }
void Display_On_Alarm_minute()//Setting Alarm minute
{
alarmMinute1 = EEPROM.read(onmin);

  if(digitalRead(P2)== LOW)
  {
    if (alarmMinute1==59)
    {
      alarmMinute1=0;
    }
    else
    {
      alarmMinute1 = alarmMinute1+1;
    }
  }
   if(digitalRead(P3)== LOW)
  {
    if (alarmMinute1==0)
    {
      alarmMinute1=59;
    }
    else
    {
      alarmMinute1=alarmMinute1-1;
    }
  }
  EEPROM.update(onmin, alarmMinute1);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(3,0);
  display.print("ON MINUTE:");
  display.setTextSize(4);
  display.setCursor(40,35);
  display.print(alarmMinute1,DEC);
  display.display();
  delay(100);
    }
void Display_Off_Alarm_hour() //Setting On Alarm hour
{
 alarmHour2 = EEPROM.read(offhr);
    
  if(digitalRead(P2)== LOW)
  {
    if(alarmHour2==23)
    {
      alarmHour2=0;
    }
    else
    {
      alarmHour2=alarmHour2+1;
    }
  }
   if(digitalRead(P3)== LOW)
  {
    if(alarmHour2==0)
    {
      alarmHour2=23;
    }
    else
    {
      alarmHour2=alarmHour2-1;
    }
  }
  EEPROM.update(offhr, alarmHour2);
 display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(3,0);
  display.print("OFF HOUR:");
  display.setTextSize(4);
  display.setCursor(40,35);
  display.print(alarmHour2,DEC);
  display.display();
  delay(100);
   }
void Display_Off_Alarm_minute()//Setting Alarm minute
{
  alarmMinute2 = EEPROM.read(offmin);

  if(digitalRead(P2)== LOW)
  {
    if (alarmMinute2==59)
    {
      alarmMinute2=0;
    }
    else
    {
      alarmMinute2 = alarmMinute2+1;
    }
  }
   if(digitalRead(P3)== LOW)
  {
    if (alarmMinute2 == 0)
    {
      alarmMinute2 = 59;
    }
    else
    {
      alarmMinute2 = alarmMinute2-1;
    }
  }
  EEPROM.update(offmin, alarmMinute2);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(3,0);
  display.print("OFF MINUTE");
  display.setTextSize(4);
  display.setCursor(40,35);
  display.print(alarmMinute2,DEC);
  display.display();
  delay(100);
    }
  void Alarm()
  {
   display.clearDisplay();
  display.setTextColor(WHITE);
display.setTextSize(2);
 display.setCursor(3, 0);
  display.print("Alarm Time");
  display.setCursor(7,45);
  display.print(" Set Done");
  display.display();
      delay(1000);
      menu = 0;
    set_date = 0; 
    }

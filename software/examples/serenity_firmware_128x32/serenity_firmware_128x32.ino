#include <OLED_I2C.h>
#include <MyRealTimeClock.h>
#include <Adafruit_NeoPixel.h>
#include <IRremote.h>
#include <TimerFreeTone.h>
#include "start.h"

#define LED_PIN       9
#define LED_COUNT     1
#define TONE_PIN      6
  
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
MyRealTimeClock myRTC(2,4,5);
OLED  myOLED(SDA, SCL);
IRsend irsend;

extern uint8_t TinyFont[], SmallFont[], MediumNumbers[], BigNumbers[], serIOT[];
extern uint8_t sunIC[], irIC[], clockIC[], bulbOnIC[], bulbOffIC[];


int hr, hrA = 12, mnA = 0, al,mn, tsc =0,sc,dy,mt,yr,wkd;
int a6,a3;
String tm, dt;
String days[] = {"Sun","Mon","Tue","Wed","Thur","Fri","Sat"};
String months[] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
int bt = 0, viewX = -1, viewY = 0, del = 0, hz = 0;
int br = 100, rd = 0, gn = 0, bl = 0, irsc = 0, scl = 0;
float lx = 0.00, adc = 0.0048828125, ldr;
bool b1,b2,b3, alON;

void setup() {
  pinMode(A0, OUTPUT);
  digitalWrite(A0, LOW);
  strip.begin(); 
  strip.setPixelColor(0, strip.Color(0,0,0)); 
  if(!myOLED.begin(SSD1306_128X32)){
    while(1){
      strip.setPixelColor(0, strip.Color(200,0,0)); 
      strip.show();
    }
  }
  
  strip.show();
  
  myOLED.clrScr();
  myOLED.drawBitmap(0, 10, serIOT, 128,32);
  myOLED.update();
  play_tune(oo7);
  myOLED.clrScr();
  myOLED.setFont(SmallFont);
  for (int j = 10; j > -3 ; j--){
    myOLED.drawBitmap(0, j, serIOT, 128,32);
    myOLED.update();
  }
  myOLED.print("biego.tech",RIGHT, 19);
  for (int i = 0; i < 128 ; i++){
    myOLED.drawRectFill(0, 30, i, 31);
    myOLED.update();
  }
  
  
  delay(100);
}

void loop() {
  
  serTime();
  a3 = analogRead(A3);
  a6 = analogRead(A6);
  ldr = map(a3,0,1023,1023,0);
  hz = map(a6,0,1023,0,24);
  keys(analogRead(A2));
  lx = (250.000000/(adc*ldr))-50.000000;
  switch (viewX) {
    case -3:
      myOLED.clrScr();
      myOLED.setFont(SmallFont);
      myOLED.drawBitmap(0, -2, serIOT, 128,32);
      myOLED.print("biego.tech",RIGHT, 21);
      myOLED.update();
      break;
    case -2:
      myOLED.setFont(SmallFont);
      myOLED.clrScr();
      if ( viewY == 1){
        hrA = map(a6,0,1023,23, 0);
      } else if ( viewY == 2){
        mnA = map(a6,0,1023,59, 0);
      } else if ( viewY == 3){
        al = map(a6,0,1023,1,0);
        if (al >= 1){
          alON = true;
        } else {
          alON = false;
        }
      } 
      myOLED.print("ALARM CLOCK", CENTER, 0);
      myOLED.drawBitmap(0, 15, clockIC, 16,16);
      myOLED.setFont(MediumNumbers);
      myOLED.invertText((viewY == 1 ? true : false));
      myOLED.print((hrA<10 ? "0" : "")+String(hrA), 30, 16);
      myOLED.invertText(false);
      myOLED.print(":",55,16);
      myOLED.invertText((viewY == 2 ? true : false));
      myOLED.print((mnA<10 ? "0" : "")+String(mnA), 70, 16);
      myOLED.setFont(SmallFont);
      myOLED.invertText((viewY == 3 ? true : false));
      myOLED.print((alON ? " ON " : " OFF "),100,20);
      myOLED.invertText(false);
      myOLED.update();
      break;
    case -1:
      myOLED.clrScr();
      myOLED.setFont(MediumNumbers);
      myOLED.print(tm, CENTER, 0);
      myOLED.setFont(SmallFont);
      myOLED.print(dt, CENTER, 19);
      if (alON){
        myOLED.drawBitmap(0, 15, clockIC, 16,16);
      }
      myOLED.update();
      break;
    case 0:
      myOLED.setFont(SmallFont);
      myOLED.clrScr();
      myOLED.print(dt, 50, 11);
      myOLED.print(tm, 50, 0);
      myOLED.drawLine(0,21,128,21);
      myOLED.drawLine(45,0,45,32);
      myOLED.print((hrA<10 ? "0" : "")+String(hrA)+":"+(mnA<10 ? "0" : "")+String(mnA)+(alON ? "   ON " : "   OFF "), 50, 24);
      myOLED.print(String(bat(), 2)+"v",  LEFT, 11);
      myOLED.print(per(bat()), 22, 0);
      myOLED.drawRect(1,1,20,6);
      myOLED.drawRectFill(2,2,int(map(bt,0,100,2,19)),5);
      myOLED.print(String(lx,0)+" lx", LEFT, 24);
      myOLED.update();
      break;
    case 1:
      myOLED.clrScr();
      if ( viewY == 1){
        br = map(analogRead(A6),0,1023,255, 0);
      } else if ( viewY == 2){
        rd = map(analogRead(A6),0,1023,255, 0);
      } else if ( viewY == 3){
        gn = map(analogRead(A6),0,1023,255, 0);
      } else if ( viewY == 4){
        bl = map(analogRead(A6),0,1023,255, 0);
      }
      myOLED.setFont(SmallFont);
      myOLED.drawBitmap(5, 10, (br == 0 ? bulbOffIC : bulbOnIC), 16,16);
      myOLED.print("RGB LED CONTROL", CENTER, 0);
      myOLED.setFont(TinyFont);
      myOLED.invertText((viewY == 1 ? true : false));
      myOLED.print(" BRIGHT ", 28, 11);
      myOLED.invertText((viewY == 2 ? true : false));
      myOLED.print(" RED ", 58, 11);
      myOLED.invertText((viewY == 3 ? true : false));
      myOLED.print(" GREEN ", 78, 11);
      myOLED.invertText((viewY == 4 ? true : false));
      myOLED.print(" BLUE ", 109, 11);
      
      myOLED.invertText(false);
      myOLED.printNumI(br, 30, 20);
      myOLED.printNumI(rd, 60, 20);
      myOLED.printNumI(gn, 80, 20);
      myOLED.printNumI(bl, 110, 20);
      
      myOLED.update();
      strip.setBrightness(br);
      strip.setPixelColor(0, strip.Color(rd,gn,bl));
      strip.show();
      break;
    case 2:
      irsc = map(a6,0,1023,0,6);
      scl = map(a6,0,1023,0,55)-3;
      myOLED.setFont(SmallFont);
      myOLED.clrScr();
      myOLED.drawBitmap(5, 10, irIC, 16,16);
      myOLED.invertText((irsc == 0 ? true : false));
      myOLED.print(" Lights ON ", 30, 11-scl);
      myOLED.invertText((irsc == 1 ? true : false));
      myOLED.print(" Lights OFF ", 30, 22-scl);
      myOLED.invertText((irsc == 2 ? true : false));
      myOLED.print(" TV Power ", 30, 33-scl);
      myOLED.invertText((irsc == 3 ? true : false));
      myOLED.print(" TV Mute ", 30, 44-scl);
      myOLED.invertText((irsc == 4 ? true : false));
      myOLED.print(" TV Vol+ ", 30, 55-scl);
      myOLED.invertText((irsc == 5 ? true : false));
      myOLED.print(" TV Vol- ", 30, 66-scl);
      myOLED.invertText((irsc == 6 ? true : false));
      myOLED.print(" TV Pr.Ch ", 30, 77-scl);
      myOLED.invertText(false);
      myOLED.drawLine(127,0+map(scl,0,55,0,26),127,5+map(scl,0,55,0,26));
      myOLED.print("               ", CENTER, 4);
      myOLED.print("   IR REMOTE   ", CENTER, 0);
      myOLED.setFont(SmallFont);
      myOLED.update();
      break;
     case 3:
      myOLED.setFont(SmallFont);
      
      myOLED.clrScr();
      myOLED.drawBitmap(5, 10, sunIC, 16,16);
      myOLED.print("LIGHT LEVEL", CENTER, 0);
      myOLED.print(String(lx,0)+" lx", 30, 22);
      myOLED.drawRect(30,12,120,18);
      myOLED.drawRectFill(31,13,int(map(ldr,1023, 0,32,119)),17);
      myOLED.update();
      break;
  }
  if (alTrue() && alON){
    if (tsc != sc ){
      tsc = sc;
      TimerFreeTone(TONE_PIN,1600,100);
      delay(50);
      TimerFreeTone(TONE_PIN,1600,100);
    }
  }
  

}


void serTime(){
  myRTC.updateTime();
  hr = myRTC.hours;
  mn = myRTC.minutes;
  sc = myRTC.seconds;
  dy = myRTC.dayofmonth;
  mt = myRTC.month;
  yr = myRTC.year;
  wkd = myRTC.dayofweek;
  tm = (hr<10 ? "0" : "")+String(hr)+":" +(mn<10 ? "0" : "")+ mn + ":"+(sc<10 ? "0" : "")+sc;
  //dt = (dy<10 ? "0" : "")+String(dy)+"/"+(mt<10 ? "0" : "")+mt+"/"+yr;
  dt = days[wkd-1]+ " "+String(dy<10 ? "0" : "")+String(dy)+" "+months[mt-1];

}

float bat(){
  analogReference(INTERNAL);
  delay(10);
  burn8Readings();
  float vol = (map(analogRead(A7),0,1023,0,680)) * 0.01;
  analogReference(DEFAULT);
  delay(10);
  burn8Readings();
  return vol;
}

String per(float vl){
  if (vl > 4.3){
    bt = 100;
    return "USB";
  } else if (vl > 3.4){
    vl = vl * 100;
    bt = map(vl, 340,430,0,100);
    return String(bt)+"%";
  } else {
    bt = 0;
    return "LOW";
  }
}

void burn8Readings(){
  for (int i = 0; i<8;i++){
    analogRead(A0);
    delay(1);
  }
}


void keys(int a2){
  if (Bt1(a2)){
    if (!b1){
      if (viewX > -3){
        viewX--;
      } else {
        viewX = 3;
      }
      viewY = 0;
      b1 = true;
    }
  } else {
    b1 = false;
  }
  if (Bt2(a2)){
    if (!b2){
      if (alTrue() && alON){
        alON = false;
        if ( viewX == -3){
          viewX = -1;
        }
      } else if (viewX == 1){
        viewY++;
        if (viewY > 4){
          viewY = 0;
        }
      } else if (viewX == 2){
        irSend(irsc);        
      } else if (viewX == -2){
        viewY++;
        if (viewY > 3){
          viewY = 0;
        }
      } 
      b2 = true;
    }
  } else {
    b2 = false;
  }

  if (Bt3(a2)){
    if (!b3){
      if (viewX < 3){
        viewX++;
      } else {
        viewX = -3;
      }
      viewY = 0;
      b3 = true;
    }
  } else {
    b3 = false;
  }
  
}

bool Bt1(int x){
  if(x>500 && x<520 || x>740 && x<765 || x>865&&x<885 || x>900&&x<915){
    return true;
  } else {
    return false;
  }
}

bool Bt2(int x){
  if(x>645 && x<665 || x>740 && x<765 || x>885&&x<915){
    return true;
  } else {
    return false;
  }
}

bool Bt3(int x){
  if(x>840 && x<915){
    return true;
  } else {
    return false;
  }
}

void irSend(int val){

  switch (val){
    case 0:
      for (int i = 0; i < 3; i++) {
        irsend.sendNEC(0xDB24E817, 32);
        delay(40);
      }
      break;
     case 1:
      for (int i = 0; i < 3; i++) {
        irsend.sendNEC(0xDB24F00F, 32);
        delay(40);
      }
      break;
    case 2:
      for (int i = 0; i < 3; i++) {
        irsend.sendSony(0xa90, 12);
        delay(40);
      }
      break;
    case 3:
      for (int i = 0; i < 3; i++) {
        irsend.sendSony(0x290, 12);
        delay(40);
      }
      break;
    case 4:
      for (int i = 0; i < 3; i++) {
        irsend.sendSony(0x490, 12);
        delay(40);
      }
      break;
    case 5:
      for (int i = 0; i < 3; i++) {
        irsend.sendSony(0xc90, 12);
        delay(40);
      }
      break;
    case 6:
      for (int i = 0; i < 3; i++) {
        irsend.sendSony(0xdd0, 12);
        delay(40);
      }
      break;
  }
}

bool alTrue(){
  if ( hrA == hr && mnA == mn){
    return true;
  } else {
    return false;
  }
}

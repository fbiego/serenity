#include <Adafruit_NeoPixel.h>
#define PIN            9
#define NUMPIXELS      1
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define REMOTEXY_MODE__HARDSERIAL

#include <RemoteXY.h>
#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

// RemoteXY connection settings 
#define REMOTEXY_SERIAL Serial
#define REMOTEXY_SERIAL_SPEED 9600


// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =
  { 255,4,0,34,0,24,1,8,130,0,
  130,1,43,47,32,10,131,130,1,4,
  34,34,23,131,130,1,79,12,18,45,
  131,130,1,43,12,32,33,131,6,0,
  47,20,22,22,2,26,65,4,48,51,
  5,5,65,4,57,51,5,5,65,4,
  66,51,5,5,65,7,65,13,9,9,
  66,130,81,27,14,11,93,25,66,130,
  81,42,14,11,94,25,66,129,9,39,
  27,4,135,24,66,129,9,45,27,4,
  190,24,66,129,9,51,27,4,36,24,
  130,1,4,12,34,19,131,129,0,6,
  40,2,3,30,88,0,129,0,5,45,
  2,3,30,89,0,129,0,6,52,2,
  3,30,90,0,129,0,13,35,8,3,
  30,77,80,85,54,48,53,48,0,129,
  0,85,33,6,3,30,76,68,82,0,
  129,0,85,49,5,3,30,80,111,116,
  0,66,129,7,19,28,4,12,24,129,
  0,11,15,10,3,31,66,97,116,116,
  101,114,121,0,129,0,47,15,8,4,
  30,76,69,68,0,129,0,6,3,18,
  6,31,83,69,82,69,78,73,84,89,
  0,129,0,79,58,7,4,31,70,76,
  88,0,67,1,7,25,11,4,31,26,
  11,67,1,20,25,12,4,31,26,11,
  129,0,45,48,11,3,31,66,117,116,
  116,111,110,115,0,1,0,83,15,9,
  9,36,31,76,49,51,0 };
  
// this structure defines all the variables of your control interface 
struct {

    // input variable
  uint8_t ws_btn_r; // =0..255 Red color value 
  uint8_t ws_btn_g; // =0..255 Green color value 
  uint8_t ws_btn_b; // =0..255 Blue color value 
  uint8_t l_13; // =1 if button pressed, else =0 

    // output variable
  uint8_t b1_r; // =0..255 LED Red brightness 
  uint8_t b2_r; // =0..255 LED Red brightness 
  uint8_t b3_r; // =0..255 LED Red brightness 
  uint8_t ws_led_r; // =0..255 LED Red brightness 
  uint8_t ws_led_g; // =0..255 LED Green brightness 
  uint8_t ws_led_b; // =0..255 LED Blue brightness 
  int8_t ldr; // =0..100 level position 
  int8_t pot; // =0..100 level position 
  int8_t mpu_x; // =0..100 level position 
  int8_t mpu_y; // =0..100 level position 
  int8_t mpu_z; // =0..100 level position 
  int8_t bat_lvl; // =0..100 level position 
  char bat_vol[11];  // string UTF8 end zero 
  char bat_per[11];  // string UTF8 end zero 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

#define PIN_L_13 13


void setup() 
{
  RemoteXY_Init (); 
  pixels.begin();
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
  pinMode (PIN_L_13, OUTPUT);
  
  // TODO you setup code
  
}

void loop() 
{ 
  analogReference(DEFAULT);
  delay(10);
  burn8Readings();
  RemoteXY_Handler ();
  Vector normAccel = mpu.readNormalizeAccel();
  RemoteXY.mpu_x = map(normAccel.XAxis,-20,20,0,100);
  RemoteXY.mpu_y = map(normAccel.YAxis,-20,20,0,100);
  RemoteXY.mpu_z = map(normAccel.ZAxis,-20,20,0,100);
  digitalWrite(PIN_L_13, RemoteXY.l_13);

  RemoteXY.ldr = map(analogRead(A3),0,1023,0,100);
  RemoteXY.pot = map(analogRead(A6),0,1023,100,0);
  RemoteXY.b1_r = Bt1(analogRead(A2));
  RemoteXY.b2_r = Bt2(analogRead(A2));
  RemoteXY.b3_r = Bt3(analogRead(A2));

  RemoteXY.ws_led_r = RemoteXY.ws_btn_r;
  RemoteXY.ws_led_g = RemoteXY.ws_btn_g;
  RemoteXY.ws_led_b = RemoteXY.ws_btn_b;
  for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(RemoteXY.ws_btn_r,RemoteXY.ws_btn_g,RemoteXY.ws_btn_b));
    pixels.show();
    delay(2);
  }

  analogReference(INTERNAL);
  delay(10);
  burn8Readings();
  float vol = map(analogRead(A7),0,1023,0,640);
  float vol2 = vol*0.01;
  float per = map(vol,350,420,0,100);
  String vl = String(vol2,2)+"V";
  String pc = String(per,0)+"%";
  RemoteXY.bat_lvl = int(per);
  vl.toCharArray(RemoteXY.bat_vol,10);
  pc.toCharArray(RemoteXY.bat_per,10);
}

int Bt1(int x){
  if(x>500 && x<520 || x>740 && x<765 || x>865&&x<885 || x>900&&x<915){
    return 255;
  } else {
    return 0;
  }
}

int Bt2(int x){
  if(x>645 && x<665 || x>740 && x<765 || x>885&&x<915){
    return 255;
  } else {
    return 0;
  }
}

int Bt3(int x){
  if(x>840 && x<915){
    return 255;
  } else {
    return 0;
  }
}

void burn8Readings(){
  for (int i = 0; i<8;i++){
    analogRead(A0);
    delay(1);
  }
}

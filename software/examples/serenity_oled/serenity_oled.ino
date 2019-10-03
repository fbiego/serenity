#include <OLED_I2C.h>

OLED  myOLED(SDA, SCL);

extern uint8_t SmallFont[];

void setup() {
  if(!myOLED.begin(SSD1306_128X64))
    while(1);   // In case the library failed to allocate enough RAM for the display buffer...
    
  myOLED.setFont(SmallFont);

}

void loop() {
  // put your main code here, to run repeatedly:
  int y = map(analogRead(A6),0,1023,26,255);
  myOLED.clrScr();
  myOLED.setBrightness(y);
  myOLED.print("SERENITY TEST", CENTER, 0);
  myOLED.print("LDR", LEFT, 20);
  //myOLED.print("POT", LEFT, 40);
  int x = map(analogRead(A3),0,1023,26,125);
  
  myOLED.drawRectFill(25,20,x,26);
  //myOLED.drawRectFill(25,40,y,46);
  myOLED.update();
  delay(300);

}


#include <IRremote.h>

IRsend irsend;
int ir = 0;

void setup()
{
  Serial.begin(9600);
}

void loop() {
  int a2 = analogRead(A2);
  //delay(10);
  if (Bt1(a2)){
    if(ir == 0){
      for (int i = 0; i < 3; i++) {
        //irsend.sendNEC(0xDB24E817, 32);
        irsend.sendSony(0x290, 12);
        delay(40);
        Serial.println("ir_sent");
      }
      ir++;
    }
  }
  if (Bt2(a2)){
    if(ir == 0){
      for (int i = 0; i < 3; i++) {
        //irsend.sendNEC(0xDB24F00F, 32);
        irsend.sendSony(0x490, 12);
        delay(40);
      }
      ir++;
    }
  }

  if (Bt3(a2)){
    if(ir == 0){
      for (int i = 0; i < 3; i++) {
        //irsend.sendNEC(0xDB2448B7, 32);
        irsend.sendSony(0xC90, 12);
        delay(40);
      }
      ir++;
    }
  }

  if (a2 < 100){
    ir = 0;
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

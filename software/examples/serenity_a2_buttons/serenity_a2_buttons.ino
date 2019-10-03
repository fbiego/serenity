void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
//  Serial.print(analogRead(A0));
//  Serial.print("\t");
//  Serial.print(analogRead(A1));
//  Serial.print("\t");
//  Serial.print(analogRead(A2));
//  Serial.print("\t");
//  Serial.print(analogRead(A3));
//  Serial.print("\t");
  int a2 = analogRead(A2);
  delay(10);
  if (Bt1(a2)){
    Serial.print("B1");
  } else {
    Serial.print("__");
  }
  if (Bt2(a2)){
    Serial.print("B2");
  } else {
    Serial.print("__");
  }

  if (Bt3(a2)){
    Serial.println("B3");
  } else {
    Serial.println("__");
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

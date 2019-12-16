#define AN    220     // 440 Hz
#define AS    233     // 466 Hz
#define BN    247     // 493 Hz
#define CN    261     // 523 Hz
#define CS    277     // 554 Hz
#define DN    294     // 588 Hz
#define DS    311     // 622 Hz
#define EN    330     // 658 Hz
#define FN    349     // 698 Hz
#define FS    370     // 740 Hz
#define GN    392     // 784 Hz
#define GS    415     // 830 Hz
// defines for the duration of the notes (in ms)
#define wh    1024
#define h1     100
#define dq     448
#define q      256
#define qt     170
#define de     192
#define e      128
#define et      85
#define oo7      1    // 007 jingle
#define PIEZO_PIN 6

/////////////////////////////////////////////////////////////
void ToneOut(int pitch, int duration){  // pitch in Hz, duration in ms
 int delayPeriod;
 long cycles, i;

pinMode(PIEZO_PIN, OUTPUT);           // turn on output pin
 delayPeriod = (500000 / pitch) - 7;   // calc 1/2 period in us -7 for overhead
 cycles = ((long)pitch * (long)duration) / 1000; // calc. number of cycles for loop

 for (i=0; i<= cycles; i++){           // play note for duration ms
   digitalWrite(PIEZO_PIN, HIGH);
   delayMicroseconds(delayPeriod);
   digitalWrite(PIEZO_PIN, LOW);
   delayMicroseconds(delayPeriod - 1); // - 1 to make up for digitaWrite overhead
 }
 pinMode(PIEZO_PIN, INPUT);            // shut off pin to avoid noise from other operations
}

void play_tune(int tune){               // play a tune . . .
 switch (tune) {                       // a case for each tune
 case oo7:                             // 007  E, F#, G (mult by 2 for next higher octave)
   ToneOut(EN*2,qt);                    
   ToneOut(FS*2,qt);
   ToneOut(GN*2,qt);
   delay(h1);
   break;
  case 12:
   ToneOut(EN*2,qt);  
   delay(h1);
   break;
   case 22:
   ToneOut(FS*2,qt);  
   delay(h1);
   break;
   case 32:
   ToneOut(GN*2,qt);  
   delay(h1);
   break;
 }
}

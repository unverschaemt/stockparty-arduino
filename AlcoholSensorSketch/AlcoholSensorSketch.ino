#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int startTime;
int timeInterval = 5000;
boolean newValue = false;

int speakerPin = 12;
int buttonPin = 13;
int sensorPin = A1;

//LED BAR 
int latchPin = 2;
int clockPin = 12;
int dataPin = 11;

byte data_reg1[17];
byte data_reg2[5];

byte reg1;
byte reg2;

void setup()
{
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Bereit");

  Serial.begin(9600);

  pinMode(buttonPin, INPUT);
  pinMode(speakerPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(sensorPin, INPUT);

  // variable  | hexa  | led status | binary
  data_reg1[0] = 0x00; // 00000000  | 00000000
  data_reg1[1] = 0x01; // 10000000  | 00000001
  data_reg1[2] = 0x02; // 01000000  | 00000010
  data_reg1[3] = 0x04; // 00100000  | 00000100
  data_reg1[4] = 0x08; // 00010000  | 00001000
  data_reg1[5] = 0x10; // 00001000  | 00010000
  data_reg1[6] = 0x20; // 00000100  | 00100000
  data_reg1[7] = 0x40; // 00000010  | 01000000
  data_reg1[8] = 0x80; // 00000001  | 10000000
  data_reg1[9] = 0x01; // 10000000  | 00000001
  data_reg1[10]= 0x03; // 11000000  | 00000011
  data_reg1[11]= 0x07; // 11100000  | 00000111
  data_reg1[12]= 0x0F; // 11110000  | 00001111
  data_reg1[13]= 0x1F; // 11111000  | 00011111
  data_reg1[14]= 0x3F; // 11111100  | 00111111
  data_reg1[15]= 0x7F; // 11111110  | 01111111
  data_reg1[16]= 0xFF; // 11111111  | 11111111

  data_reg2[0] = 0x00; // 00        | 00000000
  data_reg2[1] = 0x01; // 10        | 00000001
  data_reg2[2] = 0x02; // 01        | 00000010
  data_reg2[3] = 0x01; // 10        | 00000001
  data_reg2[4] = 0x03; // 11        | 00000011 

}

void loop() {
  //First all led's off
  reg1 = data_reg1[0];
  reg2 = data_reg2[0];
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, reg2);
  shiftOut(dataPin, clockPin, reg1);
  digitalWrite(latchPin, 1);
  delay(1);

  if(digitalRead(buttonPin) == HIGH) {
    startTime = millis();
    lcd.clear();
    lcd.print("Jetzt 5s lang");
    lcd.setCursor(0,1);
    lcd.print(" pusten");
    newValue = true;
  }
  if(millis() > startTime+timeInterval) {
    if(newValue) {
      beep();

      lcd.setCursor(0,0);
      lcd.clear();
      lcd.print("Warten...");

      int sensorValue = analogRead(sensorPin);
      Serial.println(sensorValue);
      colorMeterWithValue(sensorValue);
      delay(timeInterval);

      lcd.setCursor(0,0);
      lcd.clear();
      lcd.print("Bereit");
      newValue = false;
    }
  }

  delay(100);
}

void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {

  int i=0;
  int pinState;
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);

  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);

  for (i=7; i>=0; i--)  {
    digitalWrite(myClockPin, 0);
    if ( myDataOut & (1<<i) ) {
      pinState= 1;
    }         
    else { 
      pinState= 0;
    }
    digitalWrite(myDataPin, pinState);
    digitalWrite(myClockPin, 1);
    digitalWrite(myDataPin, 0);
  }
  digitalWrite(myClockPin, 0);
} 


void beep() {
  for (int i = 0; i < 3; i++) {
    tone(speakerPin, 261);
    delay(400); 
  }
  noTone(speakerPin); 
}

void colorMeterWithValue(int value) {
  //should light up all leds one by one
  int pause = 300; //ms

  reg2 = data_reg2[0];
  for(int start=9; start<=16; start++) {
    reg1 = data_reg1[start];
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, reg2);
    shiftOut(dataPin, clockPin, reg1);
    digitalWrite(latchPin, 1);
    delay(pause);
  }

  reg1 = data_reg1[16];
  for(int start=3; start<=4; start++) {
    reg2 = data_reg2[start];
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, reg2);
    shiftOut(dataPin, clockPin, reg1);
    digitalWrite(latchPin, 1);
    delay(pause);
  }
}
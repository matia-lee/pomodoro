#include "LiquidCrystal.h"
#include "Encoder.h"

int seconds = 0;
int minutes = 0;
int count = 0;

int study_minutes = 0;
int short_break_minutes = 0;
int long_break_minutes = 0;
int repeats = 0;



int buttonState = 0; 
int lastButtonState = LOW; 
int messageNumber = 0;
long lastDebounceTime = 0;  
long debounceDelay = 50;  



int break_duration;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int ledGreen = 6;
const int ledRed = 7;
const int startButton = 8;
const int nextButton = A0;
const int pinCLK = 13;
const int pinDT = 10;
const int pinSW = 9;

Encoder myEnc(pinCLK, pinDT);

long oldPosition = 0;

void setup() 
{
  lcd.begin(16, 2);
  pinMode(ledGreen,OUTPUT);
  pinMode(ledRed,OUTPUT);
  pinMode(startButton,INPUT);
  pinMode(pinSW, INPUT_PULLUP);

  lcd.print("Welcome! ");
  lcd.setCursor(0, 1);
  lcd.print("Click to start->");

  Serial.begin(9600);
}

void loop() {

  int reading = digitalRead(nextButton);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == HIGH) {
        Serial.print("Setting button click: ");
        Serial.println(buttonState);
        Serial.print("Setting: ");
        Serial.println(messageNumber);
        messageNumber++;
        if (messageNumber > 4) {
          lcd.clear();
          lcd.print("<- Click left");
          lcd.setCursor(0, 1);
          lcd.print("to begin!");
        } else {
          lcd.clear();
          switch (messageNumber) {
            case 1:
              lcd.print("Study mins: ");
              lcd.setCursor(0, 1);
              lcd.print(study_minutes);
              break;
            case 2:
              lcd.print("Short break mins: ");
              lcd.setCursor(0, 1);
              lcd.print(short_break_minutes);
              break;
            case 3:
              lcd.print("Long break mins: ");
              lcd.setCursor(0, 1);
              lcd.print(long_break_minutes);
              break;
            case 4:
              lcd.print("# of study");
              lcd.setCursor(0, 1);
              lcd.print("sessions: ");
              lcd.print(repeats);
              break;
          }
        }
      }
    }
  }
  lastButtonState = reading;


  long newPosition = myEnc.read();
  if (newPosition != oldPosition) {
    displayMessage();
    oldPosition = newPosition;
  }

  if (digitalRead(startButton) == HIGH) {
    Serial.print("Start button click: ");
    Serial.println(startButton);
    startTimer();
  }
}

void displayMessage() {
  long newPosition = myEnc.read();
  int delta = newPosition - oldPosition;

  switch (messageNumber) {
    case 1:
      study_minutes = study_minutes + delta;
      Serial.print("New position: ");
      Serial.println(newPosition);
      Serial.print("Old position: ");
      Serial.println(oldPosition);
      Serial.print("Delta: ");
      Serial.println(delta);
      if (study_minutes < 0) study_minutes = 0;
      Serial.print("Study time initialized to: ");
      Serial.print(study_minutes);
      Serial.println(" minutes");
      break;
    case 2:
      short_break_minutes = short_break_minutes + delta;
      Serial.print("New position: ");
      Serial.println(newPosition);
      Serial.print("Old position: ");
      Serial.println(oldPosition);
      Serial.print("Delta: ");
      Serial.println(delta);
      if (short_break_minutes < 0) short_break_minutes = 0;
      Serial.print("Short break time initialized to: ");
      Serial.print(short_break_minutes);
      Serial.println(" minutes");
      break;
    case 3:
      long_break_minutes = long_break_minutes + delta;
      Serial.print("New position: ");
      Serial.println(newPosition);
      Serial.print("Old position: ");
      Serial.println(oldPosition);
      Serial.print("Delta: ");
      Serial.println(delta);
      if (long_break_minutes < 0) long_break_minutes = 0;
      Serial.print("Long break time initialized to: ");
      Serial.print(long_break_minutes);
      Serial.println(" minutes");
      break;
    case 4:
      repeats = repeats + delta;
      Serial.print("New position: ");
      Serial.println(newPosition);
      Serial.print("Old position: ");
      Serial.println(oldPosition);
      Serial.print("Delta: ");
      Serial.println(delta);
      if (repeats < 0) repeats = 0;
      Serial.print("Study sessions initialized to: ");
      Serial.print(repeats);
      Serial.println(" repeats");
      break;
  }

  lcd.clear();
  switch (messageNumber) {
    case 1:
      lcd.print("Study mins: ");
      lcd.setCursor(0, 1);
      lcd.print(study_minutes);
      break;
    case 2:
      lcd.print("Short break mins: ");
      lcd.setCursor(0, 1);
      lcd.print(short_break_minutes);
      break;
    case 3:
      lcd.print("Long break mins: ");
      lcd.setCursor(0, 1);
      lcd.print(long_break_minutes);
      break;
    case 4:
      lcd.print("# of study");
      lcd.setCursor(0, 1);
      lcd.print("sessions: ");
      lcd.print(repeats);
      break;
  }

  oldPosition = newPosition;
}

void startTimer () {
  count = 0; 
  Serial.print("Timer count: ");
  Serial.println(count);
  while(count<repeats){ 
    lcd.clear();
    lcd.print("Study time!");
    digitalWrite(ledGreen,LOW);
    digitalWrite(ledRed,HIGH);
    lcd.setCursor(0, 1);

    seconds = 60;
    minutes = study_minutes;
    minutes--;
    Serial.print("Study minutes: ");
    Serial.println(minutes);
    while(minutes >= 0){
      delay(1000);
      seconds = 59;
      while(seconds > 0){
        lcd.setCursor(0, 1);
        if(minutes < 10){
          lcd.print("0");
        }
        lcd.print(minutes);
        lcd.print(":");
        if(seconds < 10){  
          lcd.print("0");
        }
        lcd.print(seconds);
        delay(1000);
        Serial.print("Study seconds: ");
        Serial.println(seconds);
        seconds--;
      }
      Serial.print("Study minutes: ");
      Serial.println(minutes);
      minutes--;
    }
    
    lcd.clear();
    lcd.setCursor(0, 0);
    digitalWrite(ledGreen,HIGH);
    digitalWrite(ledRed,LOW);
    
    if (repeats != 1) {
      if(count==(repeats-1)){
        break_duration = long_break_minutes;
        lcd.print("Long break!");
      }
      else{ 
        break_duration = short_break_minutes;
        lcd.print("Short break!");
      }
    }

    lcd.setCursor(0, 1);
    seconds = 60;
    minutes = break_duration;
    minutes--;
    Serial.print("Long/Short Break minutes: ");
    Serial.println(minutes);
    while(minutes >= 0) {
      delay(1000);
      seconds = 59;
      while(seconds > 0){
        lcd.setCursor(0, 1);
        if(minutes < 10){
          lcd.print("0");
        }
        lcd.print(minutes);
        lcd.print(":");
        if(seconds < 10){
          lcd.print("0");
        }
        lcd.print(seconds);
        delay(1000);
        Serial.print("Long/Short Break seconds: ");
        Serial.println(seconds);
        seconds--;
      }
      Serial.print("Long/Short Break minutes: ");
      Serial.println(minutes);
      minutes--;
    }
    count++;
    Serial.print("Timer count: ");
    Serial.println(count);  
  }
  digitalWrite(ledGreen, LOW);
  digitalWrite(ledRed, LOW);
  lcd.clear();
  lcd.print("Study session");
  lcd.setCursor(0, 1);
  lcd.print("finished!");
}
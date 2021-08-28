// Include needed libraries and define some Pins
#include <BlynkSimpleStream.h>
//#define BLYNK_PRINT Serial
#define pinLedRed 13
#define pinFlame 12
#define pinBuzz 11
#define pinLedYellow 10
#define pinTemp A0
#define pinPower 2

// Declare the identity for the setInterval loop
int timerId;
// Create an instance of SimpleTimer
SimpleTimer timer;
// auth token is sent via Email when create the Blynk project
// auth can be copied via Setting in the Blynk project
char auth[] = "XxVw5OdJ65PMYhr1fbGH_k4JvVXHB-LG";

// Configure the Emergency Switch Button
BLYNK_WRITE(V1)
{
  int val = param.asInt();
  if (val == 1)
  {
    digitalWrite(pinPower, HIGH);
  } else {
    digitalWrite(pinPower, LOW);
  }
}

// Configure the Mute Alert Button
BLYNK_WRITE(V2)
{
  int val = param.asInt();
  if (val == 1)
  {
    Safe();
    timer.disable(timerId);
  } 
  else {
    timer.enable(timerId);
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(pinFlame, INPUT);
  pinMode(pinBuzz, OUTPUT);
  pinMode(pinTemp, INPUT);
  pinMode(pinPower, OUTPUT);
  // The electricity grids is available in default
  digitalWrite(pinPower, HIGH);
  timerId = timer.setInterval(1000L, Alert);
  Blynk.begin(auth,Serial);
  // The Emergency Switch is on in default, because the electricity grids is on
  Blynk.virtualWrite(V1, HIGH);
}

// Blinking the red led, notify by buzzer and Blynk notification
void WarningHigh()
{
    analogWrite(pinBuzz,1000);
    digitalWrite(pinLedRed, HIGH);
    delay(300); 
    digitalWrite(pinLedRed,LOW);
    delay(300);
    Blynk.notify("FIRE DETECTED!!!");
    delay(1000);
}

// Blinking the yellow led
void Warning()
{
    delay(300); 
    digitalWrite(pinLedYellow, HIGH);
    delay(300);
    digitalWrite(pinLedYellow,LOW);
}

// Turn off all alert
void Safe()
{
  digitalWrite(pinLedYellow, LOW);
  digitalWrite(pinLedRed, LOW);
  analogWrite(pinBuzz, 0);
}

// This function will be called in setInterval to collect real time sensor data 
// and process actions based on the predefined condition
void Alert()
{
  int isFlame = digitalRead(pinFlame);
  float temp = analogRead(pinTemp) * 5.0 / 1024.0 * 100.0;
  Blynk.virtualWrite(V0, temp);
 if(isFlame == HIGH)
 {
  WarningHigh();
 } 
 else if (temp >= 40.0) 
 {
  Safe();
  Warning(); 
 }
 else {
  Safe();
 }
}

void loop()
{
  timer.run();
  Blynk.run();
}

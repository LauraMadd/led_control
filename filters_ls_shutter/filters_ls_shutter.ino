#include <Servo.h>
int data = 0; //will contain the byte that is read later on 
int led_pin =  8; //blinking light led
int light_pin = 11; // brightfield light led
int ledState = LOW;  //the state of the blinking light   
// unsigned long time_prev = 0; // a variable used to determine when to switch the state of the blinking light
long interval = 0; //gives how long the on and off states of the blinking light are
Servo blue;
Servo green;

// variables for widefield opto led
#define light_hp_led 9 // widefield opto led
long timeNow; // keeps track of current time
long trainInterval; // interval between pulse trains in ms
long delayStart; // amount of time before opto stimulus protocol is started in ms
double period; // 
double offFor;
double onFor;
float numPulse;
float dutyCycle;
bool optoRestart;
long timeRestart; // keeps track of time for subsequent trials, otherwise delayStart will not function correctly?        
       

void setup() {
  Serial.begin(9600);
  blue.attach(5);
  blue.write(0);
  green.attach(3);
  green.write(0);
  pinMode(2, OUTPUT);
  pinMode(light_pin, OUTPUT); //brightfield light
  pinMode(led_pin, OUTPUT); //blinking light
  pinMode(light_hp_led, OUTPUT); //widefield opto led
  optoRestart = true;
}

void widefieldOpto() { // function to blink widefield opto led
 timeNow = millis();
  if ((timeNow - delayStart) >= (trainInterval)) {
  delayStart += trainInterval; // add DELAY_TIME to reference point for next iteration
    while (millis() < (delayStart + 1000)) { // this line causes trouble somehow, 999 vs. 1000 works better somehow?
      if (period > 16383) { // delay is accurate down to 16383 µs, for lower durations use delayMicroseconds
        digitalWrite(light_hp_led,HIGH);
        delay((onFor/1000) + 1); 
        digitalWrite(light_hp_led,LOW);
        delay((offFor/1000) + 1);
      } else {
        digitalWrite(light_hp_led,HIGH);
        delayMicroseconds(onFor + 10000);
        digitalWrite(light_hp_led,LOW);
        delayMicroseconds(offFor + 10000);
      }
    }
  }
}

void parOpto() {
  // receives parameter values through PySerial
  if (data == 'a') {
    delayStart = Serial.parseInt();
  }
  if (data == 'b') {
    trainInterval = Serial.parseInt();
  }
  if (data == 'c') {
    numPulse = Serial.parseInt();
  }
  if (data == 'd') {
    dutyCycle = Serial.parseInt();
  }
  period = 1000000 / numPulse; // 1e6 µs divided by number of pulses within a single pulse train
  offFor = period - (period * (dutyCycle/100)); // duration spent off in µs
  onFor = period - offFor; // duration spent on in µs
}

void loop() {
  while (Serial.available()){ 
        data = Serial.read();
        // sets parameters for BL opto led  if a-d is received
        if (data == 'a' || data == 'b' || data == 'c' || data == 'd') { 
          parOpto();
        }
 }
 
 if (data == '0'){
  blue.write(0);
  green.write(0);
  digitalWrite(light_hp_led,LOW);
  optoRestart = true;
  }
 else if (data == '1'){ 
  blue.write(20);
  green.write(0);
  }
 else if (data == '2'){
  blue.write(0);
  green.write(20);
  }
 else if (data == '3'){ //led off
  analogWrite(light_pin,0);
  }
 else if (data == '4'){  //led 25%
  analogWrite(light_pin, 65);
  }
 else if (data == '5'){  //led 50%
  analogWrite(light_pin, 125);
  }
 else if (data == '6'){  //led 75%
  analogWrite(light_pin, 190);
  }
 else if (data == '7'){  //led on
  analogWrite(light_pin, 255);
  }
 else if (data == '8'){
  blue.write(20);
  green.write(20);
  }
 else if (data == '9'){ 
  blue.write(20);
  green.write(0);
  if (optoRestart == true) {
    delay(delayStart);
    optoRestart = false;
  }
    widefieldOpto();  
  }  
}

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
unsigned long tStart; // keeps track of when experiment started
unsigned long tPulse; // keeps track of when a light pulse is generated
bool pulsetimeBool;

// variables for pump controller
#define pumpControl 2
float pumpSpeed; // controls the pumping speed

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
  pinMode(pumpControl, OUTPUT); //pump controller
  optoRestart = true;
}

void parOpto() {
  // receives parameter values through PySerial
  //if (data == 'b') {
    delayStart = 0;//Serial.parseInt();
 // }
  //else if (data == 'c') {
    trainInterval = 2000;//Serial.parseInt();
 // }
 // else if (data == 'd') {
    numPulse = 1;//Serial.parseInt();
  //}
 // else if (data == 'e') {
    dutyCycle = 20;//Serial.parseInt();
 // }
  period = 1000000 / numPulse; // 1e6 µs divided by number of pulses within a single pulse train
  offFor = period - (period * (dutyCycle/100)); // duration spent off in µs
  onFor = period - offFor; // duration spent on in µs
}

void widefieldOpto() { // function to blink widefield opto led
  timeNow = millis();
  if ((timeNow - delayStart) >= (trainInterval)) {
  delayStart += trainInterval; // add DELAY_TIME to reference point for next iteration
  Serial.println("g");
  }
  while (millis() < (delayStart + 1000)) {
    if (period > 16383) { // delay is accurate down to 16383 µs, for lower durations use delayMicroseconds
      tPulse = millis(); //This will not work if we are stuck in while loop!
      Serial.println(tPulse);
      digitalWrite(light_hp_led,HIGH);
      delay((onFor/1000)); 
      digitalWrite(light_hp_led,LOW);
      delay((offFor/1000));
    } else {
      tPulse = millis();
      Serial.println(tPulse);
      digitalWrite(light_hp_led,HIGH);
      delayMicroseconds(onFor);
      digitalWrite(light_hp_led,LOW);
      delayMicroseconds(offFor);
    }
  }
}

void loop() {
  while (Serial.available()){ 
    data = Serial.read();
  }
  if (data == 'a' || data == 'b' || data == 'c' || data == 'd') { // sets parameters for BL opto led  if "b-e" is received
    parOpto();
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
      parOpto();
      tStart = millis();
      Serial.print("f"); // sends signal to communicate with Python
      delay(delayStart);
      optoRestart = false;
    }
      widefieldOpto();  
  }
  else if (data == 'k'){
    digitalWrite(pumpControl,HIGH);
  }
  else if (data == 'l'){
    digitalWrite(pumpControl,LOW);
  }
}

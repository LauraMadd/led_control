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
int trainCount; // keeps track of the pulse train number
int cumSeconds; // keeps time in cumulative seconds
int minutes; // keeps time in minutes
int seconds; // keeps time in seconds
int milliseconds; // keeps time in milliseconds
double period; // 
double offFor;
double onFor;
float numPulse;
float dutyCycle;
bool optoRestart;       
unsigned long tStart; // keeps track of when experiment started
unsigned long tPulse; // keeps track of when a light pulse is generated
bool pulsetimeBool;
bool timeBool; 

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

void loop() {
  while (Serial.available()) { // checks whether serial buffer contains input from python
    data = Serial.read(); // loads in character input when available
    if (data == 'a' || data == 'b' || data == 'c' || data == 'd') { // sets parameters for BL led if a-d is received
      parOpto();
    }
  }
  selectProgram(); // selects program to run if data != a-d
}

void parOpto() { // function to initialise BL led parameters
  // receives parameter values through PySerial
  if (data == 'a') {
    delayStart = Serial.parseInt();
  }
  else if (data == 'b') {
    trainInterval = Serial.parseInt();
  }
  else if (data == 'c') {
    numPulse = Serial.parseInt();
  }
  else if (data == 'd') {
    dutyCycle = Serial.parseInt();
  }
  period = 1000000 / numPulse; // 1e6 µs divided by number of pulses within a single pulse train
  offFor = period - (period * (dutyCycle/100)); // duration spent off in µs
  onFor = period - offFor; // duration spent on in µs
}

void selectProgram() { // function to select program to run based on character received from python
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
      printHeaders();
      delay(delayStart);
      optoRestart = false;
    }
    widefieldOpto();  
  }
  else if (data == 'e'){
    digitalWrite(pumpControl,HIGH);
  }
  else if (data == 'f'){
    digitalWrite(pumpControl,LOW);
  }
}

void printHeaders() { // function to print parameter values and headers at the start of the experiment
  trainCount = 0;
  tStart = millis();
  Serial.println("Somehow this line is not being sent by Arduino");
  Serial.print("Acclimatisation time (s) ;"); // first two lines appear to be not printed
  Serial.println(delayStart/1000);
  Serial.print("Train interval (s) ;");
  Serial.println(trainInterval/1000);
  Serial.print("Pulse frequency (Hz) ;");
  Serial.println(numPulse);
  Serial.print("Duty cycle (%) ;");
  Serial.println(dutyCycle);
  Serial.println("T start (min:sec:millisec); Tpulse (min:sec:millisec); Train number"); // sends signal to communicate with Python
  Serial.print(tStart);
}

void widefieldOpto() { // function to blink widefield opto led
  timeNow = millis();
  if ((timeNow - delayStart) >= (trainInterval)) {
    delayStart += trainInterval; // add DELAY_TIME to reference point for next iteration
    timeBool = true; // allows counting pulse train number
    while (millis() < (delayStart + 1000)) { // must be 999, sometimes protocol fires early by 1 ms (1 Hz)
      if (period > 16383) { // delay is accurate down to 16383 µs, for lower durations use delayMicroseconds
        printTime();
        digitalWrite(light_hp_led,HIGH);
        delay((onFor/1000)); 
        digitalWrite(light_hp_led,LOW);
        delay((offFor/1000));
      } else {
        printTime();
        digitalWrite(light_hp_led,HIGH);
        delayMicroseconds(onFor);
        digitalWrite(light_hp_led,LOW);
        delayMicroseconds(offFor);
      }
    }
  }
}

void printTime() { // function to print time stamps for each optogenetic pulse
  long tNow = millis();
  tPulse = tNow - tStart; //This will not work if we are stuck in while loop!
  if (timeBool == true) {
    trainCount += 1;
    timeBool = false;
  }
  Serial.print(" ; ");
  cumSeconds = tPulse / 1000;
  milliseconds = tPulse - (cumSeconds * 1000);
  convertTime();
  Serial.println(trainCount);
}

void convertTime() { // function to convert milliseconds to min:sec:millisec format, requires cumSeconds and milliseconds to be defined first!
  minutes = cumSeconds / 60;
  seconds = cumSeconds - (60 * minutes);
  Serial.print(minutes);
  Serial.print(":");
  Serial.print(seconds);
  Serial.print(":");
  Serial.print(milliseconds);
  Serial.print(" ; ");
}

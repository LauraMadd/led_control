#include <Servo.h>
int data = 0; //will contain the byte that is read later on 
int led_pin =  8; //blinking light led
int light_pin = 11; // brightfield light led
int ledState = LOW;  //the state of the blinking light           
unsigned long time_prev = 0; // a variable used to determine when to switch the state of the blinking light
long interval = 0; //gives how long the on and off states of the blinking light are
Servo blue;
Servo green;           

void setup() {
  Serial.begin(9600);
  blue.attach(5);
  blue.write(0);
  green.attach(3);
  green.write(0);
  pinMode(2, OUTPUT);
  pinMode(light_pin, OUTPUT); //brightfield light
  pinMode(led_pin, OUTPUT); //blinking light
}
void loop() {
  unsigned long time_now = millis(); //this one is to track the time. note that is does overflow after 50 days, so it might give some weird blink after a seriously long use of the arduino
  while (Serial.available()){ 
        data = Serial.read();
        //commands for the blinking light always start with t, so it is possible to send additional data
        if(data =='t'){
          interval = Serial.parseInt(); //the additional data is read as an integer
        }
    }
 if (data == '0'){
  blue.write(0);
  green.write(0);
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
  if(interval > 0){
    //interval > 0 corresponds to the state of the led where it is on and off equally long
    if (time_now - time_prev >= interval) {
      time_prev = time_now;
      if (ledState == LOW) {
        ledState = HIGH;
      } else {
        ledState = LOW;
      }
      digitalWrite(led_pin, ledState);
      //to keep track of when it needs to switch to a different state, the distance between the previous value and the current value is determined compared with interval, and if it is bigger, then it is  time to switch
    }
  }
  else if (interval == 0){
    // interval == 0 corresponds to the continiously off state of the led
  digitalWrite(led_pin, LOW);
}
else{
  //this only is checked if interval < 0 and it corresponds to the continiously off state of the led
  digitalWrite(led_pin, HIGH);
}
}

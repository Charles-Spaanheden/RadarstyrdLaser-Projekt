/*
File: Arduino_projekt_MRGL    
Author: Charles Spaanheden
Date: 2023-12-10
Description: A mounted radar guided laser that aims at the closest object in vicinity

Note: The code includes oled code but the project doesn't have a oled display inserted on the breadboard. 
*/

#include <Servo.h>
#include <U8glib.h>
#include <Wire.h>

const unsigned int ECHO_PIN = 12;
const unsigned int TRIG_PIN = 13;

 // creates a servo by using the Servo library
Servo servo;
Servo servolaser;
U8GLIB_SH1106_128X64 oled(U8G_I2C_OPT_NO_ACK);

int pos = 90; // sets the position of the radar servo
int poslaser = 90; // sets the position of the laser servo
const unsigned int BAUD_RATE = 9600;
int smallest_distance = 7500/29/2; // sets the max distance to 130cm ( /29/2 is the formula to turn the info the ultrasonic sensor sends to cm units)
int smallest_angle = 180; //setting a variable for the laser servo

/*
VOID SETUP

sets up the ultrasonic sensor module pins
sets servo pins
pin 6 = HIGH --> Laser is always on 
*/
void setup() {
  servo.attach(8);
  servolaser.attach(7);
  digitalWrite(6, HIGH);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.begin(BAUD_RATE);
  Wire.begin(); 
  oled.setFont(u8g_font_helvB10);
  oled.begin(); 
}

/*
VOID LOOP

Moves the radar servo from 40 to 140 in a sweeping motion. 
Everytime the sensor sends a distance that is distance < smallest_distance, smallest_distance is replaced with the current distance.
writes on the serial monitor to display the distance, smallest distance and what degree the laser servo is. 
When the sweeping motion turns the opposite way there is a 50 delay and a line of code that puts the smallest distance at 130cm to prevent it from becoming 0 for some reason.
*/
void loop() {
  int distance; 

  for (int i = 40; i <= 140; i = i + 3) {
    pos = i;
    servo.write(i);
    delay(5);
    distance = measureDistance(); 
    delay(1);
    Serial.println("Distans till objekt: " + String(distance) + " cm");
    Serial.println("Minsta distans: " +String(smallest_distance));
    Serial.println(smallest_angle);
    Serial.println("Position: " + String(pos));


    if (distance < smallest_distance) {
      Serial.println("hejhejhejhejhej");
      smallest_distance = distance;
      smallest_angle = pos;
      poslaser = smallest_angle;
      servolaser.write(poslaser);
      Serial.print("Smallest Distance: ");
      Serial.print(smallest_distance);
      Serial.print(", Smallest Angle: ");
      Serial.println(smallest_angle);
      Serial.println("Position: " + String(pos));
    }
  }

delay(50);
smallest_distance = 7500/29/2;


  for (int i = 140; i >= 0; i = i - 3) {
    pos = i;
    servo.write(i);
    delay(5);
    distance = measureDistance(); 
    delay(1);
    Serial.println("Distans till objekt: " + String(distance) + " cm");
    Serial.println("Minsta distans: " +String(smallest_distance));
    Serial.println(smallest_angle);

    
    if (distance < smallest_distance) {
      Serial.println("hejhejhejhejhej");
      smallest_distance = distance;
      smallest_angle = pos;
      poslaser = smallest_angle;
      servolaser.write(poslaser);
      Serial.print("Smallest Distance: ");
      Serial.print(smallest_distance);
      Serial.print(", Smallest Angle: ");
      Serial.println(smallest_angle);
    }
  }
}
/*
MEASURE DISTANCE

is called upon everytime the distance is being meassured
The "TRIG_PIN" is turned on and off repeatedly
The value that is received by ECHO_PIN is divided by 29 then 2 to turn it to cm. 
the distance is then returned to the void loop
*/
int measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  unsigned long duration = pulseIn(ECHO_PIN, HIGH);
  int distance = duration / 29 / 2;
  return distance;
}

/*
UPDATE OLED

Not important in the moment since the oled is not being used. This can be improved in the future so this code can be used.
I chose not to use this code because the oled module kept glitching and there was bigger priorities.
*/
void updateOled(String text, String text2) {
  oled.firstPage();
  do {
    oled.drawStr(0, 15, text.c_str());
    oled.drawStr(0, 50, text2.c_str());
  } while (oled.nextPage());
}


#include <LiquidCrystal.h>
#include <Servo.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

Servo myServo;

long duration;   //variables
float distance;

float alertDistance = 25;  //trigger distance (inches)

int joyX = A0;    //servo stuff + joystick
int servopos = 0;
int joybutton = 7;


const int trigPin = 9;   //ultrasonic sensor pins
const int echoPin = 10;

const int buzzerPin = 8;  //buzzer pin

int beepDuration = 100;
unsigned long lastBeep = 0;


void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  myServo.attach(6); //pwm pin 6
  myServo.write(servopos); //set position to middle 0-180 degree range

  pinMode(joybutton, INPUT_PULLUP);

  Serial.begin(9600); 

  lcd.begin(16, 2);
  lcd.print("Sonar Test");
  delay(1500);
  lcd.clear();



}

void loop() {   // not actaul used here
  int xVar = analogRead(joyX);   // goes 0 - 1023
  int step = 10;
  int buttonstate = digitalRead(joybutton);

  if (buttonstate == HIGH) {    //button not pressed

    if (xVar < 400) {           //moves left and right from joystick
    servopos -= step;
  } else if (xVar >600) {
    servopos += step;
  }

  //servopos = constrain(servopos, 0, 180);

  //myServo.write(servopos);

  }

  if (buttonstate == LOW) {       // button pressed turns on autoswivel
    servopos += step;

    if (xVar >= 180 || xVar <= 0) {
      servopos = -step;  //way to switch directions at end
    }

  }
  


  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 30000);

  if (duration == 0) {
    distance = -1;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Out of Range");

  } else {
    distance = duration * 0.0135 / 2;
    Serial.print("Distance: ");   //serial print
    Serial.print(distance);
    Serial.println(" inches");
    delay(50);

    lcd.clear();          //LCD output
    lcd.setCursor(0, 0);
    lcd.print("Distance: ");
    lcd.setCursor(0, 1);
    lcd.print(distance, 1);
    lcd.print(" inches");
    delay(100);


    Serial.println(distance);

    if(distance > 0 && distance <= alertDistance){
    // Map distance to beep interval (closer = faster)
    int interval = map(distance, 1, alertDistance, 50, 1000); 
    // 0 cm → 100 ms between beeps, 50 cm → 1000 ms between beeps

    if(millis() - lastBeep >= interval){
      tone(buzzerPin, 1000, beepDuration); // beep for 100 ms
      lastBeep = millis();
    }
  } else {
    noTone(buzzerPin); // turn off if out of range
  }


  }

  delay(50);




float plottedDistance = (distance < 0) ? 100.0 : distance; //graph

// Keep graph within 0-100 inches
if (plottedDistance > 100.0) {
  plottedDistance = 100.0;
}

// Send labeled graph data
Serial.print("Distance (inches):");
Serial.print(plottedDistance);

Serial.print("\tAlert Threshold (inches):");
Serial.println(alertDistance);


 }
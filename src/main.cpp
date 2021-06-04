#include <Arduino.h>

//--Motor Settings --
#define maxSpeed 255   // Максимальна Швидкість
#define normalSpeed 50 // Нормальна швидкість
#define turnSpeed 25   // Швидкість колеса при повороті

//-- Motor Right Pins --
#define speedRight 5
#define rightA 6
#define rightB 7

//-- Motor Left Pins --
#define speedLeft 10
#define leftA 8
#define leftB 9

//-- Distance Pins --
#define leftDistance A7
#define rightFrontDistance A3
#define leftFrontDistance A6
#define rightDistance A2

//-- Line track Pins --
#define rightFront A0
#define rightBack 13
#define leftBack 12
#define leftFront A1



class Motor
{

private:
  byte pinForward;
  byte pinBackward;
  byte pinSpeed;

public:
  Motor()
  {
  }

  Motor(int pinF, int pinB, int pinS)
  {
    pinForward = pinF;
    pinBackward = pinB;
    pinSpeed = pinS;
  }

  void forward(int speed)
  {
    digitalWrite(pinForward, HIGH);
    digitalWrite(pinBackward, LOW);
    analogWrite(pinSpeed, speed);
  }
  void backward(int speed)
  {
    digitalWrite(pinForward, LOW);
    digitalWrite(pinBackward, HIGH);
    analogWrite(pinSpeed, speed);
  }
};

class Car
{
public:
  Motor *leftSide;  // * це покажчик
  Motor *rightSide; // * це покажчик

  void setLeftSide(int pinF, int pinB, int pinS)
  {
    leftSide = new Motor(pinF, pinB, pinS);
  }

  void setRightSide(int pinF, int pinB, int pinS)
  {
    rightSide = new Motor(pinF, pinB, pinS);
  }

  void forward(int speed)
  {
    leftSide->forward(speed);
    rightSide->forward(speed);
  }

  void backward(int speed)
  {
    leftSide->backward(speed);
    rightSide->backward(speed);
  }

  void rotate(int speed)
  {
    leftSide->backward(speed);
    rightSide->forward(speed);
  }

  int getDistance(int pin)
  {
    float volts = analogRead(pin) * 0.0048828125;
    return 13 * pow(volts, -1);
  }

  void ringCheck()
  {
    if (digitalRead(rightFront) || digitalRead(leftFront))
    {
      backward(maxSpeed);
      delay(150);
      rotate(maxSpeed);
      delay(300);
      forward(255);
    }
    else if (digitalRead(rightBack) || digitalRead(leftBack))
    {
      forward(maxSpeed);
      delay(150);
      forward(maxSpeed);
    }
  }

  void debugSensors()
  {
    Serial.println("---- Distance ----");
    Serial.print("LF = ");
    Serial.println(getDistance(leftFrontDistance));
    Serial.print("RF = ");
    Serial.println(getDistance(rightFrontDistance));

    Serial.print("L = ");
    Serial.println(getDistance(leftDistance));
    Serial.print("R = ");
    Serial.println(getDistance(rightDistance));

    Serial.println("---- Line ----");
    Serial.print("LF = ");
    Serial.println(digitalRead(leftFront));
    Serial.print("RF = ");
    Serial.println(digitalRead(rightFront));
    Serial.print("LB = ");
    Serial.println(digitalRead(leftBack));
    Serial.print("RB = ");
    Serial.println(digitalRead(rightBack));
    delay(500);
  }
};

Car sumo; //Створення об'єкту машинки

void setup()
{
  pinMode(rightA, OUTPUT);
  pinMode(rightB, OUTPUT);
  pinMode(leftA, OUTPUT);
  pinMode(leftB, OUTPUT);

  pinMode(speedLeft, OUTPUT);
  pinMode(speedRight, OUTPUT);

  sumo.setLeftSide(rightA, rightB, speedRight);
  sumo.setRightSide(leftA, leftB, speedLeft);
  Serial.begin(9600);
}

void loop()
{
  sumo.debugSensors();
}
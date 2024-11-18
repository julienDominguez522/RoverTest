// RoverTest.ino
// FGCU Computing & Software Engineering
// Prof. Allen
// Test the basic Rover robot interfaces. Uses a set of states
// to run the wheels through each of the basic movements and
// adjusts the head to point in the direction of movement. The
// Serial inteface is used to output direction of movement, along
// with direction of the head and distance measured.
// ------------------------------------------------------------

#include "RoverHead.h"
#include "RoverWheels.h"
#include "StopWatch.h"

enum RoverWheelState {
  Stop,
  Straight,
  ShortStraight,
  CorrectLeft,
  TurnLeft,
  CorrectRight,
  TurnRight,
  Backup
};


fgcu::FourPin motorPinsLeft { A0, A1, A2, A3};
fgcu::FourPin motorPinsRight { 4, 5, 6, 7};
float speed = 600.f;
fgcu::RoverWheels wheels{motorPinsLeft, motorPinsRight, speed};

RoverWheelState wheelStates[] = { Straight, TurnLeft, ShortStraight, Backup, 
                                  CorrectRight, ShortStraight, CorrectRight, 
                                  Straight, Stop };
int wheelStateCount = 9;
int wheelStateIndex = 0;

const byte ServoPin = 9;
const byte EchoPin = A5;
const byte TriggerPin = A4;

fgcu::RoverHead head{EchoPin, TriggerPin, ServoPin};



void setup() {

  delay(2000);

  Serial.begin(9600);

  head.turnHead(90);

  // blocking call to get the head turned 
  // to 90 degrees and distance taken
  bool done = false;
  do {
    done = head.run();
  } while (!done);
  
} // setup


void loop() {

  // run the head as often as possible in case it is turning
  head.run();

  // run the wheels as often as possible in case they are moving
  if (!wheels.run()) { // if last movement completed

    // start next movement
    if (wheelStateIndex < wheelStateCount) {      
      Serial.print("Bearing: ");
      Serial.print(head.getBearing());
      Serial.print(" Distance: ");
      Serial.print(head.getDistance());
      Serial.print('\n');

      moveWheels();
      ++wheelStateIndex;
    }

  }

} // loop

// method uses the states collection to see what wheel state to execute next
void moveWheels() {

  switch (wheelStates[wheelStateIndex]) {
    case Stop:
      Serial.print("Stop\n");
      wheels.stop();
      head.turnHead(90);
      break;
    case Straight:
      Serial.print("Straight\n");
      wheels.moveForward();
      head.turnHead(90);
      break;
    case ShortStraight:
      Serial.print("ShortStraight\n");
      wheels.moveForward(0.5f);
      head.turnHead(90);
      break;
    case CorrectLeft:
      Serial.print("CorrectLeft\n");
      wheels.turnLeft(0.5f);
      head.turnHead(135);
      break;
    case TurnLeft:
      Serial.print("TurnLeft\n");
      wheels.turnLeft();
      head.turnHead(180);
      break;
    case CorrectRight:
      Serial.print("CorrectRight\n");
      wheels.turnRight(0.5f);
      head.turnHead(45);
      break;
    case TurnRight:
      Serial.print("TurnRight\n");
      wheels.turnRight();
      head.turnHead(0);
      break;
    case Backup:
      Serial.print("Backup\n");
      wheels.moveBackward();
      head.turnHead(90);
  } // rover state

} // moveWheels


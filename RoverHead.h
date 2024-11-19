// RoverHead.h
// FGCU Computing & Software Engineering
// Prof. Allen
// Control the head of the Rover robot car that contains a
// Servo motor with 180 degree travel and an Ultrasonic sensor
// for taking measurements in inches or centimeters.
// ------------------------------------------------------------

#include "ServoMotor.h";
#include "Ultrasonic.h";

#ifndef FGCU_ROVER_HEAD
#define FGCU_ROVER_HEAD

namespace fgcu {

class RoverHead {
private:

  byte _bearing;
  word _distance;

  bool _measuring;

  Ultrasonic _sonic;
  ServoMotor _servo;

public:

  RoverHead( byte echo=0, byte trigger=0 ,byte servoPin=9) 
    : _sonic{echo, trigger}, _servo{servoPin}, _measuring{false}, _bearing{0}, _distance{0} {    
  }

  bool isMeasuring() { return _measuring; }

  byte getBearing() { return _bearing; }

  word getDistance() { return _distance; }

  void turnHead(byte direction) {
    _bearing = direction;
    _servo.moveTo(_bearing);
    _measuring = true;
    run(); // kick off running
  }


  bool run() {
    bool running = false;

    if (_measuring) { // taking a measurement
    
      if (!_servo.run()) { // servo reach destination

        // get measurement (blocking)
        word inches = _sonic.inches();

        // if measurement didn't timeout
        if (inches > 0) { 
          _distance = inches;
        }
        
        _measuring = false;
        
      } 
      // servo is still moving
      else {
        running = true;
      }

    } // measuring

    return running; 
  } // run


}; // RoverHead


}; // fgcu


#endif // FGCU_ROVER_HEAD

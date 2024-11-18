// RoverHead.h
// FGCU Computing & Software Engineering
// Prof. Allen
// Control the drive train of the Rover robot which has two 
// driving wheels mounted side-by-side with a rear castor wheel.
// The wheels are driven by 28BYJ-48 stepper motors with ULN2003 
// motor drivers with the left motor inverted to the travel of
// the robot. The Rover provides all of the major movements
// with a scale factor < 1.0 to create proportional steps, where
// a step is a full tire rotation. Turns are done using the 
// tank-tread method of the outside wheel of the turn moving 
// forward and the inside wheel moving in reverse. Turns are 
// defaulted to half-step and can be adjusted by a scale factor.  
// 
// The run method must be called as often as possible, i.e. in 
// the main loop() method. A moving flag indicates that a move
// has been initiated, and is false, and run returns false when
// a target rotation has been reached.
// ------------------------------------------------------------

#include "Motor.h"

namespace fgcu {

class RoverWheels {
public:
  enum Direction { Forward, Left, Right, Reverse, Stop };

private:
 static const float TURN_90_FACT = 0.55f;
	
	fgcu::Motor _motorLeft;
	fgcu::Motor _motorRight;

  float _speed;

  bool _moving;

public:

	RoverWheels(FourPin& leftPins, FourPin& rightPins, float speed)  {
    
    _motorLeft = Motor(leftPins, true);
    _motorRight = Motor(rightPins);

    _speed = speed;

    _moving = false;
	}

  bool isMoving() { return _moving; }

  float setSpeed(float speed) { 
    _speed = speed;
    _motorLeft.setSpeed(_speed);
    _motorRight.setSpeed(_speed);
  }
	
	void moveForward(float scale=1.f) { 
    _motorLeft.forward(_speed, scale);
    _motorRight.forward(_speed, scale);
    _moving = true;
	}
	
	void moveBackward(float scale=1.f) { 
    _motorLeft.reverse(_speed, scale);
    _motorRight.reverse(_speed, scale);
    _moving = true;
	}
	
	void turnLeft(float scale=1.f) { 
    _motorLeft.reverse(_speed, 0.5f * scale);
    _motorRight.forward(_speed, 0.5f * scale);
    _moving = true;
	}
	
	void turnRight(float scale=1.f) { 
    _motorLeft.forward(_speed, 0.5f * scale);
    _motorRight.reverse(_speed, 0.5f * scale);
    _moving = true;
  }
	
	void stop() {  
    _motorLeft.setSpeed(0);
    _motorRight.setSpeed(0);
    _moving = false;
  }
	
	bool run() {
    
    if (_moving) {
      _motorLeft.runSpeedToPosition();
      _motorRight.runSpeedToPosition();
      
      bool atTargetLeft = _motorLeft.distanceToGo() == 0;
      bool atTargetRight = _motorRight.distanceToGo() == 0;

      _moving = !(atTargetLeft && atTargetRight);
    }

    return _moving;
	} // run

}; // RoverWheels

}; // fgcu


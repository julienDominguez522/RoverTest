// ServoMotor.h
// FGCU Computing & Software Engineering
// Prof. Allen
// Servo motor library for 3-wire hobby servo. 
// Based on a full 180 degree traversal in 1.2 seconds.
//
// Timer1 will be used each time moveTo() is called and must
// be reset by the caller.
// ------------------------------------------------------------
#include <Servo.h>
#include "StopWatch.h"

#ifndef FGCU_SERVO_MOTOR_H
#define FGCU_SERVO_MOTOR_H

namespace fgcu {

class ServoMotor {
private:

  // 1.2 second full 180 degree travel
  const float MillisPerDeg = 6.67f; 

  // servo positioning constants
  const word ServoMin = 500;
  const word ServoMax = 2500;

  // servo stop-watch 
  StopWatch _timer;

  // servo current and next positions
  byte _position;
  byte _target;
   
  byte _pin;    // gpio for servo motor 
  Servo _servo; // the servo motor

public:

  // default/property constructor
  ServoMotor(byte pin=9) 
    : _pin(pin), _timer{}, _position(0), _target(0) {
  }

  // destructor ensures servo is detached
  ~ServoMotor() {
  	if (_servo.attached())
      _servo.detach();
  }
  
  // copy constructor
  ServoMotor(const ServoMotor& other) {
    this->_timer = other._timer;
    this->_position = other._position;
    this->_target = other._target;
    this->_pin = other._pin;
  }

  // copy assignment operator
  ServoMotor& operator=(const ServoMotor& rhs) {
    // self-assignment check
    if (this != &rhs) {
      this->_timer = rhs._timer;
      this->_position = rhs._position;
      this->_target = rhs._target;
      this->_pin = rhs._pin;
    }
    return *this;
  }

  // move constructor
  ServoMotor(const ServoMotor&& other) {
    this->_timer = other._timer;
    this->_position = other._position;
    this->_target = other._target;
    this->_pin = other._pin;
  }

  // move assignment operator
  ServoMotor& operator=(const ServoMotor&& rhs) {
    // self-assignment check
    if (this != &rhs) {
      this->_timer = rhs._timer;
      this->_position = rhs._position;
      this->_target = rhs._target;
      this->_pin = rhs._pin;
    }
    return *this;
  }


  byte getPosition() { return _position; }

  byte getTarget() { return _target; }

  /**
   * Move the servo to a position between 0 and 180 
   * Attaches to the servo pin and uses Timer1.
   * @param position - target value in degrees
   * @return - true if servo can move (not already moving) and position is valid
   */
  bool moveTo(byte position) {
    bool canMove = false;

    if (position < 0 || position > 180)
      return false;

    // make sure there isn't an active timer running
    if (!(_servo.attached() && _timer.isRunning())) {
      // save the new target
      _target = position;

      // calculate difference in degrees from current position to target
      float delta = _target > _position ? _target - _position : _position - _target; 

      // set a timer as of now for degrees to move per millis
      _timer.start(delta * MillisPerDeg);

      // start the servo moving
      _servo.attach(_pin, ServoMin, ServoMax);
      _servo.write(_target);

      canMove = true;
    }

    return canMove;
  } // moveTo

  bool isRunning() {
    return run();
  }

  /**
   * Ticks off a stopwatch for the number of milliseconds-per-degree travel
   * and updates position and detaches from the servo pin and Timer1 at completion.
   * Run should be called as often as possible in the main loop. 
   * @return - true if the servo is running (attached) and time has not expired.
   */
  bool run() {
    bool running = false;

    if (_servo.attached()) { // on process when attached
    
      // see if the servo has an active timer going (moving)
      if (_timer.isRunning()) {        
        running = true;
      }
      else { // time completed
        _timer.reset();
        _position = _target;
        _target = 0;
        _servo.detach();
      }
    
    } // attached
    
    return running;
  } // run

}; // ServoMotor

}; // fgcu

#endif FGCU_SERVO_MOTOR_H

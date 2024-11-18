// Motor.h
// Prof. Allen
// FGCU Computing & Software Engineering
//
// Abstraction of a single stepper motor such as the 28BYJ-48 using
// the AccelStepper library to support control of mutliple instances
// of this class.
// --------------------------------------------------------------------------------

#include <AccelStepper.h>

namespace fgcu {

// helper structure for defining a single 4-pin variabley
struct FourPin {
  byte in1;
  byte in2;
  byte in3;
  byte in4;
};

class Motor : public AccelStepper {
  private:
    bool _inverted;
  public:

  static const float MAX_SPEED = 2000.f;
  static const float DEFAULT_SPEED = 600.f;
  static const float DEFAULT_ACCEL = 100.f;
  static const float FULL_STEP = 2038.f;

  Motor(FourPin pins={3,4,5,6}, bool invert=false) 
      : AccelStepper(AccelStepper::MotorInterfaceType::FULL4WIRE, pins.in1, pins.in3, pins.in2, pins.in4),
        _inverted(invert) {
    setMaxSpeed(MAX_SPEED);  
  }

  void forward(float speed, float scale=1.f) {
    setCurrentPosition(0);
    float target = FULL_STEP;
    if (_inverted)
      target *= -1;
    target *= scale;
    move(target);
    setSpeed(speed);
  }

  void reverse(float speed, float scale=1.f) {
    setCurrentPosition(0);
    float target = -FULL_STEP;
    if (_inverted)
      target *= -1;
    target *= scale;
    move(target);
    setSpeed(speed);
  }


}; // Motor



}; // fgcu

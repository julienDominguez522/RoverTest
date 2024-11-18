// Ultrasonic.h
// FGCU Computing & Software Engineering
// Prof. Allen
// Use an Ultrasonic module to take measurements in inches 
// or millimeters using a blocking call.
// ------------------------------------------------------------

#ifndef FGCU_ULTRASONIC
#define FGCU_ULTRASONIC

namespace fgcu {

class Ultrasonic {
private:
  
  unsigned long Timeout = (400 * 29.1 * 2);
  
  byte echoPin;
  byte triggerPin;
  
public:
  
  Ultrasonic(byte echo=-1, byte trigger=-1) 
    : echoPin{echo}, triggerPin{trigger} {
      
      if (echoPin > 0)
        pinMode(echoPin, INPUT);
      
      if(triggerPin > 0)
        pinMode(triggerPin, OUTPUT);      
  } // constructor
  
  
  long measure() {
    unsigned long duration{0};
    
    noInterrupts();
        
    // make sure echo pin is low to start
    pinMode(echoPin, OUTPUT);
    digitalWrite(echoPin, LOW);
    delayMicroseconds(5);
    pinMode(echoPin, INPUT);
        
    // trigger module
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);
     
    // wait for echo pin to go High
    duration = pulseIn(echoPin, HIGH, Timeout);
    
    interrupts();
    
    if (duration > Timeout)
      duration = 0;
    
    return duration;
  } // measure
  
  unsigned int centimeters() {
  	return (measure() / 2) / 29.1;
  }
  
  unsigned int inches() {
  	return (measure() / 2) / 74;
  }
  
  
}; // Ultrasonic

}; // fgcu

#endif // FGCU_ULTRASONIC

// StopWatch.h
// FGCU Computing & Software Engineering
// Prof. Allen
// Utility class provides an asyncronous timer. 
// ------------------------------------------------------------

#ifndef FGCU_STOP_WATCH
#define FGCU_STOP_WATCH

namespace fgcu {

class StopWatch {
public:
  enum Precision {
    Millis,
    Micros
  };

private:

  unsigned long _start;
  unsigned long _stop;
  fgcu::StopWatch::Precision _precision;

public:

  StopWatch(unsigned long stop=0, fgcu::StopWatch::Precision precision=Millis)
    : _start(0), _stop(stop), _precision(precision) { }

  // copy constructor
  StopWatch(const StopWatch& other) {
    this->_start = other._start;
    this->_stop = other._stop;
  }

  // copy assignment operator
  StopWatch& operator=(const StopWatch& rhs) {
    // self-assignment check
    if (this != &rhs) {
      this->_start = rhs._start;
      this->_stop = rhs._stop;
    }
    return *this;
  }

  // move constructor
  StopWatch(const StopWatch&& other) {
    this->_start = other._start;
    this->_stop = other._stop;
  }

  // move assignment operator
  StopWatch& operator=(const StopWatch&& rhs) {
    // self-assignment check
    if (this != &rhs) {
      this->_start = rhs._start;
      this->_stop = rhs._stop;
    }
    return *this;
  }

  unsigned long getStart() { return _start; }
  
  unsigned long getStop()  { return _stop; }
  void setStop(unsigned long stop){ _stop = stop; }

  fgcu::StopWatch::Precision getPrecision() { return _precision; }
  void setPrecision(fgcu::StopWatch::Precision precision) { 
    _start = 0;
    _stop = 0;
    _precision = precision; 
    }


  void start(unsigned long stop) {
    if (_precision == Millis)
      _start = millis();
    else
      _start = micros();
    
    _stop = stop;
  }

  bool run() {
    bool running = false;

    if (_stop > 0) {
      unsigned long now{0};

      if (_precision == Millis)
        now = millis();
      else
        now = micros();

      running = (now - _start < _stop);
    }
    else {
      reset();
    }

    return running;
  }

  bool isRunning() { return run(); }
  

  void reset() {
    _start = 0;
    _stop = 0;
    _precision = Millis;
  }

}; //StopWatch

}; // fgcu

#endif // FGCU_STOP_WATCH
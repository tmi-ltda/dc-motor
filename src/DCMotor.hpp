#pragma once

#include <pwm-config.h>
#include <stdint.h>
#include <Arduino.h>

class DCMotor {
  private:
    uint8_t _pin1, _pin2;
    uint16_t _speed;

  public:
    DCMotor(uint8_t pin1, uint8_t pin2) : _pin1(pin1), _pin2(pin2), _speed(4095) {
      ledcAttach(_pin1, PWM_FREQUENCY, PWM_RESOLUTION);
      ledcAttach(_pin2, PWM_FREQUENCY, PWM_RESOLUTION);

      stop();
    }

    void forward() {
      ledcWrite(_pin1, _speed);
      ledcWrite(_pin2, 0);
    }

    void backward() {
      ledcWrite(_pin1, 0);
      ledcWrite(_pin2, _speed);
    }

    void stop() {
      ledcWrite(_pin1, 0);
      ledcWrite(_pin2, 0);
    }

    void setSpeed(uint16_t speed) {
      _speed = speed;
    }
};
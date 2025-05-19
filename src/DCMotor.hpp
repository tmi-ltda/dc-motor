#pragma once

#include "../include/pwm-config.h"
#include <stdint.h>
#include <Arduino.h>

class DCMotor {
  private:
    uint8_t _pin1, _pin2;
    uint16_t _forward_speed, _backward_speed;
    float _max_power;
    unsigned long last_call = 0;

  public:
    DCMotor(uint8_t pin1, uint8_t pin2)
    : _pin1(pin1), _pin2(pin2), _forward_speed(MIN_SPEED), _backward_speed(MIN_SPEED), _max_power(1.0f) {
      ledcAttach(_pin1, PWM_FREQUENCY, PWM_RESOLUTION);
      ledcAttach(_pin2, PWM_FREQUENCY, PWM_RESOLUTION);

      stop();
    }

    void forward() {
      ledcWrite(_pin1, accelerate(_forward_speed));
      ledcWrite(_pin2, decelerate(_backward_speed));
    }

    void backward() {
      ledcWrite(_pin1, decelerate(_forward_speed));
      ledcWrite(_pin2, accelerate(_backward_speed));
    }

    void stop() {
      ledcWrite(_pin1, decelerate(_forward_speed));
      ledcWrite(_pin2, accelerate(_backward_speed));
    }

    void setMaxPower(float power) {
      _max_power = min(power, 1.0f);
    }

    uint16_t accelerate(uint16_t &speed) {
      if (last_call - millis() >= 10) {
        last_call = millis();

        return speed = min(speed + 1, MAX_SPEED) * _max_power;
      }

      return speed;
    }

    uint16_t decelerate(uint16_t &speed) {
      if (last_call - millis() >= 10) {
        last_call = millis();

        return speed = max(speed - 1, MIN_SPEED) * _max_power;
      }

      return speed;
    }
};
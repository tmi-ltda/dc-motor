#pragma once

#include <pwm-config.h>
#include <stdint.h>
#include <Arduino.h>

class DCMotor {
  private:
    uint8_t _pin1, _pin2;
    int16_t _forward_speed, _backward_speed;
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
      accelerate(_forward_speed);
      decelerate(_backward_speed);
      ledcWrite(_pin1, _forward_speed * _max_power);
      ledcWrite(_pin2, _backward_speed * _max_power);
    }

    void backward() {
      decelerate(_forward_speed);
      accelerate(_backward_speed);
      ledcWrite(_pin1, _forward_speed * _max_power);
      ledcWrite(_pin2, _backward_speed * _max_power);
    }

    void stop() {
      decelerate(_forward_speed);
      decelerate(_backward_speed);
      ledcWrite(_pin1, _forward_speed * _max_power);
      ledcWrite(_pin2, _backward_speed * _max_power);
    }

    void setMaxPower(float power) {
      _max_power = min(power, 1.0f);
    }

    void accelerate(int16_t &speed) {
      if (millis() - last_call >= 10) {
        speed = min((int16_t)MAX_SPEED, speed += ACCELERATION);
        last_call = millis();
      }
    }

    void decelerate(int16_t &speed) {
      if (millis() - last_call >= 10) {
        speed = max(speed -= ACCELERATION, (int16_t)MIN_SPEED);
        last_call = millis();
      }
    }
};
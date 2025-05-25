#pragma once

#include <pwm-config.h>
#include <stdint.h>
#include <Arduino.h>

class DCMotor {
  private:
    uint8_t _pin1, _pin2;
    uint16_t _speed;
    float _max_power;
    unsigned long last_call = 0;

  public:
    DCMotor(uint8_t pin1, uint8_t pin2)
    : _pin1(pin1), _pin2(pin2), _speed(MAX_SPEED), _max_power(1.0) {
      ledcAttach(_pin1, PWM_FREQUENCY, PWM_RESOLUTION);
      ledcAttach(_pin2, PWM_FREQUENCY, PWM_RESOLUTION);

      stop();
    }

    void forward() {
      ledcWrite(_pin1, _speed * _max_power);
      ledcWrite(_pin2, 0);
    }

    void backward() {
      ledcWrite(_pin1, 0);
      ledcWrite(_pin2, _speed * _max_power);
    }

    void halfForward() {
      ledcWrite(_pin1, _speed * _max_power / 2);
      ledcWrite(_pin2, 0);
    }

    void halfBackward() {
      ledcWrite(_pin1, 0);
      ledcWrite(_pin2, _speed * _max_power / 2);
    }

    void stop() {
      ledcWrite(_pin1, 0);
      ledcWrite(_pin2, 0);
    }

    void setMaxPower(float power) {
      _max_power = min(power, 1.0f);
    }

    int16_t accelerate(int16_t &speed) {
      if (millis() - last_call >= 10) {
        speed = min((int16_t)MAX_SPEED, speed += ACCELERATION);
        last_call = millis();
      }

      return speed;
    }

    int16_t decelerate(int16_t &speed) {
      if (millis() - last_call >= 10) {
        speed = max(speed -= ACCELERATION, (int16_t)MIN_SPEED);
        last_call = millis();
      }

      return speed;
    }

    void setSpeed(uint16_t speed) {
      if (speed < 0) _speed = max((uint16_t)MIN_SPEED, speed);
      else _speed = min((uint16_t)MAX_SPEED, speed);
    }
};
#include "ServoController.h"
#include <Arduino.h>

ServoController::ServoController() : pwm() {}

void ServoController::begin() {
    pwm.begin();
    pwm.setPWMFreq(50); // Standard servo frequency (50 Hz)
}

void ServoController::set(int servo, int angle) {
    int pulseLen = map(angle, 0, 180, SERVOMIN, SERVOMAX);
    pwm.setPWM(servo, 0, pulseLen);
}

void ServoController::update(int servo,int& angle, int& dr,  bool go) {
    if (go) {
        angle += dr;
        int min = 75;
        int max = 105;
        if (angle > max) {
            dr = -dr;
            set(servo, max);
        }
        if (angle < min) {
            dr = -dr;
            set(servo, min);
        }
    } else {
        set(servo, 90);
        angle = 90;
    }
}

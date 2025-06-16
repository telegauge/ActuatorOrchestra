#ifndef SERVO_CONTROLLER_H
#define SERVO_CONTROLLER_H

#include <Adafruit_PWMServoDriver.h>



class ServoController {
public:
    ServoController();
    void begin();
    void set(int servo, int angle);
    void update( int servo, int& angle, int& dr, bool go);
private:
    Adafruit_PWMServoDriver pwm;
    static const int SERVOMIN = 150;
    static const int SERVOMAX = 600;
};

#endif // SERVO_CONTROLLER_H 
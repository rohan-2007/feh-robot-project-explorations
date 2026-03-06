#include <FEH.h>
#include <Arduino.h>
#include <FEHServo.h>


// Declare things like Motors, Servos, etc. here
// For example:
// FEHMotor leftMotor(FEHMotor::Motor0, 6.0);
// FEHServo servo(FEHServo::Servo0);

#define SERVO_MIN 500
#define SERVO_MAX 2300

#define FULL_LIGHT 0
#define FULL_COVERED 5
// #define BACK_RIGHT_SWITCH 1
// #define FRONT_RIGHT_SWITCH 2
// #define FRONT_LEFT_SWITCH 3

FEHMotor leftMotor(FEHMotor::Motor1, 9.0);
FEHMotor rightMotor(FEHMotor::Motor0, 9.0);
DigitalEncoder left_encoder(FEHIO::Pin9);
DigitalEncoder right_encoder(FEHIO::Pin8);
// DigitalInputPin bump_switch(FEHIO::Pin13);
AnalogInputPin CdS_cell(FEHIO::Pin3);

void ERCMain()
{
    while (true) {
        if (CdS_cell.Value == FULL_LIGHT) {
            leftMotor.SetPercent(40);
            rightMotor.SetPercent(40);
        }
    }
}

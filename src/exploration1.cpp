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
#define BACK_RIGHT_SWITCH 1
#define FRONT_RIGHT_SWITCH 2
#define FRONT_LEFT_SWITCH 3

AnalogInputPin cds(FEHIO::Pin0);
FEHServo servo(FEHServo::Servo7);
FEHMotor right_motor(FEHMotor::Motor0, 9.0);
FEHMotor left_motor(FEHMotor::Motor1, 9.0);

DigitalInputPin front_right(FEHIO::Pin2);
DigitalInputPin front_left(FEHIO::Pin3);
DigitalInputPin back_left(FEHIO::Pin15);
DigitalInputPin back_right(FEHIO::Pin1);


void ERCMain()
{
    // Your code here!

    // Light Sensor:

    // Or just use the TestGUI function
    // TestGUI();

    // while (1) {
    //     LCD.Write(cds.Value());
    //     Sleep(1.0);
    //     LCD.Clear();
    // }

    // Servo Calibration:

    // servo.TouchCalibrate();

    // Control Servo:
    // servo.SetMin(SERVO_MIN);
    // servo.SetMax(SERVO_MAX);

    // while (1) {
    //     int cds_value = cds.Value();

    //     servo.SetDegree(cds_value*(180/5));

    //     Sleep(0.01);
    // }

    // Navigate Course

    bool firstTime = true;
    while (1) {
        // bool right_signal = ;
        if (!front_right.Value()) {
            // while (!(front_left.Value() && front_right.Value())) {
            //     right_motor.SetPercent(25.);
            //     left_motor.SetPercent(25.);
            // }
            while (true) {
                left_motor.SetPercent(25.);
                right_motor.SetPercent(25.);
                if (!front_left.Value() && !front_right.Value()) {
                    left_motor.Stop();
                    right_motor.Stop();
                    break;
                }
            }

            Sleep(0.2);
            while (true) {
                right_motor.SetPercent(-25.);
                if (!back_left.Value() && !back_right.Value()) {
                    left_motor.Stop();
                    right_motor.Stop();
                    break;
                }
            }
            // firstTime = false;
            // if (!front_left.Value() && !front_right.Value()) {
            //     Sleep(0.2);
            //     right_motor.SetPercent(-25.);
            //     firstTime = false;
            // }

            Sleep(0.2);
            while (true) {
                right_motor.SetPercent(25.);
                left_motor.SetPercent(25.);
                if (!front_left.Value() && !front_right.Value()) {
                    left_motor.Stop();
                    right_motor.Stop();
                    break;
                }
            }

            Sleep(0.2);
            while (true) {
                left_motor.SetPercent(-25.);
                if (!back_left.Value() && !back_right.Value()) {
                    left_motor.Stop();
                    right_motor.Stop();
                    break;
                }
            }

            Sleep(0.2);
            while (true) {
                right_motor.SetPercent(25.);
                left_motor.SetPercent(25.);
                if (!front_left.Value() && !front_right.Value()) {
                    left_motor.Stop();
                    right_motor.Stop();
                    break;
                }
            }

            break;
                
        }
    }
    
}

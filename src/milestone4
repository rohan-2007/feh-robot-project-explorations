#include <FEH.h>
#include <Arduino.h>
#include <FEHSERVO.h>
#include <FEHRCS.h>

// Declare things like Motors, Servos, etc. here
// For example:
// FEHMotor leftMotor(FEHMotor::Motor0, 6.0);
// FEHServo servo(FEHServo::Servo0);

#define SERVO_MIN 500
#define SERVO_MAX 2500

#define FULL_LIGHT 0.25
#define FULL_COVERED 5

// Circumference for turn
static const float CIRCUMFERENCE = 22.7765;

// Encoder counts per inch
static const float INCH_TO_COUNTS = 40.49;

//threshold for line following
static const float LINE_THRESHOLD = 3;

// Motor percents
static const int STRAIGHT_PCT = 15;
static const float TURN_45_DEG_COUNTS = 144.0; 
static const int TURN_PCT     = -15;

static const int RED_VALUE = 2.25;
static const int BLUE_VALUE = 0.67;
// #define BACK_RIGHT_SWITCH 1
// #define FRONT_RIGHT_SWITCH 2
// #define FRONT_LEFT_SWITCH 3

FEHMotor leftMotor(FEHMotor::Motor2, 9.0);
FEHMotor rightMotor(FEHMotor::Motor0, 9.0);
FEHServo armServo(FEHServo::Servo0);


DigitalEncoder left_encoder(FEHIO::Pin9);
DigitalEncoder right_encoder(FEHIO::Pin10);
// DigitalInputPin bump_switch(FEHIO::Pin13);
AnalogInputPin CdS_cell(FEHIO::Pin3);

AnalogInputPin leftOpto(FEHIO::Pin14);
AnalogInputPin midOpto(FEHIO::Pin13);
AnalogInputPin rightOpto(FEHIO::Pin12);

enum LineState {
    MIDDLE,
    LEFT,
    RIGHT
};

// Helper function to determine if the sensor is on the line
static inline bool isOnLine(float v) {
        return v > LINE_THRESHOLD;
}

static inline void driveStraight(double distance) {

    int counts = distance * INCH_TO_COUNTS;

    leftMotor.SetPercent(40);
    rightMotor.SetPercent(-40);

    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    while (left_encoder.Counts() < counts);

    leftMotor.Stop();
    rightMotor.Stop();
}

static inline void driveBackwards(double distance) {

    int counts = distance * INCH_TO_COUNTS;

    leftMotor.SetPercent(-40);
    rightMotor.SetPercent(40);

    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    while (left_encoder.Counts() < counts);

    leftMotor.Stop();
    rightMotor.Stop();
}

static inline void turnLeft(int degrees) {

    double limit = (degrees * CIRCUMFERENCE) / 360;
    int counts = limit * INCH_TO_COUNTS;

    leftMotor.SetPercent(TURN_PCT);
    rightMotor.SetPercent(-STRAIGHT_PCT);

    right_encoder.ResetCounts();
    left_encoder.ResetCounts();
    
    while (left_encoder.Counts() < counts);

    leftMotor.Stop();
    rightMotor.Stop();
}

static inline void turnRight(int degrees) {

    double limit = (degrees * CIRCUMFERENCE) / 360;
    int counts = limit * INCH_TO_COUNTS;

    leftMotor.SetPercent(STRAIGHT_PCT);
    rightMotor.SetPercent(-TURN_PCT);

    right_encoder.ResetCounts();
    left_encoder.ResetCounts();
    
    while (left_encoder.Counts() < counts);

    leftMotor.Stop();
    rightMotor.Stop();
}

void ERCMain()
{

    // RCS.InitializeTouchMenu("1020C8WIE");

    while (true) {
        if (CdS_cell.Value() < FULL_LIGHT) {
            armServo.SetMin(SERVO_MIN);
            armServo.SetMax(SERVO_MAX);
            // Move back to hit start button
            driveBackwards(2.0);

            // Pause
            Sleep(1.);

            // Move almost to bucket
            driveStraight(21);

            //turn to face
            turnLeft(45);
            
            // Sleep
            Sleep(1.0);

            // Extend arm
            armServo.SetDegree(80.);

            // Sleep
            Sleep(1.0);

            //go to bucket
            driveStraight(1.);
            
            // Sleep
            Sleep(1.0);

            //retract arm
            armServo.SetDegree(110.);
            
            // Sleep
            Sleep(1.0);

            //back up
            driveBackwards(1.5);

            //turn to face line
            turnRight(180);

            driveStraight(18);
            
        }
    }
}

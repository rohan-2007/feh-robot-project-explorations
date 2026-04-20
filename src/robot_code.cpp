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

static const int RED_VALUE = 2.25;
static const int BLUE_VALUE = 0.67;

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

// #define BACK_RIGHT_SWITCH 1
// #define FRONT_RIGHT_SWITCH 2
// #define FRONT_LEFT_SWITCH 3

FEHMotor leftMotor(FEHMotor::Motor2, 9.0);
FEHMotor rightMotor(FEHMotor::Motor0, 9.0);
FEHServo armServo(FEHServo::Servo0);
FEHServo compostBinServo(FEHServo::Servo1);
                                  

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

static inline void lineUp(float distance) {
    int counts = distance * INCH_TO_COUNTS;

    leftMotor.SetPercent(40);

    left_encoder.ResetCounts();

    while (left_encoder.Counts() < counts);

    leftMotor.Stop();
}

static inline void undoLineUp(float distance) {
    int counts = distance * INCH_TO_COUNTS;

    leftMotor.SetPercent(-40);

    left_encoder.ResetCounts();

    while (left_encoder.Counts() < counts);

    leftMotor.Stop();
}

void ERCMain()
{
    RCS.InitializeTouchMenu("1020C8WIE");

    WaitForFinalAction();

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
            turnLeft(43);

            driveBackwards(1.0);

            // Extend arm
            armServo.SetDegree(48.);
            
            // // Sleep
            // Sleep(1.0);

            // Sleep
            Sleep(1.0);

            //go to bucket
            driveStraight(1.5);
            
            // Sleep
            Sleep(1.0);

            //retract arm
            armServo.SetDegree(110.);
            
            // Sleep
            Sleep(1.0);

            //back up
            driveBackwards(1);

            turnRight(45);

            //back up
            driveBackwards(19);

            turnRight(50);

            driveStraight(17);

            driveStraight(19);

            //turn to face line
            turnLeft(90);

            driveStraight(6);

            turnRight(105);

            driveStraight(15);

            Sleep(1.0);

            // Release apple bucket
            armServo.SetDegree(40);

            Sleep(1.0);
            
            driveBackwards(12);

            turnLeft(45);

            armServo.SetDegree(110);

            //start the lever code
            int leverCount = RCS.GetLever();
            // int leverCount = 1;

            if (leverCount == 0) {
                driveStraight(6);
                turnLeft(90);
                driveStraight(4.5);
                turnRight(90);
                driveStraight(5);
                armServo.SetDegree(30);
            }

            if (leverCount == 1) {
                driveStraight(11);
                armServo.SetDegree(30);
            }

            if (leverCount == 2) {
                driveStraight(6);
                turnRight(90);
                driveStraight(4.5);
                turnLeft(90);
                driveStraight(5);
                armServo.SetDegree(30);
            }

            Sleep(1.0);

            armServo.SetDegree(50);

            // MOVE BACKWARDS TO FLIP LEVER UP
            Sleep(1.0);

            driveBackwards(7);

            Sleep(4.0);

            armServo.SetDegree(20);

            Sleep(1.0);

            driveStraight(7);

            Sleep(1.0);

            armServo.SetDegree(60);

            Sleep(1.0);

            armServo.SetDegree(30);

            Sleep(1.0);
            
            driveBackwards(12);

            Sleep(1.0);

            armServo.SetDegree(135);

            Sleep(1.0);

            turnRight(45);

            driveBackwards(6);

            turnLeft(90);

            driveStraight(9);

            driveBackwards(5);

            turnRight(90);

            driveStraight(6.5);

            turnLeft(90);

            // Move to humidifier light
            driveStraight(6);

            // Read the humidifier light color
            // while (CdS_cell.Value() != RED_VALUE && CdS_cell.Value() != BLUE_VALUE);

            // if (CdS_cell.Value() == RED_VALUE) {
            //     LCD.Write("RED");
            //     turnRight(90);
            //     driveStraight(1.0);
            //     turnLeft(90);
            // } else if (CdS_cell.Value() == BLUE_VALUE) {
            //     LCD.Write("BLUE");
            //     turnLeft(90);
            //     driveStraight(1.0);
            //     turnRight(90);
            // }
            driveStraight(5.0);

            // Make contact with the humidifier button
            Sleep(1.0);

            // Drive to compost bin
            driveBackwards(22.0);

            driveStraight(5);

            turnLeft(90);

            // driveStraight(12);

            driveStraight(31);

            Sleep(1.);

            turnRight(135);

            driveBackwards(2.0);

            Sleep(1.0);
        }
    }
}

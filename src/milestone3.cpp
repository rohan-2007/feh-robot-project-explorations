#include <FEH.h>
#include <Arduino.h>
#include <FEHServo.h>
#include <FEHRCS.h>

// Declare things like Motors, Servos, etc. here
// For example:
// FEHMotor leftMotor(FEHMotor::Motor0, 6.0);
// FEHServo servo(FEHServo::Servo0);

#define SERVO_MIN 500
#define SERVO_MAX 2300

#define FULL_LIGHT 0.25
#define FULL_COVERED 5

// Circumference for turn
static const float CIRCUMFERENCE = 37.699;

// Encoder counts per inch
static const float INCH_TO_COUNTS = 40.49;

//threshold for line following
static const float LINE_THRESHOLD = 3;

// Motor percents
static const int STRAIGHT_PCT = 15;
static const float TURN_45_DEG_COUNTS = 144.0; 
static const int TURN_PCT     = 0;

static const int RED_VALUE = 2.25;
static const int BLUE_VALUE = 0.67;
// #define BACK_RIGHT_SWITCH 1
// #define FRONT_RIGHT_SWITCH 2
// #define FRONT_LEFT_SWITCH 3

FEHMotor leftMotor(FEHMotor::Motor2, 9.0);
FEHMotor rightMotor(FEHMotor::Motor0, 9.0);
DigitalEncoder left_encoder(FEHIO::Pin9);
DigitalEncoder right_encoder(FEHIO::Pin8);
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
    rightMotor.SetPercent(40);

    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    while (left_encoder.Counts() < counts && right_encoder.Counts() < counts);

    leftMotor.Stop();
    rightMotor.Stop();
}

static inline void driveBackwards(double distance) {

    int counts = distance * INCH_TO_COUNTS;

    leftMotor.SetPercent(-40);
    rightMotor.SetPercent(-40);

    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    while (left_encoder.Counts() < counts && right_encoder.Counts() < counts);

    leftMotor.Stop();
    rightMotor.Stop();
}

static inline void turnLeft(int degrees) {

    double limit = (degrees * CIRCUMFERENCE) / 360;
    int counts = limit * INCH_TO_COUNTS;

    leftMotor.SetPercent(TURN_PCT);
    rightMotor.SetPercent(STRAIGHT_PCT);

    right_encoder.ResetCounts();
    left_encoder.ResetCounts();
    
    while (right_encoder.Counts() < counts);

    leftMotor.Stop();
    rightMotor.Stop();
}

static inline void turnRight(int degrees) {

    double limit = (degrees * CIRCUMFERENCE) / 360;
    int counts = limit * INCH_TO_COUNTS;

    leftMotor.SetPercent(STRAIGHT_PCT);
    rightMotor.SetPercent(TURN_PCT);

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
            // Move back to hit start button
            driveBackwards(2.0);

            // Pause
            Sleep(1.);

            // Move forward to start location
            driveStraight(2.0);
            
            // Pause
            Sleep(1.);

            // Turn right 45 degrees
            turnRight(45);

            // Pause
            Sleep(1.);

            // Drive up the ramp
            driveStraight(31.0);

            // leftMotor.SetPercent(40);
            // rightMotor.SetPercent(40);

            // left_encoder.ResetCounts();
            // right_encoder.ResetCounts();

            // while (left_encoder.Counts() < 1256 && right_encoder.Counts() < 1256);
        
            // Follow the line for 27 inches
            // leftMotor.SetPercent(40);
            // rightMotor.SetPercent(40);

            // left_encoder.ResetCounts();
            // right_encoder.ResetCounts();

            // while (left_encoder.Counts() < 1073 && right_encoder.Counts() < 1117) {
            //     LineState state = MIDDLE;
            //     float l = leftOpto.Value(); 
            //     float m = midOpto.Value();
            //     float r = rightOpto.Value();

            //     bool L = isOnLine(l);
            //     bool M = isOnLine(m);
            //     bool R = isOnLine(r);

            //     switch (state) {
            //         case MIDDLE:

            //             driveStraight();

            //             if (R && !M) {
            //                 state = RIGHT;
            //             }
            //             else if (L && !M) {
            //                 state = LEFT;
            //             }
            //             break;

            //         case RIGHT:
            //             turnRight();
            //             if (M) {
            //                 state = MIDDLE;
            //             }
            //             else if (L && !R) {
            //                 state = LEFT;
            //             }
            //             break;

            //         case LEFT:
            //             turnLeft();

            //             if (M) {
            //                 state = MIDDLE;
            //             }
            //             else if (R && !L) {
            //                 state = RIGHT;
            //             }
            //             break;

            //         default:
            //             leftMotor.Stop();
            //             rightMotor.Stop();
            //             state = MIDDLE;
            //             break;
            //     }

            //     Sleep(0.01);
            // }

            // If optosensor doesn't work:
            // Drive forward roughly 12 inches
            driveStraight(6);

            // Pause
            Sleep(0.1);

            // Turn 90 degrees left
            turnLeft(90);

            // Pause
            Sleep(0.1);


            // // Read the humidifier light color
            // while 
            // driveBackwards(-20.0);(CdS_cell.Value() != RED_VALUE && CdS_cell.Value() != BLUE_VALUE);

            // if (CdS_cell.Value() == RED_VALUE) {
            //     LCD.Write("RED");
            //     turnRight(90);
            //     turnLeft(90);
            // } else if (CdS_cell.Value() == BLUE_VALUE) {
            //     LCD.Write("BLUE");
            //     turnLeft(90);
            //     turnRight(90);
            // }

            // leftMotor.SetPercent(40);
            // rightMotor.SetPercent(40);

            // while (RCS.isWindowOpen() == 0);

            // leftMotor.Stop();
            // rightMotor.Stop();

            driveStraight(18.0);
            
            
        }
    }
}

#include <FEH.h>
#include <FEHMotor.h>
#include <FEHIO.h>

//motors
FEHMotor rightMotor(FEHMotor::Motor0, 9.0);
FEHMotor leftMotor(FEHMotor::Motor1, 9.0);

//opto sensors
AnalogInputPin leftOpto(FEHIO::Pin14);
AnalogInputPin midOpto(FEHIO::Pin13);
AnalogInputPin rightOpto(FEHIO::Pin12);

//Line following parameters
static const float LINE_THRESHOLD = 3;

// Motor percents
static const int STRAIGHT_PCT = 15;
static const int TURN_PCT     = 0;

// State machine for line following
enum LineState {
    MIDDLE,
    LEFT,
    RIGHT
};

// Helper function to determine if the sensor is on the line
static inline bool isOnLine(float v) {
        return v > LINE_THRESHOLD;
}

static inline void driveStraight() {
    leftMotor.SetPercent(STRAIGHT_PCT);
    rightMotor.SetPercent(-15);
}

static inline void turnLeft() {
    leftMotor.SetPercent(TURN_PCT);
    rightMotor.SetPercent(-15);
}

static inline void turnRight() {
    leftMotor.SetPercent(STRAIGHT_PCT);
    rightMotor.SetPercent(0);
}

void ERCMain()
{
    LineState state = MIDDLE;
    while (true) {
        float l = leftOpto.Value(); 
        float m = midOpto.Value();
        float r = rightOpto.Value();

        bool L = isOnLine(l);
        bool M = isOnLine(m);
        bool R = isOnLine(r);

        switch (state) {
            case MIDDLE:

                driveStraight();

                if (R && !M) {
                    state = RIGHT;
                }
                else if (L && !M) {
                    state = LEFT;
                }
                break;

            case RIGHT:
                turnRight();
                if (M) {
                    state = MIDDLE;
                }
                else if (L && !R) {
                    state = LEFT;
                }
                break;

            case LEFT:
                turnLeft();

                if (M) {
                    state = MIDDLE;
                }
                else if (R && !L) {
                    state = RIGHT;
                }
                break;

            default:
                leftMotor.Stop();
                rightMotor.Stop();
                state = MIDDLE;
                break;
        }

        Sleep(0.01);
    }
}



#include <Servo.h>
static Servo ventilationValve;

#define VALVE_ANGLE_CLOSED 45
#define VALVE_ANGLE_OPEN 135
#define VALVE_ANGLE_MID 90
#define VALVE_MOVE_TIMER 7 * ( VALVE_ANGLE_OPEN - VALVE_ANGLE_CLOSED )

void MoveVentliation()
{
	ventilationValve.attach(SERVO_PIN);
	int valveState = ventilationValve.read();
	valveState < VALVE_ANGLE_MID ? valveState = VALVE_ANGLE_OPEN : valveState = VALVE_ANGLE_CLOSED; //inverse
	ventilationValve.write(valveState);
	delay(VALVE_MOVE_TIMER);
	ventilationValve.detach();
	valveState == VALVE_ANGLE_OPEN ? valvePosition = VALVE_OPEN : valvePosition = VALVE_CLOSED;
	Serial.printf("\nValve is now %s.\r\n", valvePosition == VALVE_OPEN ? "open" : "closed");
}

void MoveVentliation(VALVE valvePos)
{
	ventilationValve.attach(SERVO_PIN);
	int angle = map(valvePos, VALVE_CLOSED, VALVE_OPEN, VALVE_ANGLE_CLOSED, VALVE_ANGLE_OPEN);
	ventilationValve.write(angle);
	delay(VALVE_MOVE_TIMER);
	ventilationValve.detach();
	valvePosition = valvePos;
	Serial.printf("\nValve is now %s.\r\n", valvePos == VALVE_CLOSED ? "closed" : "open");
}


void initServo()
{
	MoveVentliation();
	MoveVentliation();
}

#include <VarSpeedServo.h>

VarSpeedServo doorServo;
VarSpeedServo handServo;

int selectedMove = 0;

int LED_PIN = 5;
int SWITCH_PIN = 8;
int DOOR_PIN = 9;
int HAND_PIN = 10;

int HAND_OUT= 30;
int HAND_IN = 180;
int DOOR_OPENED = 55;
int DOOR_CLOSED = 120;
int HAND_HALFWAY = 90;

int FULL_SPEED = 200;
bool attached = false;

void setup()
{
	Serial.begin(9600);
	pinMode(SWITCH_PIN, INPUT_PULLUP);

	initializeScenarios();
}


// Array of function pointers
void (*scenarios[30])();
const char *scenariosNames[30];

int scenariosCount = 10;
void initializeScenarios() {
	scenarios[0] = closeQuickly;
	scenarios[1] = closeQuicklyWithDelayOnSwitchAndReturnSlowly;
	scenarios[2] = pauseBeforeClosing;
	scenarios[3] = peekBeforeClosing;
	scenarios[4] = angryDoor;
	scenarios[5] = keepHandOnSwitchAndBounce;
	scenarios[6] = hitSwitchMultipleTimes;
	scenarios[7] = reallyReallySlowHand;
	scenarios[8] = crazyDoor;
	scenarios[9] = closeAndHideAndInsist;

	scenariosNames[0] = "closeQuickly";
	scenariosNames[1] = "closeQuicklyWithDelayOnSwitchAndReturnSlowly";
	scenariosNames[2] = "pauseBeforeClosing";
	scenariosNames[3] = "peekBeforeClosing";
	scenariosNames[4] = "angryDoor";
	scenariosNames[5] = "keepHandOnSwitchAndBounce";
	scenariosNames[6] = "hitSwitchMultipleTimes";
	scenariosNames[7] = "reallyReallySlowHand";
	scenariosNames[8] = "crazyDoor";
	scenariosNames[9] = "closeAndHideAndInsist";
}

void loop() {
	// test mode: set to move number to test only one selected move
	// (set to -1 to run normally i.e: roundrobbin on amm moves)
	int testMove = -1;

	if(digitalRead(SWITCH_PIN) == LOW) {
		if (!attached) {
			delay(1000);
			doorServo.attach(DOOR_PIN);
			handServo.attach(HAND_PIN);
			attached = true;
		}
		if (testMove != -1) {
			selectedMove = testMove;
		}
		runScenario(selectedMove);
		if (testMove == -1) {
			selectedMove = (selectedMove + 1) % (scenariosCount + 1);
		}
	}
}

/* . . . . . . . . . . . . . . Basic Moves . . . . . . . . . . . . . . . . */

void moveDoor(int openPercentage, int speed, bool sync) {
	int target = (int)(-0.65 * openPercentage + 120);
	doorServo.write(target, speed, sync);
}

void moveHand(int outPercentage, int speed, bool sync) {
	int target = (int)(-1.5 * outPercentage + 180);
	handServo.write(target, speed, sync);
}

/* . . . . . . . . . . . . . . . . Scenarios . . . . . . . . . . . . . . . . */

void closeQuickly() {
	moveDoor(100, FULL_SPEED, true);
	moveHand(110, FULL_SPEED, true);
	delay(300);
	moveHand(0, FULL_SPEED, true);
	moveDoor(0, FULL_SPEED, true);
}

void closeQuicklyWithDelayOnSwitchAndReturnSlowly() {
	delay(200);
	moveDoor(100, FULL_SPEED, true);
	moveHand(110, FULL_SPEED, true);
	delay(1000);
	moveHand(0, 40, true);
	delay(700);
	moveDoor(0, 70, true);
}

void pauseBeforeClosing() {
	delay(2000);
	moveDoor(100, 30, true);
	delay(1000);
	moveHand(80, 40, true);
	delay(700);
	moveHand(110, FULL_SPEED, true);
	delay(1000);
	moveHand(0, 40, true);
	delay(700);
	moveDoor(0, 70, true);
}

void peekBeforeClosing() {
	delay(200);
	moveDoor(70, FULL_SPEED, true);
	delay(2500);
	moveDoor(0, FULL_SPEED, true);
	delay(2000);
	moveDoor(100, FULL_SPEED, true);
	delay(200);
	moveHand(110, FULL_SPEED, true);
	delay(200);
	moveHand(0, FULL_SPEED, true);
	delay(400);
	moveDoor(0, FULL_SPEED, true);
}

void angryDoor() {
	delay(200);
	for (int i = 0 ; i < 20 ; i++) {
		moveDoor(30, FULL_SPEED, true);
		delay(50);
		moveDoor(25, FULL_SPEED, true);
		delay(50);
	}

	moveDoor(100, FULL_SPEED, true);
	delay(2000);
	moveHand(60, 30, true);
	moveHand(110, FULL_SPEED, true);
	delay(300);
	moveHand(60, FULL_SPEED, true);
	moveHand(0, 30, true);
	moveDoor(50, FULL_SPEED, true);
	moveDoor(0, 30, true);
}

void keepHandOnSwitchAndBounce() {
	delay(500);
	moveDoor(100, 100, true);
	delay(100);
	moveHand(110, FULL_SPEED, true);
	delay(700);

	for (int i = 0 ; i < 6 ; i++) {
		moveDoor(40, 85, true);
		moveDoor(90, 85, true);
	}

	moveDoor(100, FULL_SPEED, true);
	delay(1000);
	moveHand(0, FULL_SPEED, true);
	delay(200);
	moveDoor(0, FULL_SPEED, true);
}

void hitSwitchMultipleTimes() {
	delay(200);
	moveDoor(100, FULL_SPEED, true);
	delay(100);
	moveHand(110, FULL_SPEED, true);
	delay(1000);

	for (int i = 0 ; i < 6 ; i++) {
		moveHand(80, FULL_SPEED, true);
		delay(40);
		moveHand(110, FULL_SPEED, true);
	}
	delay(1000);

	moveHand(0, FULL_SPEED, true);
	delay(1000);
	moveDoor(0, FULL_SPEED, true);

	// peek
	delay(1000);
	moveDoor(70, FULL_SPEED, true);
	delay(2500);
	moveDoor(0, FULL_SPEED, true);
}

void reallyReallySlowHand() {
	delay(200);
	moveDoor(100, FULL_SPEED, true);
	moveHand(80, 10, true);

	delay(500);
	moveHand(110, FULL_SPEED, true);
	moveHand(0, FULL_SPEED, true);
	moveDoor(0, FULL_SPEED, true);
}

void crazyDoor() {
	delay(200);
	for (int i = 0 ; i < 8 ; i++) {
		moveDoor(90, FULL_SPEED, true);
		delay(30);
		moveDoor(20, FULL_SPEED, true);
	}
	moveDoor(0, FULL_SPEED, true);
	delay(1000);
	moveDoor(100, FULL_SPEED, true);
	moveHand(110, FULL_SPEED, true);
	delay(300);
	moveHand(0, FULL_SPEED, true);
	moveDoor(0, FULL_SPEED, true);
}

void closeAndHideAndInsist() {
	delay(200);
	moveDoor(100, FULL_SPEED, true);
	moveHand(110, FULL_SPEED, true);
	delay(200);
	moveDoor(55, FULL_SPEED, true);
	delay(1000);
	moveDoor(80, FULL_SPEED, true);
	for (int i = 0 ; i < 3 ; i++) {
		moveHand(60, FULL_SPEED, true);
		delay(50);
		moveHand(110, FULL_SPEED, true);
		delay(50);
	}
	moveDoor(55, FULL_SPEED, true);
	delay(2000);

	moveDoor(100, FULL_SPEED, true);
	delay(500);
	moveHand(0, FULL_SPEED, true);
	delay(300);
	moveDoor(0, FULL_SPEED, true);
}

void runScenario(int index) {
	Serial.print("Running scenario ");
	Serial.println(scenariosNames[index]);
	(*scenarios[index])();
}


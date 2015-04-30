#include "LEDStrip.h"

// Pins
const int ledPin = 13;
const int redPin = 5;
const int greenPin = 6;
const int bluePin = 3;

const int lightSensorPin = A5;
const int motionSensorPin = 12;

// Command interpretation
const byte commandBuffer = 20;        			// Max size of the command array
byte command[commandBuffer];          			// Holds the actual command
int commandPointer = 0;               			// Points to the next free index in command aray
boolean isReceivingCommand = false;

// Command API identifier (Remote Control Collection app)
const byte CMD_START = 60;         				// Ascii for "<"
const byte CMD_END = 62;           				// Ascii for ">"
const byte CMD_APIv3 = 127; 
const byte CMD_KEYBOARD = 21; 
const byte CMD_KEYBOARD_KEYCODE = 2;
const byte CMD_KEYBOARD_KEYCODE_CUSTOM = 221;	// Holds the first custom key code
const byte CMD_ACTION_DOWN = 0;
const byte CMD_ACTION_UP = 1;
const byte CMD_ACTION_CLICK = 2;

const int INTERVAL_LIGHT_CHECK = 1000;
const int INTERVAL_MOTION_CHECK = 100;
const int INTERVAL_SENSOR_CHECK = 1000;
const int INTERVAL_FADE_STEP = 30;

// Light sensor
const int LIGHT_THRESHOLD = 50;				// Used to prevent self-triggering the light sensor
const int LIGHT_NIGHT = 300;
const int LIGHT_DUSK = 400;
const int LIGHT_CLOUDY = 500;
const int LIGHT_DAY = 600;

boolean useLightSensor = true;				// Auto toggle the light if the room is dark
unsigned long timeLastLightCheck = 0;
unsigned long timeLastLightChange = 0;
int lastLightSensorValue = 1024;

// Motion sensor
const int RECENT_MOTION_TIMEOUT = 300;		// Timeout in seconds
boolean useMotionSensor = true;				// Auto toggle the light if there's motion
unsigned long timeLastMotionCheck = 0;
unsigned long timeLastMotionChange = 0;
unsigned long timeLastMotionDetected = 0;

// General
unsigned long timeLastSensorCheck = 0;
unsigned long timeLastFadeStep = 0;

// Instance of the LED stripe
LEDStrip led(redPin, greenPin, bluePin);

void setup() {
	Serial.begin(9600);
	Serial.println("Up and running");

	pinMode(ledPin, OUTPUT);
	pinMode(motionSensorPin, INPUT);

	led.turnOn();
}

void loop() {
	unsigned long timeNow = millis();

	// Listen to serial commands
	readSerial();

	// Perform fading steps
	if (led.shouldFadeToColor) {
		if (timeLastFadeStep + INTERVAL_FADE_STEP < timeNow) {
			led.fadeToColorStep();
			timeLastFadeStep = timeNow;
		}		
	}

	// Check the light sensor
	if (useLightSensor) {
		if (timeLastLightCheck + INTERVAL_LIGHT_CHECK < timeNow) {
			checkLightSensor();
			timeLastLightCheck = timeNow;
		}
	}

	// Check the motion sensor
	if (useMotionSensor) {
		if (timeLastMotionCheck + INTERVAL_MOTION_CHECK < timeNow) {
			checkMotionSensor();
			timeLastMotionCheck = timeNow;
		}
	}

	// Process sensor data
	if (led.masterOn) {
		if (timeLastSensorCheck + INTERVAL_SENSOR_CHECK < timeNow) {
			processSensorData();
			timeLastSensorCheck = timeNow;
		}
	}
}

void processSensorData() {
	// Log sensor info
	if (true) {
		Serial.print("Seconds since last motion: ");
		Serial.print(getSecondsSinceLastMotion());
		Serial.print(" Last light sensor value: ");
		Serial.println(lastLightSensorValue);
	}

	// Toggle the light depending on sensor data
	if (hasRecentlyDetectedActivity() && shouldLightenUpRoom()) {
		// Lighten up the room
		if (!led.isLightningUp()) {
			led.fadeToColor(led.lastManuallySetColor);
		}
	} else {
		// Turn the light off
		if (led.isLightningUp()) {
			led.fadeToColor(led.BLACK);
		}
	}
}
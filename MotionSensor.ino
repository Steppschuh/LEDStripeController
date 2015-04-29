void checkMotionSensor() {
	boolean motionDetected = getMotionSensorValue();

	if (motionDetected) {
		timeLastMotionDetected = millis();
	}
}

boolean getMotionSensorValue() {
	return digitalRead(motionSensorPin) == HIGH;
}

unsigned long getSecondsSinceLastMotion() {
	return (millis() - timeLastMotionDetected) / 1000;
}

boolean hasRecentlyDetectedActivity() {
	return getSecondsSinceLastMotion() < RECENT_MOTION_TIMEOUT;
}
void checkLightSensor() {
	lastLightSensorValue = getLightSensorValue();
}

int getLightSensorValue() {
	return analogRead(lightSensorPin);
}

boolean shouldLightenUpRoom() {
	if (led.isLightningUp()) {
		return lastLightSensorValue - LIGHT_THRESHOLD < LIGHT_DUSK;
	} else {
		return lastLightSensorValue + LIGHT_THRESHOLD < LIGHT_DUSK;
	}
}
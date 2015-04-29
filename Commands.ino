
void readSerial() {
  if (Serial.available() > 0) {
    int incomingByte = 0;
    incomingByte = Serial.read();

    switch (incomingByte) {
      case CMD_START:
        isReceivingCommand = true;
        commandPointer = 0;
        break;
      case CMD_END:
        isReceivingCommand = false;
        parseCommand(command);
        //Serial.println("Command received");
        break;
      default:
        if (isReceivingCommand) {
          command[commandPointer] = incomingByte;
          commandPointer++;
          if (commandPointer >= commandBuffer) {
            // Command buffer is full, can't parse next byte
            commandPointer = 0;
            isReceivingCommand = false;
            Serial.println("Command buffer overflow!");
          }
        } 
        else {
          // Commad didn't start with CMD_START
          unknownCommandReceived();
        }
    }
  }
}

void parseCommand(byte *command) {
  switch (command[0]) {
      case CMD_APIv3:
        parseAPIv3Command(command);
        break;
      default:
        unknownCommandReceived();
        Serial.print(command[0], DEC);
  }
}

void parseAPIv3Command(byte *command) {
  switch (command[1]) {
      case CMD_KEYBOARD:
        parseKeyboardCommand(command);
        break;
      default:
        unknownCommandReceived();
        Serial.print(command[1], DEC);
  }
}

void parseKeyboardCommand(byte *command) {
  switch (command[7]) {
    case CMD_KEYBOARD_KEYCODE_CUSTOM:
      led.turnOn();
      break;
    case CMD_KEYBOARD_KEYCODE_CUSTOM + 1:
      led.turnOff();
      break;
    case CMD_KEYBOARD_KEYCODE_CUSTOM + 2:
      
      break;
    default:
      unknownCommandReceived();
  }
}

void unknownCommandReceived() {
  Serial.println("Unknown command received");
}
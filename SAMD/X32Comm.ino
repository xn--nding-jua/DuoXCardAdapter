/*
  This file contains code to emulate an X32 to communicate with the Behringer ExpansionCards
*/

void handleXCard1Communication() {
  String msg;
  if (SerialCard1.available() > 0) {
    msg = SerialCard1.readStringUntil('#');
    Serial.println("XCard #1: " + msg);
  }
}

void handleXCard2Communication() {
  String msg;
  if (SerialCard2.available() > 0) {
    msg = SerialCard2.readStringUntil('#');
    Serial.println("XCard #2: " + msg);
  }
}

void x32InitCard1() {
  delay(1000); // X32 would take some time to boot. So we have to be patient and wait here some time to let X-Card initialize
  Serial.println(F("   waiting... 3s"));
  delay(1000); // X32 would take some time to boot. So we have to be patient and wait here some time to let X-Card initialize
  Serial.println(F("   waiting... 2s"));
  delay(1000); // X32 would take some time to boot. So we have to be patient and wait here some time to let X-Card initialize
  Serial.println(F("   waiting... 1s"));
  delay(1000); // X32 would take some time to boot. So we have to be patient and wait here some time to let X-Card initialize
  Serial.println(F("   wait for CardReady-Signal..."));
  
  // wait until card is ready (that's when CardRead-Signal is high)
  while(!digitalRead(CARD_READY_PIN)) {
    delay(10);
  }
  delay(100);

  // now initialize the card #1
  Serial1Flush(); // flush serial for card #1
  String ans;
  Serial.println(F("   Initializing Card #1..."));
  Serial.println(F("       Sending *8I#..."));
  SerialCard1.print("*8I#"); // send identify-command
  ans = SerialCard1.readStringUntil('#'); // ans should be *8X-UREC:A:12#
  while (ans.indexOf("*8X") == -1) {
    Serial.println("       Ignoring response " + ans);
    ans = SerialCard1.readStringUntil('#'); // ans should be *8X-UREC:A:12#
  }
  Serial.println("    Found " + ans);

  delay(250);
/*
  Serial.println(F("       Sending *8R#..."));
  SerialCard1.print("*8R#"); // send second identify-command
  ans = SerialCard1.readStringUntil('#'); // ans should be *8X-UREC:A:12#
  while (ans.indexOf("*8X") == -1) {
    Serial.println("       Ignoring response " + ans);
    ans = SerialCard1.readStringUntil('#'); // ans should be *8X-UREC:A:12#
  }
  Serial.println("    Found " + ans);

  delay(250);
*/

  // now send a bunch of initialization commands - don't know for what - just doing what the X32 is doing
  Serial.println(F("       Sending long Init-Command..."));
  for (uint8_t i=0; i<8; i++) {
    SerialCard1.print("*1G" + String(i) + "0000#");
    SerialCard1.print("*1G" + String(i) + "0000#");
  }
  for (uint8_t i=0; i<8; i++) {
    SerialCard1.print("*0G" + String(i) + "0000#");
    SerialCard1.print("*0G" + String(i) + "0000#");
  }
  for (uint8_t i=0; i<8; i++) {
    SerialCard1.print("*3G" + String(i) + "0000#");
    SerialCard1.print("*3G" + String(i) + "0000#");
  }
  for (uint8_t i=0; i<8; i++) {
    SerialCard1.print("*2G" + String(i) + "0000#");
    SerialCard1.print("*2G" + String(i) + "0000#");
  }

  delay(1000);

  // set Card to USB-Mode
  Serial.println(F("       Set Card-Mode to USB via *8C80U#..."));
  SerialCard1.print("*8C80U#"); // 32 channels from USB
  Serial.println(F("   Done."));
}


void x32InitCard2() {
  // now initialize the card #1
  String ans;
  Serial.println(F("   Initializing Card #2..."));
  Serial.println(F("       Sending *8I#..."));

  // if we can not communicate with card #2, exit
  Serial2Flush(); // flush serial for card #2
  SerialCard2.print("*8I#"); // send identify-command

  if (!waitForCard2Response(2)) {
    Serial.println(F("   Could not find Card #2 - sorry :-("));
    return;
  }

  ans = SerialCard2.readStringUntil('#'); // ans should be *8X-UREC:A:12#
  while (ans.indexOf("*8X") == -1) {
    Serial.println("       Ignoring response " + ans);
    ans = SerialCard2.readStringUntil('#'); // ans should be *8X-UREC:A:12#
  }
  Serial.println("    Found " + ans);

  delay(250);
/*
  Serial.println(F("       Sending *8R#..."));
  SerialCard2.print("*8R#"); // send second identify-command
  ans = SerialCard2.readStringUntil('#'); // ans should be *8X-UREC:A:12#
  while (ans.indexOf("*8X") == -1) {
    Serial.println("       Ignoring response " + ans);
    ans = SerialCard2.readStringUntil('#'); // ans should be *8X-UREC:A:12#
  }
  Serial.println("    Found " + ans);

  delay(250);
*/

  // now send a bunch of initialization commands - don't know for what - just doing what the X32 is doing
  Serial.println(F("       Sending long Init-Command..."));
  for (uint8_t i=0; i<8; i++) {
    SerialCard2.print("*1G" + String(i) + "0000#");
    SerialCard2.print("*1G" + String(i) + "0000#");
  }
  for (uint8_t i=0; i<8; i++) {
    SerialCard2.print("*0G" + String(i) + "0000#");
    SerialCard2.print("*0G" + String(i) + "0000#");
  }
  for (uint8_t i=0; i<8; i++) {
    SerialCard2.print("*3G" + String(i) + "0000#");
    SerialCard2.print("*3G" + String(i) + "0000#");
  }
  for (uint8_t i=0; i<8; i++) {
    SerialCard2.print("*2G" + String(i) + "0000#");
    SerialCard2.print("*2G" + String(i) + "0000#");
  }

  delay(1000);

  // set Card to USB-Mode
  Serial.println(F("       Set Card-Mode to USB via *8C80U#..."));
  SerialCard2.print("*8C80U#"); // 32 channels from USB
  Serial.println(F("   Done."));
}

bool waitForCard2Response(uint8_t maxSeconds) {
  uint8_t cardTimeoutCounter = 0;
  while ((SerialCard2.available() < 4) && (cardTimeoutCounter < maxSeconds)) { // we expect either *8BE# or *8X-UREC:A:12#
    Serial.println(F("       Waiting for response..."));
    cardTimeoutCounter += 1;
    delay(1000);
  }
  Serial.println("       We received " + String(SerialCard2.available()) + "bytes from card...");
  return (SerialCard2.available() >= 4);
}
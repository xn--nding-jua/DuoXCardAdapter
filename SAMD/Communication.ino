/*
  Control the Behringer X32 Cards using emulated UART-connection via USB
  
  Example: List all sessions on SD-Card of a connected X-LIVE, send the following commands:
  card1:mode:card
  card1:select:sdcard@0
  card1:toc?
  
  To switch to USB use this command:
  card1:mode:usb
*/

// USB-CMD-Receiver
void handleUSBCommunication() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n'); // we are using both CR/LF but we have to read until LF
    command.trim();
    Serial.println(executeCommand(command));
  }
}

// command-interpreter
String executeCommand(String Command) {
  String Answer;

  if (Command.length()>2){
    // we got a new command. Lets find out what we have to do today...

    if (Command.indexOf(F("card1:mode:usb")) > -1){
      Serial1Flush();
      SerialCard1.print("*8C80U#");
      Answer = SerialCard1.readStringUntil('#');
    }else if (Command.indexOf(F("card1:mode:card")) > -1){
      Serial1Flush();
      SerialCard1.print("*8C80C#");
      Answer = SerialCard1.readStringUntil('#');
    }else if (Command.indexOf(F("card1:session@")) > -1){
      Serial1Flush();
      SerialCard1.print("*9B" + Command.substring(Command.indexOf("@")+1) + "#"); // sometimes a trailing Q#
      Answer = SerialCard1.readStringUntil('#');
    }else if (Command.indexOf(F("card1:play")) > -1){
      Serial1Flush();
      SerialCard1.print("*9D#");
      Answer = SerialCard1.readStringUntil('#');
    }else if (Command.indexOf(F("card1:pause")) > -1){
      Serial1Flush();
      SerialCard1.print("*9E#");
      Answer = SerialCard1.readStringUntil('#');
    }else if (Command.indexOf(F("card1:stop")) > -1){
      Serial1Flush();
      SerialCard1.print("*9F#");
      Answer = SerialCard1.readStringUntil('#');
    }else if (Command.indexOf(F("card1:toc?")) > -1){
      Serial1Flush();
      SerialCard1.print("*9AF#"); // request first entry
      String ans = SerialCard1.readStringUntil('#'); // *ASNxxxxxxxx
      String toc = ans.substring(5); // add without *ASN

      while ((ans.indexOf("*9AEN00") == -1) && ((ans.indexOf("*9ASF") == 0) || (ans.indexOf("*9ASN") == 0))) {
        // we have more to read
        Serial1Flush();
        SerialCard1.print("*9AN#"); // request follow-up-titles
        ans = SerialCard1.readStringUntil('#'); // *ASNxxxxxxxx
        if (ans.indexOf("*9ASN") == 0) {
          toc = toc + " " + ans.substring(5); // add without *ASN
        }else{
          toc = toc + " EOT";
        }
      }
      Answer = toc;
    }else if (Command.indexOf(F("card1:marker?")) > -1){
      Serial1Flush();
      SerialCard1.print("*9C00#");
      String ans = SerialCard1.readStringUntil('#'); // *ASNxxxxxxxx
      Answer = ans;
    }else if (Command.indexOf(F("card1:size")) > -1){
      Serial1Flush();
      SerialCard1.print("*9G0#");
      Answer = SerialCard1.readStringUntil('#');
    }else if (Command.indexOf(F("card1:select:sdcard@")) > -1){
      // card1:select:sdcard@0 or card1:select:sdcard@1
      Serial1Flush();
      SerialCard1.print("*9R" + Command.substring(Command.indexOf("@")+1) + "#");
      Answer = SerialCard1.readStringUntil('#');




    }else if (Command.indexOf(F("card2:mode:usb")) > -1){
      Serial2Flush();
      SerialCard2.print("*8C80U#");
      Answer = SerialCard2.readStringUntil('#');
    }else if (Command.indexOf(F("card2:mode:card")) > -1){
      Serial2Flush();
      SerialCard2.print("*8C80C#");
      Answer = SerialCard2.readStringUntil('#');
    }else if (Command.indexOf(F("card2:session@")) > -1){
      Serial2Flush();
      SerialCard2.print("*9B" + Command.substring(Command.indexOf("@")+1) + "#"); // sometimes a trailing Q#
      Answer = SerialCard2.readStringUntil('#');
    }else if (Command.indexOf(F("card2:play")) > -1){
      Serial2Flush();
      SerialCard2.print("*9D#");
      Answer = SerialCard2.readStringUntil('#');
    }else if (Command.indexOf(F("card2:pause")) > -1){
      Serial2Flush();
      SerialCard2.print("*9E#");
      Answer = SerialCard2.readStringUntil('#');
    }else if (Command.indexOf(F("card2:stop")) > -1){
      Serial2Flush();
      SerialCard2.print("*9F#");
      Answer = SerialCard2.readStringUntil('#');
    }else if (Command.indexOf(F("card2:toc?")) > -1){
      Serial2Flush();
      SerialCard2.print("*9AF#");
      String ans = SerialCard2.readStringUntil('#');
      String toc = ans.substring(5); // add without *ASN

      while ((ans.indexOf("*9AEN00") == -1) && ((ans.indexOf("*9ASF") == 0) || (ans.indexOf("*9ASN") == 0))) {
        // we have more to read
        Serial2Flush();
        SerialCard2.print("*9AN#"); // request follow-up-titles
        ans = SerialCard2.readStringUntil('#'); // *ASNxxxxxxxx
        if (ans.indexOf("*9ASN") == 0) {
          toc = toc + " " + ans.substring(5); // add without *ASN
        }else{
          toc = toc + " EOT";
        }
      }
      Answer = toc;
    }else if (Command.indexOf(F("card2:size")) > -1){
      Serial2Flush();
      SerialCard2.print("*9G0#");
      Answer = SerialCard2.readStringUntil('#');
    }else if (Command.indexOf(F("card2:marker?")) > -1){
      Serial2Flush();
      SerialCard2.print("*9C00#");
      String ans = SerialCard2.readStringUntil('#'); // *ASNxxxxxxxx
      Answer = ans;
    }else if (Command.indexOf(F("card2:select:sdcard@")) > -1){
      // card1:select:sdcard@0 or card1:select:sdcard@1
      Serial2Flush();
      SerialCard2.print("*9R" + Command.substring(Command.indexOf("@")+1) + "#");
      Answer = SerialCard2.readStringUntil('#');
    }else if (Command.indexOf(F("card1:init")) > -1){
      x32InitCard1();
      Answer = "OK";
    }else if (Command.indexOf(F("card2:init")) > -1){
      x32InitCard2();
      Answer = "OK";




    }else{
      // unknown command
      Answer = "UNKNOWN_CMD";
    }
  }else{
    Answer = "ERROR";
  }

  return Answer;
}

void Serial1Flush() {
  while (SerialCard1.available() > 0) {
    SerialCard1.read();
  }
}

void Serial2Flush() {
  while (SerialCard2.available() > 0) {
    SerialCard2.read();
  }
}
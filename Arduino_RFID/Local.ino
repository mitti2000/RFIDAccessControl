// Get id of RFID chip
unsigned long getRfidId(){
  if (!rfid.PICC_ReadCardSerial()) { //Since a PICC placed get Serial and continue
    return -1;
  }
  unsigned long hex_num;
  hex_num =  rfid.uid.uidByte[0] << 24;
  hex_num += rfid.uid.uidByte[1] << 16;
  hex_num += rfid.uid.uidByte[2] <<  8;
  hex_num += rfid.uid.uidByte[3];
  rfid.PICC_HaltA(); // Stop reading
  return hex_num;
}



// Check if response is ok or fail
boolean checkResponse(String response){
  //Serial.println(response);
  if(response.equals("fail")){
    return false;
  }
  return true;
}



// wait for PIN entry
String waitForPin(){
  String pinCode;
  int counter = 0;
  char key = 0;
  unsigned long old = millis();
  while(counter < 4){
    while(key == 0){
      if(old + 20000 < millis()) return "0";
      key = kpd.getKey();
    }
    if(old + 20000 < millis()) return "0";     
    pinCode += key;
    counter++;
    key = 0;
  }
return pinCode;
}


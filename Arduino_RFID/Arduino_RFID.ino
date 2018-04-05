#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN A2
MFRC522 rfid(SS_PIN, RST_PIN);  // Create MFRC522 instance.


void setup() {
  Serial.begin(9600); // Initialize serial communications with the PC
  SPI.begin();        // Init SPI bus
  rfid.PCD_Init();    // Init MFRC522 card
  Serial.println("Scan PICC to see UID and type...");
}

void loop() {
  // Look for new cards
  if ( ! rfid.PICC_IsNewCardPresent()) {
    return;
  }

  // Send card id to server
  sendToServer("id", (String)getRfidId());

  // Wait for response
  String response = waitForMessage("name");
    
  Serial.println(response);
}

void sendToServer(String function, String data){
  String message = function + "_" + data;
  Serial.println(message);
}

String waitForMessage(String function){
  String response;
  do{
    response = Serial.readString();    
  }
  while(response.length() == 0);

  if(response.startsWith(function + "_")){    
    int separator = response.indexOf(function + "_") + function.length() + 1;
    return response.substring(separator);
  }

  return "bullshit";
}

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


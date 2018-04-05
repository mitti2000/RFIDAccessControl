#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>

#define SS_PIN 10 // SlaveSelect Pin of RFID Reader
#define RST_PIN A2 // Reset Pin of RFID Reader
#define LED_RED A0 // Red LED
#define LED_GREEN A1 // Green LED
MFRC522 rfid(SS_PIN, RST_PIN);  // Create MFRC522 instance.
LiquidCrystal_PCF8574 lcd(0x3F); // Create display Instance
boolean initMessage = false; // is inital message set?

// define Keypad Matrix
const byte ROWS = 4; // Four rows
const byte COLS = 3; // Three columns
// Define the Keymap
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'#','0','*'}
};
// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = { 5, 4, 3, 2 };
// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = { 8, 7, 6 }; 

// Create the Keypad
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  Serial.begin(9600); // Initialize serial communications with the PC
  SPI.begin();        // Init SPI bus
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  digitalWrite(A0,LOW);
  digitalWrite(A1,LOW);
  lcd.begin(16,2); // Init Display
  lcd.setBacklight(255);
  rfid.PCD_Init();    // Init MFRC522 card
  //Serial.println("Scan PICC to see UID and type...");
}

void loop() {
  // Show initial Message
  if(!initMessage){
    writeToDisplay("Show Badge:",1);
    initMessage = true;
  }
  // Look for new cards
  if ( ! rfid.PICC_IsNewCardPresent()) {
    return;
  }

  // if a card is recognized, write to display
  writeToDisplay("Thanks. Wait for",1);
  writeToDisplay("check",2);
  String rfid = (String)getRfidId();
  // Send card id to server
  sendToServer("id", rfid);

  // Wait for response
  String response = waitForMessage("name");
  boolean ok = checkResponse(response);
  displayResponse(ok, response);
  String pinCode;
  if(ok) {
    pinCode = waitForPin();
  }
  if(!pinCode.equals("0")){
    String pinMessage = rfid + "#" + pinCode;
    sendToServer("pin", pinMessage);
    String access = waitForMessage("access");
    if(access.equals("true")) accessGranted();
    else accessDenied();
  } else{
    digitalWrite(LED_RED, HIGH);
    writeToDisplay("Timeout!",1);
    writeToDisplay("Try again!",2);
    delay(2000);
    digitalWrite(LED_RED, LOW);
    clearDisplayLine(1);
    clearDisplayLine(2);
    initMessage = false;
  }
}
// send to Server
void sendToServer(String function, String data){
  String message = function + "_" + data;
  Serial.println(message);
}

// wait for Server Response
String waitForMessage(String function){
  String response;
  unsigned long now = millis();
  do{
    /*
    if(millis() - 5000 < now){
      return "fail";
    }
    */
    response = Serial.readString();    
  }
  while(response.length() == 0);
  if(response.startsWith(function + "_")){    
    int separator = response.indexOf(function + "_") + function.length() + 1;
    return response.substring(separator);
  }

  return "bullshit";
}

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

// Write line to Display (line 1 or 2)
void writeToDisplay(String message, int line){
  if(line > 2 || line < 1) return;
  clearDisplayLine(line);
  lcd.setCursor(0,line-1);
  lcd.print(message);
}

// clear one line of the display
void clearDisplayLine(int line){
  lcd.setCursor(0,line-1);
  lcd.print("                ");
}

// Check if response is ok or fail
boolean checkResponse(String response){
  //Serial.println(response);
  if(response.equals("fail")){
    return false;
  }
  return true;
}

// display a response if chip is accepted or not
void displayResponse(boolean ok, String response){
  if(!ok){
    digitalWrite(LED_RED, HIGH);
    writeToDisplay("Badge not",1);
    writeToDisplay("recognized",2);
    delay(2000);
    digitalWrite(LED_RED, LOW);
    clearDisplayLine(1);
    clearDisplayLine(2);
    initMessage = false;
  } else{
    writeToDisplay("Hi " + response, 1);
    writeToDisplay("Enter PIN: ", 2);
  }
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

// show access granted
void accessGranted(){
  digitalWrite(LED_GREEN, HIGH);
  clearDisplayLine(2);
  writeToDisplay("Access Granted",1);
  sing(1);
  delay(3000);
  digitalWrite(LED_GREEN, LOW);
  initMessage = false;
}

// sow access denied
void accessDenied(){
  digitalWrite(LED_RED, HIGH);
  clearDisplayLine(2);
  writeToDisplay("Access Denied",1);
  firstSection();
  delay(3000);
  digitalWrite(LED_RED, LOW);
  initMessage = false;
}












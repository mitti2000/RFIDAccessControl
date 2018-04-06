#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>

#define SS_PIN 10 // SlaveSelect Pin of RFID Reader
#define RST_PIN A2 // Reset Pin of RFID Reader
#define LED_RED A0 // Red LED
#define LED_GREEN A1 // Green LED
#define BUZZER_PIN 9
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
  pinMode(BUZZER_PIN, OUTPUT);
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

  String rfid = (String)getRfidId();  // read RFID ID from chip
  
  sendToServer("id", rfid);   // Send card id to server

  String response = waitForMessage("name");   // Wait for response
  boolean ok = checkResponse(response);   //Check if response ok or not
  //Show result on Display and LED
  if(ok) chipAcceptedResponse(response);
  else {
    chipNotAcceptedResponse();
    return;
  }
  
  String pinCode;
  pinCode = waitForPin(); // wait for Pin Code entry

  if(!pinCode.equals("0")){  // if the PinCode is not 0
    String pinMessage = rfid + "#" + pinCode; // create message for Server
    sendToServer("pin", pinMessage); // send message to server
    String access = waitForMessage("access"); // wait for server response
    if(access.equals("true")) accessGranted(); // if pin is ok grant access
    else if (access.equals("timeout")) pinTimeout();
    else accessDenied(); // if pin is not ok, deny access
  } else{
    pinTimeout(); // otherwise show the timeout
  }
}


















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

// display a response if chip is accepted or not
void chipAcceptedResponse(String response){
  writeToDisplay("Hi " + response, 1);
  writeToDisplay("Enter PIN: ", 2);
}

void chipNotAcceptedResponse(){
  digitalWrite(LED_RED, HIGH);
  writeToDisplay("Badge not",1);
  writeToDisplay("recognized",2);
  firstSection();
  delay(3000);
  digitalWrite(LED_RED, LOW);
  clearDisplayLine(1);
  clearDisplayLine(2);
  initMessage = false;
}

// show access granted
void accessGranted(){
  digitalWrite(LED_GREEN, HIGH);
  clearDisplayLine(2);
  writeToDisplay("Access Granted",1);
  secondSection();
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

void pinTimeout(){
  digitalWrite(LED_RED, HIGH);
    writeToDisplay("Timeout!",1);
    writeToDisplay("Try again!",2);
    firstSection();
    delay(2000);
    digitalWrite(LED_RED, LOW);
    clearDisplayLine(1);
    clearDisplayLine(2);
    initMessage = false;
}

void showPinEntry(int pos){
  lcd.setCursor(pos,2);
  lcd.print("*");
}



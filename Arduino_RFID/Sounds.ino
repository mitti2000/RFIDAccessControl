const int c = 261;
const int d = 294;
const int e = 329;
const int f = 349;
const int g = 391;
const int gS = 415;
const int a = 440;
const int aS = 455;
const int b = 466;
const int cH = 523;
const int cSH = 554;
const int dH = 587;
const int dSH = 622;
const int eH = 659;
const int fH = 698;
const int fSH = 740;
const int gH = 784;
const int gSH = 830;
const int aH = 880;
const int aSH = 932;
const int bH = 988;
const int cH2 = 1048;
const int cSH2 = 1109;
const int dH2 = 1175;
const int dSH2 = 1245;

int counter = 0;

void beep(int note, int duration)
{
  //Play tone on BUZZER_PIN
  tone(BUZZER_PIN, note, duration);
 
  //Play different LED depending on value of 'counter'
  delay(duration);
 
  //Stop tone on BUZZER_PIN
  noTone(BUZZER_PIN);
 
  delay(50);
 
  //Increment counter
  counter++;
}

// imperial theme
void firstSection()
{
  beep(a, 500);
  beep(a, 500);    
  beep(a, 500);
  beep(f, 350);
  beep(cH, 150);  
  beep(a, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 650);
}

//Star wars theme
void secondSection(){
  beep(cH,500);
  beep(gH,500);
  beep(fH,150);
  beep(eH,150);
  beep(dH,150);
  beep(cH2,500);
  beep(gH,300);
}
 









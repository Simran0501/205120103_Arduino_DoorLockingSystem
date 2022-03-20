#include <Servo.h>
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Password.h>
Servo doorLock;
int incomingByte = 0;
String newPasswordString; //hold the new password
char newPassword[6]; //character string of newPasswordString
LiquidCrystal lcd(8,9,4,5,6,7);
//initialize password to 1234
//you can use password.set(newPassword) to overwrite it
Password password = Password( "1092" );
byte maxPasswordLength = 6; 
byte currentPasswordLength = 0;
const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns
int buttonState = 0; 
//Define the keymap
char keys[ROWS][COLS] = {
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}
};
//// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = {34,35,36,37}; //connect to row pinouts
// Connect keypad COL0, COL1, COL2 and COL3 to these Arduino pins.
byte colPins[COLS] = {30,31,32,33}; //connect to column pinouts
// Create the Keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
void setup(){
  doorLock.attach(44);
  doorLock.write(100);
  Serial1.begin(9600);
  pinMode(44, OUTPUT);
  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Passkey: ");
  pinMode(2, INPUT);
}
void loop(){
  char key = keypad.getKey();
  buttonState = digitalRead(2);
  if (buttonState == HIGH) { 
     doorLock.write(-180);
     delay(10000);
     doorLock.write(100);
     setup();
  }
  if (key != NO_KEY){
     delay(60); 
     switch (key){
     case 'A': break; 
     case 'B': break; 
     case 'C': break; 
     case '#': checkPassword(); break;
     case '*': resetPassword(); break;
     default: processNumberKey(key);
     }
  }
}
void processNumberKey(char key) {
  Serial1.print(key);
  currentPasswordLength++;
  password.append(key);
  lcd.print(int(key)-int(48));
  if (currentPasswordLength == maxPasswordLength) {
     checkPassword();
  }
}
void checkPassword() {
  if (password.evaluate()){
     Serial.println(" OK.");
     lcd.clear();
     lcd.print("Key Authorized");
     delay(1000);
     lcd.clear();
     lcd.print("Complete BioLock");
     Serial1.read();
     Serial1.flush();
     while (!Serial1.available()){
     }
     while (int(Serial1.read())==!int(1)){
     }
     lcd.clear();
     lcd.print("User Authorized");
     doorLock.write(-180);
     delay(10000);
     doorLock.write(100);
     setup();
  } else {
     lcd.clear();
     lcd.print("Access Denied");
     delay(1000);
     setup();
  } 
}
void resetPassword() {
  password.reset(); 
  currentPasswordLength = 0;
  lcd.clear();
} 

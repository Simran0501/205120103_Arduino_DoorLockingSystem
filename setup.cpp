#include <Adafruit_Fingerprint.h>
// On Leonardo/Micro or others with hardware serial, use those! #0 is green wire, #1 is white
// uncomment this line:
// #define mySerial Serial1
// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// comment these two lines if using hardware serial
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
void setup()  
{
 Serial.begin(9600);
 while (!Serial);  // For Yun/Leo/Micro/Zero/...
 delay(100);
 // set the data rate for the sensor serial port
 finger.begin(57600);
 if (finger.verifyPassword()) {
 } else {
   Serial.println("Did not find fingerprint sensor :(");
   while (1) { delay(1); }
 }
 finger.getTemplateCount();
}
void loop()                     // run over and over again
{
 getFingerprintIDez();
 delay(50);            //don't ned to run this at full speed.
}
uint8_t getFingerprintID() {
 uint8_t p = finger.getImage();
 switch (p) {
   case FINGERPRINT_OK:
     Serial.println("Image taken");
     break;
   case FINGERPRINT_NOFINGER:
     Serial.println("No finger detected");
     return p;
   case FINGERPRINT_PACKETRECIEVEERR:
     Serial.println("Communication error");
     return p;
   case FINGERPRINT_IMAGEFAIL:
     Serial.println("Imaging error");
     return p;
   default:
     Serial.println("Unknown error");
     return p;
 }
 // OK success!
 p = finger.image2Tz();
 switch (p) {
   case FINGERPRINT_OK:
     Serial.println("Image converted");
     break;
   case FINGERPRINT_IMAGEMESS:
     Serial.println("Image too messy");
     return p;
   case FINGERPRINT_PACKETRECIEVEERR:
     Serial.println("Communication error");
     return p;
   case FINGERPRINT_FEATUREFAIL:
     Serial.println("Could not find fingerprint features");
     return p;
   case FINGERPRINT_INVALIDIMAGE:
     Serial.println("Could not find fingerprint features");
     return p;
   default:
     Serial.println("Unknown error");
     return p;
 }
 // OK converted!
 p = finger.fingerFastSearch();
 if (p == FINGERPRINT_OK) {
   Serial.println("Found a print match!");
 } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
   Serial.println("Communication error");
   return p;
 } else if (p == FINGERPRINT_NOTFOUND) {
   Serial.println("Did not find a match");
   return p;
 } else {
   Serial.println("Unknown error");
   return p;
 }   
 Serial.print(1);
 return finger.fingerID;
}
// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
 uint8_t p = finger.getImage();
 if (p != FINGERPRINT_OK)  return -1;
 p = finger.image2Tz();
 if (p != FINGERPRINT_OK)  return -1;
 p = finger.fingerFastSearch();
 if (p != FINGERPRINT_OK)  return -1;
 Serial.print(1);
}

#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522 (SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27,20,4);

const int pushButtonA = 5;
const int pushButtonB = 6;
const int pushButtonC = 7;
const int pushButtonD = 8;
 
byte ActualUID[4]; //almacenará el código del Tag leído
byte Usuario1[4]= {0x13, 0x7F, 0x89, 0x1A} ; //código del usuario 1
byte Usuario2[4]= {0xF2, 0xED, 0xE8, 0x20} ; //código del usuario 2

bool cardState;

Servo myservoA;
Servo myservoB;
Servo myservoC;
Servo myservoD;

int pos = 0;
int buttonStateA = 0;
int buttonStateB = 0;
int buttonStateC = 0;
int buttonStateD = 0;
 

void setup() {
  Serial.begin(9600);
  SPI.begin();
  lcd.init(); 
  pinMode(pushButtonA, INPUT);
  pinMode(pushButtonB, INPUT);
  pinMode(pushButtonC, INPUT);
  pinMode(pushButtonD, INPUT);
  mfrc522.PCD_Init();
  Serial.println("Control de acceso:");
}

void loop() {
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("Bienvenido");
  lcd.setCursor(0,1);
  lcd.print("Pon tu tarjeta!!");

  buttonStateA = digitalRead (pushButtonA);
  buttonStateB = digitalRead (pushButtonB);
  buttonStateC = digitalRead (pushButtonC);
  buttonStateD = digitalRead (pushButtonD);

    if ( mfrc522.PICC_IsNewCardPresent()) {  
      if ( mfrc522.PICC_ReadCardSerial()) {
          for (byte i = 0; i < mfrc522.uid.size; i++) {                        
               ActualUID[i]=mfrc522.uid.uidByte[i];          
          }                  
          if(compareArray(ActualUID,Usuario1)){
          messageAuthorized();         
          } else if(compareArray(ActualUID,Usuario2)){
          messageAuthorized();
          } else {
          cardState = false;
                  
          mfrc522.PICC_HaltA();
    } 
   } 
  } 
  Serial.println(cardState);
         
  if((buttonStateA == LOW) && (buttonStateB == HIGH) && (buttonStateC == HIGH ) && (buttonStateD == HIGH)){
    lcd.clear();
    if(cardState == 0){
    messageUnauthorized();
    } else  if(cardState == 1) {
      myservoA.attach(A0);
      myservoA.write(180);
      delay(1000);
      myservoA.write(90);
      myservoA.detach();
    cardState = false;
    lcd.print("Toma el producto");
    delay(2000);
    }
    lcd.clear();
  }

  if((buttonStateB == LOW) && (buttonStateA == HIGH) && (buttonStateC == HIGH) && (buttonStateD == HIGH)){
    lcd.clear();
    if(cardState == 0){
    messageUnauthorized();
    } else  if(cardState == 1) {
    myservoB.attach(2);
    myservoB.write(180);
    delay(1000);
    myservoB.detach();
    cardState = false;
    lcd.print("Toma el producto");
    delay(2000);
    }
    lcd.clear();
  }

  if((buttonStateC == LOW) && (buttonStateB == HIGH) && (buttonStateA == HIGH) && (buttonStateD == HIGH)){
    lcd.clear();
    if(cardState == 0){
    messageUnauthorized();
    } else  if(cardState == 1) {
    myservoC.attach(3);
    myservoC.write(180);
    delay(1000);
    myservoC.detach();
    cardState = false;
    lcd.print("Toma el producto");
    delay(2000);
    }
    lcd.clear();  
  }

  if((buttonStateD == LOW) && (buttonStateB == HIGH) && (buttonStateA == HIGH) && (buttonStateC == HIGH)){
    lcd.clear();
    if(cardState == 0){
    messageUnauthorized();
    } else  if(cardState == 1) {
    myservoD.attach(4);
    myservoD.write(180);
    delay(1000);
    myservoD.detach();
    cardState = false;
    lcd.print("Toma el producto");
    delay(2000);
    }
    lcd.clear();
  }
}

void messageAuthorized(){
    lcd.clear();
    lcd.setCursor(5,0);
    lcd.print("Acceso");
    lcd.setCursor(3,1);
    lcd.print("Autorizado");
    delay(2000);
    cardState = true;
}

void messageUnauthorized(){
    lcd.setCursor(1,0);
    lcd.print("No  Autorizado");
    lcd.setCursor(0,1);
    lcd.print("Pon tu tarjeta!!");
    delay(2000);
}

bool compareArray(byte array1[],byte array2[]) {
  if(array1[0] != array2[0])return(false);
  if(array1[1] != array2[1])return(false);
  if(array1[2] != array2[2])return(false);
  if(array1[3] != array2[3])return(false);
  return(true);
}

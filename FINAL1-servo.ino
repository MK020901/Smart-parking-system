#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
 #include <LiquidCrystal_I2C.h> // Decleration the LCD header file
#include <string.h>
#define SS_PIN 10
#define RST_PIN 9

LiquidCrystal_I2C lcd(0X27, 16, 2);//RS, E, D4, D5, D6, D7
const int sensora = 6;
const int sensorb = 7;
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
Servo myServo; //define servo name
 
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  myServo.attach(8); //servo pin
  lcd.clear();
  lcd.begin(16,2);
  lcd.init(); // initialize the lcd
  lcd.backlight();

}

 int angle=0;
 int count=1;
 int statusSensor1 = 0;
 int statusSensor2 = 0;
 int a = 0;

void loop() 
{
    statusSensor1 = digitalRead (sensora);
    statusSensor2 = digitalRead (sensorb);
  
    a = statusSensor1 + statusSensor2;
    lcd.setCursor(0,0);
    lcd.print("HELLO WELCOME");
    Serial.println("Welcome");
    Serial.println();
    lcd.setCursor(1,1);
    if (a == 2){
    lcd.print("AVBLE. SLOTS:2");
    Serial.println("AVBLE SLOTS: 2");
  }
  if (a == 1){
    lcd.print("AVBLE. SLOTS:1");
    Serial.println("AVBLE SLOTS: 1");
  }
  if (a == 0){
    lcd.print("AVBLE. SLOTS:0");
    Serial.println("AVBLE SLOTS: 0");
    
  }
  delay(1000);
  if(a==1 || a==2) {
  // Look for new cards
  Serial.println("Put your card to the reader...");
  Serial.println();
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "E9 E2 41 E9" && count%2 == 1) //change here the UID of the card/cards that you want to give access
  {
    count=count+1;
    Serial.println("Authorized access");
    Serial.println();
    delay(100);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("ACCESS GRANTED");
     
    for(angle = 0; angle < 90; angle += 1)    // command to move from 0 degrees to 180 degrees 
    {                                  
    myServo.write(angle);                 //command to rotate the servo to the specified angle
    delay(15);    
    }

    for(angle = 90; angle>=0; angle -= 1)     // command to move from 180 degrees to 0 degrees 
   {                                
    myServo.write(angle);              //command to rotate the servo to the specified angle
    delay(25);                     
  } 
  }
  else if (content.substring(1) == "E9 E2 41 E9" && count%2 == 0)
  {
    for(angle = 0; angle < 90; angle += 1)    // command to move from 0 degrees to 180 degrees 
    {                                  
    myServo.write(angle);                 //command to rotate the servo to the specified angle
    delay(15);    
    }

    for(angle = 90; angle>=0; angle -= 1)     // command to move from 180 degrees to 0 degrees 
   {                                
    myServo.write(angle);              //command to rotate the servo to the specified angle
    delay(25);                     
  } 
  
    count=count+1;
    Serial.println("Thank you! Please visit again");
    Serial.println();
    delay(100);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("VISIT AGAIN");                     
  }                       

 else   {
    Serial.println(" Access denied.SORRY! Not registered");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("ACCESS DENIED");
 }
} 
}

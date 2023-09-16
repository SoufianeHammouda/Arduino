#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h> 
#include <RFID.h>
#include <Servo.h> 

LiquidCrystal_I2C lcd(0x3F, 16, 2);     // Set the LCD address (0x27) and size of lcd (16 chars and 2 line display)


RFID rfid(10, 9);       //D10:pin of tag reader SDA. D9:pin of tag reader RST 
unsigned char status; 
unsigned char str[MAX_LEN]; //MAX_LEN is 16: size of the array 

String accessGranted [2] = {"310988016", "19612012715"};  //RFID serial numbers to grant access to
int accessGrantedSize = 2;                                //The number of serial numbers

Servo lockServo;                //Servo for locking mechanism
int lockPos = 15;               //Locked position limit
int unlockPos = 75;             //Unlocked position limit
boolean locked = true;

int redLEDPin = 5;
int greenLEDPin = 6;

void setup() 
{ 
    lcd.init();         // initialize the LCD
  lcd.backlight();

  
  Serial.begin(9600);     //Serial monitor is only required to get tag ID numbers and for troubleshooting
  SPI.begin();            //Start SPI communication with reader
  rfid.init();            //initialization 
  pinMode(redLEDPin, OUTPUT);     //LED startup sequence
  pinMode(greenLEDPin, OUTPUT);
  digitalWrite(redLEDPin, HIGH);
  delay(200);
  digitalWrite(greenLEDPin, HIGH);
  delay(200);
  digitalWrite(redLEDPin, LOW);
  delay(200);
  digitalWrite(greenLEDPin, LOW);
  lockServo.attach(3);
  lockServo.write(lockPos);         //Move servo into locked position
  Serial.println("Place card/tag near reader...");
} 

void loop() 
{ 
    lcd.clear();                      // clear the LCD
  lcd.setCursor (0, 0);             // Set the cursor to 1st line
  lcd.print("Door Closed");         // print the message
  lcd.setCursor (4, 1);             // Set the cursor to 2nd line
  lcd.print("Check Admin");        // print the message
  delay(2000);
  lcd.clear();                      // clear the LCD
  lcd.setCursor (3, 0);             // Set the cursor
  lcd.print("Door");       // print the message
  lcd.setCursor (8, 1);             // Set the cursor
  lcd.print("Opened");        // print the message
  delay(2000);
  
  if (rfid.findCard(PICC_REQIDL, str) == MI_OK)   //Wait for a tag to be placed near the reader
  { 
    Serial.println("Card found"); 
    String temp = "";                             //Temporary variable to store the read RFID number
    if (rfid.anticoll(str) == MI_OK)              //Anti-collision detection, read tag serial number 
    { 
      Serial.print("The card's ID number is : "); 
      for (int i = 0; i < 4; i++)                 //Record and display the tag serial number 
      { 
        temp = temp + (0x0F & (str[i] >> 4)); 
        temp = temp + (0x0F & str[i]); 
      } 
      Serial.println (temp);
      checkAccess (temp);     //Check if the identified tag is an allowed to open tag
    } 
    rfid.selectTag(str); //Lock card to prevent a redundant read, removing the line will make the sketch read cards continually
  }
  rfid.halt();
}

void checkAccess (String temp)    //Function to check if an identified tag is registered to allow access
{
  boolean granted = false;
  for (int i=0; i <= (accessGrantedSize-1); i++)    //Runs through all tag ID numbers registered in the array
  {
    if(accessGranted[i] == temp)            //If a tag is found then open/close the lock
    {
      Serial.println ("Access Granted");
      granted = true;
      if (locked == true)         //If the lock is closed then open it
      {
          lockServo.write(unlockPos);
          locked = false;
      }
      else if (locked == false)   //If the lock is open then close it
      {
          lockServo.write(lockPos);
          locked = true;
      }
      digitalWrite(greenLEDPin, HIGH);    //Green LED sequence
      delay(200);
      digitalWrite(greenLEDPin, LOW);
      delay(200);
      digitalWrite(greenLEDPin, HIGH);
      delay(200);
      digitalWrite(greenLEDPin, LOW);
      delay(200);
    }
  }
  if (granted == false)     //If the tag is not found
  {
    Serial.println ("Access Denied");
    digitalWrite(redLEDPin, HIGH);      //Red LED sequence
    delay(200);
    digitalWrite(redLEDPin, LOW);
    delay(200);
    digitalWrite(redLEDPin, HIGH);
    delay(200);
    digitalWrite(redLEDPin, LOW);
    delay(200);
  }
}

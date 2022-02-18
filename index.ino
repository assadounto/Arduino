/*

   This is the code for the Project
   IoT Security Project using Fingerprint sensor & Hex Keypad.


   This code was written by Sachin Soni and Joy Jacob for
   project made for techiesms YouTube channel.You can
   watch the full tutorial video for the project on the
   channel.

   Anyone using this code for commercial purpose or replicating this
   project on thier social medias should give credits to techiesms.


   Channel Link :- http://www.youtube.com/techiesms


           techiesms
   explore | learn | share

*/



#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#include "enrol.h"
#include "fingerprint.h"

// Icon of Fingerprint




void setup()
{
  // put your setup code here, to run once:
  digitalWrite(Relay, HIGH);
  pinMode(Relay, OUTPUT);
  digitalWrite(Relay, HIGH);
  digitalWrite(buzzer, LOW);
  lcd.init();                      // initialize the lcd
  lcd.init();
  lcd.backlight();

  initWifi();

  digitalWrite(buzzer, LOW);
  digitalWrite(Relay, HIGH);
}



void loop() {
  // put your main code here, to run repeatedly:

  char customKey = customKeypad.getKey();

  if (customKey)
  {
    Serial.println(customKey);

    // Maze Lock Mode
    if (customKey == 'A')
    {
      Serial.println("Maze Lock Mode");
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("KEYPAD MODE");
      beep(200);
      beep(200);
      delay(2000);

      Serial.println("Enter the \nFour Digit \nCode");

      fingerprint_flag = 0;
    }

    // Enrollment Mode
    if (customKey == 'C')
    {
      beep(200);
      if (Keypad_Input() == Secret_Code)
      {
        Serial.println("D");
        Serial.println("Enrollment mode");
        lcd.clear();
        lcd.setCursor(1, 0);            // Start at top-left corner
        lcd.print("Enrollment Mode");
        beep(200);
        beep(200);
        delay(1000);
        Serial.println("Enter ID");
        lcd.clear();
        lcd.setCursor(4, 0);            // Start at top-left corner
        lcd.print("ID XXX");
        ID_e = "";
        i = 0;
        fingerprint_flag = 2;
        setup_e();
        loop_e();
        fingerprint_flag = 1;
      }
      else
      {
        Serial.println("Wrong Code");
        lcd.clear();
        lcd.setCursor(4, 0); // Start at top-left corner
        lcd.print("Wrong Code");
        beep(200);
        beep(200);
        delay(1000);
      }
    }
  }


  if (fingerprint_flag == 1)
  {
    customKey = customKeypad.getKey();
    finger.fingerID = 0;
    delay(50);
    setup_f();
    loop_f();
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("PLACE FINGER");
    lcd.setCursor(0, 1);
    lcd.print("................");
    while (customKey == 'C' || customKey == 'A')
      break;
    delay(1000);
  }


  if (fingerprint_flag == 0)
  {
    if (Keypad_Input() == Secret_Code)
    {
      Serial.println("Correct Code");
      lcd.clear();
      lcd.setCursor(3, 0);           // Start at top-left corner
      lcd.print("Correct Code");
      beep(1000);
      Door_control();
      ID = 200;
      makeIFTTTRequest();

      delay(1000);

      fingerprint_flag = 1;
    }
    else
    {
      Serial.println("Wrong Code");
      lcd.clear();
      lcd.setCursor(4, 0);            // Start at top-left corner
      lcd.print("Wrong Code");
      beep(200);
      beep(200);
    }
    ID_e = "";
  }
}


// Establish a Wi-Fi connection with your router
void initWifi()
{
  Serial.print("Connecting to: ");
  Serial.print(ssid);
  lcd.clear();

  // Draw white text
  lcd.setCursor(3, 0);            // Start at top-left corner
  lcd.print("Connecting");
  lcd.setCursor(4, 1);
  lcd.print(ssid);
  lcd.display();
  delay(1000);
  WiFi.begin(ssid, password);

  int timeout = 10 * 4; // 10 seconds
  while (WiFi.status() != WL_CONNECTED  && (timeout-- > 0)) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Failed to connect, going back to sleep");
    lcd.clear();
    lcd.setCursor(0, 0);            // Start at top-left corner
    lcd.print("Not Connected");
  }
  else
  {
    Serial.print("WiFi connected in: ");
    Serial.print(millis());
    Serial.print(", IP address: ");
    Serial.println(WiFi.localIP());
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.println(("Connected"));
  }
  delay(700);
}

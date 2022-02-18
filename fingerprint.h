
// Replace with your SSID and Password
const char* ssid     = "richmond1";
const char* password = "12345678";

// Necesarry Variables to make Attendance Sheet
String Event_name = "fingerprint";
String Webhooks_key = "nEfCB7Q7XpxyCxnAtSx9dgpsLnDgCi3ICr00lbKFURr";

//The Secret Code
int Secret_Code = 5050;

// Pin Definition
int buzzer = 15;
int Relay = 23;

//Misc. Variables
int i = 0;
int k = 0;
bool wrong_code = 1;
uint8_t new_ID = 0;
String NAME = "";
String ID = "";
char* response = " ";
String res = "";
char* succ_code = "200 OK";
unsigned int p;


// Maker Webhooks IFTTT
String resource = "/trigger/" + Event_name + "/with/key/" + Webhooks_key;
const char* server = "maker.ifttt.com";

unsigned int getFingerprintEnroll();
unsigned int readnumber();
unsigned int getFingerprintID();


void beep(int duration);
void Door_control();
unsigned int getFingerprintEnroll();
unsigned int readnumber();
unsigned int getFingerprintID();
int getFingerprintIDez();



void setup_f()
{
  Serial.begin(57600);
  pinMode(buzzer, OUTPUT);
  
  while (!Serial);
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    int i;
    while (i < 7) {
      delay(10);
      Serial.print(".!F!.");
      i++;
    }
  }

  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  Serial.println("Waiting for valid finger...");
  //  display.clearDisplay();
  //  display.setTextSize(2);             // Normal 1:1 pixel scale
  //  display.setTextColor(SSD1306_WHITE);        // Draw white text
  //  display.setCursor(0, 0);            // Start at top-left corner
  //  display.println(("Waiting \nfor \nvalid \nfinger..."));
  //  display.display();
  //  delay(500);
}

void loop_f()                     // run over and over again
{
  getFingerprintID();
  delay(50);            //don't ned to run this at full speed.
}



// Make an HTTP request to the IFTTT web service
void makeIFTTTRequest()
{
  Serial.print("Connecting to ");
  Serial.print(server);

  WiFiClient client;
  int retries = 5;
  while (!!!client.connect(server, 80) && (retries-- > 0)) {
    Serial.print(".");
  }
  Serial.println();
  if (!!!client.connected()) {
    Serial.println("Failed to connect...");
  }

  Serial.print("Request resource: ");
  Serial.println(resource);

  // Temperature in Celsius
  String jsonObject = String("{\"value1\":\"") + ID + "\"}";
  client.println(String("POST ") + resource + " HTTP/1.1");
  client.println(String("Host: ") + server);
  client.println("Connection: close\r\nContent-Type: application/json");
  client.print("Content-Length: ");
  client.println(jsonObject.length());
  client.println();
  client.println(jsonObject);

  int timeout = 3 * 10; // 5 seconds
  while (!!!client.available() && (timeout-- > 0)) {
    delay(100);
  }
  if (!!!client.available()) {
    Serial.println("No response...");
  }
  while (client.available())
  {
    // Serial.write(client.read());
    char add = client.read();
    res = res + add;
  }
  response = &res[0];
  Serial.println("=======");
  Serial.println(response);
  if (strstr(response, succ_code))
  {
    Serial.println("Registered");
    lcd.clear();
    lcd.setCursor(0, 0);            // Start at top-left corner
    lcd.println(("Registered"));
    delay(1000);
  }
  else
  {
    Serial.println("Not Registered");
    lcd.clear();
    lcd.setCursor(0, 0);            // Start at top-left corner
    lcd.println("Not Registered");
    delay(1000);
  }
  response = "";
  res = "";
  Serial.println("\nclosing connection");

  client.stop();
}

// returns -1 if failed, otherwise returns ID #
unsigned int getFingerprintID()
{
  unsigned int p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");

      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");

      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      lcd.clear();
      // Draw white text
      lcd.setCursor(0, 0);            // Start at top-left corner
      lcd.print("Commun error");
      beep(200);
      beep(200);
      beep(200);
      delay(200);
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

  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
    lcd.clear();
    lcd.setCursor(2, 0);            // Start at top-left corner
    lcd.print("FINGER FOUND");
    lcd.setCursor(1, 1);            // Start at top-left corner
    lcd.print("YOU ARE WELCOME");
    beep(1000);
    //delay(1000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    lcd.clear();
    lcd.setCursor(0, 0);            // Start at top-left corner
    lcd.print("FINGER NOT FOUND");
    beep(200);
    beep(200);
    delay(500);
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  lcd.clear();
  lcd.setCursor(6, 0);            // Start at top-left corner
  lcd.println(("ID"));
  lcd.setCursor(2, 1);
  lcd.print(finger.fingerID);
  delay(500);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  if (finger.confidence > 40)
  {
    Door_control();
    ID = finger.fingerID;
    makeIFTTTRequest();
  }
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

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);

  return finger.fingerID;
}
void beep(int duration)
{
  digitalWrite(buzzer, HIGH);
  delay(duration);
  digitalWrite(buzzer, LOW);
  delay(30);
}

void Door_control()
{
  digitalWrite(Relay, LOW);
  Serial.println("Opening the door");
  lcd.clear();
  lcd.setCursor(0, 0);            // Start at top-left corner
  lcd.print("Opening the door");
  delay(1000);
  Serial.println("Closing the door");
  lcd.clear();
  lcd.setCursor(0, 0);            // Start at top-left corner
  lcd.println("Closing the door");

  delay(2000);
  digitalWrite(Relay, HIGH);
}

//}

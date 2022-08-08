#define BLYNK_TEMPLATE_ID "TMPLHet55qJa"
#define BLYNK_DEVICE_NAME "Antithief"
#define BLYNK_AUTH_TOKEN "B8mcIQ6cjhe_agA4Ptdkc2WJS5ujQxJO"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

// Thư viện kết nối app blynk
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
// Thư viện đọc tín hiệu thẻ từ
#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Iphone XR";
char pass[] = "12345678";

WidgetLCD lcd(V5);

// or Software Serial on Uno, Nano...
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(2, 3);  // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 38400

ESP8266 wifi(&EspSerial);
// define led connect
WidgetLED led_connect(V0);
BlynkTimer timer;

// define variable in use
boolean bt1_state = LOW;  // => trạng thái bật/tắt hệ thống
boolean bt2_state = LOW;  // => trạng thái bật/tắt còi báo động
boolean bt3_state = LOW;  // => trạng thái đóng/mở cửa

// define digital in use
#define SENSOR 6  //=> cảm biến hồng ngoại
#define LED 4     //=> còi báo hiệu
#define ELOCK 5   //=> Khóa điện

#define RST_PIN 9  // Thẻ từ
#define SS_PIN 10  // Thẻ từ
// define MFRC522 => bắt buộc
MFRC522 mfrc522(SS_PIN, RST_PIN);

// TODO: read add and remove card
unsigned long uidDec, uidDecTemp;
int ARRAYindexUIDcard;
int EEPROMstartAddr;
long adminID = 1122539531; // Change admin card
bool beginCard = 0;
bool addCard = 1;
bool skipCard = 0;
int LockSwitch;
unsigned long CardUIDeEPROMread[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

void setup() {
  // Debug console
  Serial.begin(115200);

  // Set up led and sensor
  pinMode(SENSOR, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(ELOCK, INPUT);

  // setup thẻ từ
  SPI.begin();
  mfrc522.PCD_Init();
  delay(4);	
  mfrc522.PCD_DumpVersionToSerial();

  // Set ESP8266 baud rate and connect to wifi
  EspSerial.begin(ESP8266_BAUD);
  Blynk.begin(auth, wifi, ssid, pass, "blynk.cloud", 80);

  // sync value from server blynk
  Blynk.syncVirtual(V1);
  Blynk.syncVirtual(V2);
  Blynk.syncVirtual(V3);
  digitalWrite(LED, LOW);
}

void loop() {
  check_card();
  connect();
  check_btn();
  // Blynk vs timer
  Blynk.run();
  timer.run();
}

void connect() {
  if (Blynk.connect()) {
    led_connect.on();
  } else {
    led_connect.off();
  }
}

void check_card() {

  if (digitalRead(BTN_PIN) == LOW) {
    digitalWrite(SLN_PIN, HIGH); //unlock
    lcd.clear();
    lcd.print(0, 0, " BUTTON UNLOCK ");
    lcd.print(0, 1, "   DOOR OPEN   ");
    // digitalWrite(PiezoPin, HIGH), delay(200), digitalWrite(PiezoPin, LOW);
    delay(2000);
    DisplayWAiT_CARD();
  }

  if (beginCard == 0) {
    if ( ! mfrc522.PICC_IsNewCardPresent()) {  //Look for new cards.
      Blynk.run();
      return;
    }

    if ( ! mfrc522.PICC_ReadCardSerial()) {  //Select one of the cards.
      Blynk.run();
      return;
    }
  }
  
  //Read "UID".
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uidDecTemp = mfrc522.uid.uidByte[i];
    uidDec = uidDec * 256 + uidDecTemp;
  }

  if (beginCard == 1 || LockSwitch > 0)EEPROMwriteUIDcard();  //uidDec == adminID

  if (LockSwitch == 0) {
    //CardUIDeEPROMread.
    for (ARRAYindexUIDcard = 0; ARRAYindexUIDcard <= 9; ARRAYindexUIDcard++) {
      if (CardUIDeEPROMread[ARRAYindexUIDcard] > 0) {
        if (CardUIDeEPROMread[ARRAYindexUIDcard] == uidDec) {
          lcd.clear();
          lcd.print(0, 0, "CARD ACCESS OPEN");
          lcd.print(3, 1, uidDec);
          digitalWrite(SLN_PIN, HIGH); //unlock
          digitalWrite(PiezoPin, HIGH), delay(200), digitalWrite(PiezoPin, LOW);
          delay(2000);
          break;
        }
      }
    }

    if (ARRAYindexUIDcard == 10) {
      lcd.clear();
      lcd.print(0, 0, " Card not Found ");
      lcd.print(0, 1, "                ");
      lcd.print(0, 1, "ID : ");
      lcd.print(5, 1, uidDec);
      for (int i = 0; i <= 2; i++)delay(100), digitalWrite(PiezoPin, HIGH), delay(100), digitalWrite(PiezoPin, LOW);
      digitalWrite(SLN_PIN, LOW);  //lock();
      delay(2000);
    }

    ARRAYindexUIDcard = 0;
    DisplayWAiT_CARD();
  }
}

void EEPROMwriteUIDcard() {

  if (LockSwitch == 0) {
    lcd.clear();
    lcd.print(0, 0, " START REC CARD ");
    lcd.print(0, 1, "PLEASE SCAN CARDS");
    delay(500);
  }

  if (LockSwitch > 0) {
    if (skipCard == 1) {  //uidDec == adminID
      lcd.clear();
      lcd.print(0, 0, "   Remove RECORD   ");
      lcd.print(0, 1, "                ");
      lcd.print(0, 1, "   label : ");
      lcd.print(11, 1, EEPROMstartAddr / 5);
      EEPROMstartAddr += 5;
      skipCard = 0;
    } else {
      Serial.println("writeCard");
      EEPROM.write(EEPROMstartAddr, uidDec & 0xFF);
      EEPROM.write(EEPROMstartAddr + 1, (uidDec & 0xFF00) >> 8);
      EEPROM.write(EEPROMstartAddr + 2, (uidDec & 0xFF0000) >> 16);
      EEPROM.write(EEPROMstartAddr + 3, (uidDec & 0xFF000000) >> 24);
      EEPROM.commit();
      delay(10);
      lcd.clear();
      lcd.print(0, 1, "                ");
      lcd.print(0, 0, "RECORD OK! IN   ");
      lcd.print(0, 1, "MEMORY : ");
      lcd.print(9, 1, EEPROMstartAddr / 5);
      EEPROMstartAddr += 5;
      delay(500);
    }
  }

  LockSwitch++;

  if (EEPROMstartAddr / 5 == 10) {
    lcd.clear();
    lcd.print(0, 0, "RECORD FINISH");
    delay(2000);
    EEPROMstartAddr = 0;
    uidDec = 0;
    ARRAYindexUIDcard = 0;
    EEPROMreadUIDcard();
  }
}

void EEPROMreadUIDcard() {
  for (int i = 0; i <= 9; i++) {
    byte val = EEPROM.read(EEPROMstartAddr + 3);
    CardUIDeEPROMread[ARRAYindexUIDcard] = (CardUIDeEPROMread[ARRAYindexUIDcard] << 8) | val;
    val = EEPROM.read(EEPROMstartAddr + 2);
    CardUIDeEPROMread[ARRAYindexUIDcard] = (CardUIDeEPROMread[ARRAYindexUIDcard] << 8) | val;
    val = EEPROM.read(EEPROMstartAddr + 1);
    CardUIDeEPROMread[ARRAYindexUIDcard] = (CardUIDeEPROMread[ARRAYindexUIDcard] << 8) | val;
    val = EEPROM.read(EEPROMstartAddr);
    CardUIDeEPROMread[ARRAYindexUIDcard] = (CardUIDeEPROMread[ARRAYindexUIDcard] << 8) | val;

    ARRAYindexUIDcard++;
    EEPROMstartAddr += 5;
  }

  ARRAYindexUIDcard = 0;
  EEPROMstartAddr = 0;
  uidDec = 0;
  LockSwitch = 0;
  DisplayWAiT_CARD();
}

void DisplayWAiT_CARD() {
  lcd.clear();
  lcd.print(0, 0, "    SCAN THE   ");
  lcd.print(0, 1, "      CARD      ");
}

void check_btn() {
  // kiểm tra hệ thống cảnh báo được bật
  Serial.print("V1....");
  Serial.println(bt1_state);

  Serial.print("V2....");
  Serial.println(bt2_state);

  Serial.print("V3....");
  Serial.println(bt3_state);

  Serial.print("V4....");
  Serial.println(bt3_state);
  if (bt1_state == HIGH) {  //
    // Trường hợp: cảnh báo bật, btn tắt còi báo động tắt
    if (digitalRead(SENSOR) == HIGH && bt2_state == HIGH) {
      Serial.println("Warning....");
      digitalWrite(LED, HIGH);
    }

    if (digitalRead(SENSOR) == HIGH && bt2_state == LOW) {
      Serial.println("Off....");
      digitalWrite(LED, LOW);
    }

    if (digitalRead(LED) == HIGH) {
      Serial.println("Warning....");
      Blynk.virtualWrite(V4, HIGH);
      delay(10000);
      Blynk.virtualWrite(V4, LOW);
    }

    if (digitalRead(ELOCK) == HIGH) {
      Blynk.virtualWrite(V1, LOW);
      Blynk.virtualWrite(V2, LOW);
      Blynk.virtualWrite(V3, LOW);
      Blynk.virtualWrite(V4, LOW);
    }

    // Trường hợp
  } else {
    digitalWrite(LED, LOW);
    Blynk.virtualWrite(V1, LOW);
    Blynk.virtualWrite(V2, LOW);
    Blynk.virtualWrite(V3, LOW);
  }
}

BLYNK_WRITE(V1) {
  bt1_state = param.asInt();
}

BLYNK_WRITE(V2) {
  bt2_state = param.asInt();
}

BLYNK_WRITE(V3) {
  bt3_state = param.asInt();
}
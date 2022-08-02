#define BLYNK_TEMPLATE_ID "TMPLHet55qJa"
#define BLYNK_DEVICE_NAME "Antithief"
#define BLYNK_AUTH_TOKEN "B8mcIQ6cjhe_agA4Ptdkc2WJS5ujQxJO"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial


#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "621 Truong Chinh Lau 4.1";
char pass[] = "@93755457@";

// or Software Serial on Uno, Nano...
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(2, 3);  // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 38400

ESP8266 wifi(&EspSerial);
// define led connect
WidgetLED led_connect(V0);

// define variable in use
boolean bt1_state = LOW;  // => trạng thái bật/tắt hệ thống
boolean bt2_state = HIGH;  // => trạng thái bật/tắt còi báo động
boolean bt3_state = LOW;  // => trạng thái đóng/mở cửa

// define digital in use
#define SENSOR 6  //=> cảm biến hồng ngoại
#define LED 4     //=> còi báo hiệu
#define ELOCK 5   //=> Khóa điện

void setup() {
  // Debug console
  Serial.begin(115200);

  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);
  Blynk.begin(auth, wifi, ssid, pass, "blynk.cloud", 80);

  // set up LED
  pinMode(SENSOR, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(ELOCK, INPUT);
  digitalWrite(LED, LOW);
}

void loop() {
  Blynk.run();

  connect();

  check_btn();
}

void connect() {
  if (led_connect.getValue()) {
    led_connect.off();
  } else {
    led_connect.on();
  }
  delay(500);
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
  if (bt1_state == HIGH) {
    // Trường hợp: cảnh báo bật, btn tắt còi báo động tắt
    if (digitalRead(SENSOR) == HIGH && bt2_state == HIGH) {
      Serial.println("Warning....");
      digitalWrite(LED, HIGH);
      Blynk.virtualWrite(V4, HIGH);
    }

    if (digitalRead(SENSOR) == HIGH && bt2_state == LOW) {
      Serial.println("Off....");
      digitalWrite(LED, LOW);
      Blynk.virtualWrite(V4, LOW);
    }

    // Trường hợp
  } else {
    bt1_state = LOW;
    bt2_state = LOW;
    bt3_state = LOW;
    digitalWrite(LED, LOW);
  }
}

BLYNK_WRITE(V1) {
  int p = param.asInt();
  // Blynk.virtualWrite(V1, p);
  if (bt1_state == LOW) {
    bt1_state = p;
  } else {
    bt1_state = p;
  }
}

BLYNK_WRITE(V2) {
  int p = param.asInt();
  // Blynk.virtualWrite(V2, p);
  if (bt2_state == LOW) {
    bt2_state = HIGH;
  } else {
    bt2_state = LOW;
  }
}

BLYNK_WRITE(V3) {
  int p = param.asInt();
  // Blynk.virtualWrite(V3, p);
  if (bt3_state == LOW) {
    bt3_state = p;
    digitalWrite(ELOCK, HIGH);
  } else {
    bt3_state = p;
    digitalWrite(ELOCK, LOW);
  }
}
#include <SoftwareSerial.h>

#define Rx 11 // Định nghĩa chân 11 là Rx
#define Tx 12 // Định nghĩa chân 12 là Tx

SoftwareSerial mySerial(Rx, Tx); // Khởi tạo cổng serial mềm

// PIR - cảm biến hồng ngoại
#define PIR 6
#define PIR2 9
#define PIR3 10

// LED báo động
#define LED 13

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerial.begin(9600);
  // Set up LED
  pinMode(LED, OUTPUT);
  // Set up cảm biến
  pinMode(PIR, INPUT);
  pinMode(PIR2, INPUT);
  pinMode(PIR3, INPUT);
  // Set default value for OUTPUT
  digitalWrite(LED, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  int value = digitalRead(PIR);
  int value2 = digitalRead(PIR2);
  int value3 = digitalRead(PIR3);
  
  if (value == HIGH || value2 == HIGH || value3 == HIGH) {
    mySerial.print("PIR 1\r\n"); // Gửi tín hiệu phát hiện chuyển động
//    Serial.println("PIR on");
  } else {
//    Serial.println("PIR off");
  }

  if(mySerial.available()){
      digitalWrite(LED, LOW);
  } else {
      digitalWrite(LED, HIGH);
  }
}

// Cảm biến hồng ngoại
int PIR = 6;
int PIR2 = 9;
int PIR3 = 10;

// Đèn + còi báo
int LED = 12;
int LED2 = 13;

// Input từ ESP module
int ESOutPIR = 2;
int ESPInLED = 3;
int ESPInLOCK = 5;
int ESPInTIME = 11;

void setup() 
{
  // set up cảm biến
  Serial.begin(9600);
  pinMode(PIR, INPUT);
  pinMode(PIR2, INPUT);
  pinMode(PIR3, INPUT);
  // Set up LED
  pinMode(LED, OUTPUT);
  pinMode(LED2, OUTPUT);
  // Set up ES Module
  pinMode(ESPIn, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int value = digitalRead(PIR);
  int value2 = digitalRead(PIR2);
  int value3 = digitalRead(PIR3);
  digitalWrite(LED, LOW);
  digitalWrite(LED2, LOW);

  if (value == HIGH || value2 == HIGH || value3 == HIGH) {
    Serial.print("Giá trị của cảm biến là:");
    Serial.println(value);
    Serial.println(value2);
    digitalWrite(LED, HIGH);
    digitalWrite(LED2, HIGH);
  }
}
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "6hPLb91nWuGoSK9X9Innb3VHsXn_prer";
char ssid[] = "DTM E-SMART";
char pass[] = "0919890938";

#define ledSignal 2 //D4
#define relay     5 //D1
#define button    0 //D3
WidgetLED led(V0);
BlynkTimer timer;
boolean buttonState=HIGH;

void setup()
{
  Serial.begin(9600);
  pinMode(ledSignal,OUTPUT);
  pinMode(relay,OUTPUT);  //D4
  pinMode(button,INPUT_PULLUP);   //D3
  digitalWrite(ledSignal,HIGH);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L,blinkLedWidget);
}

void loop()
{
  Blynk.run();
  timer.run();
  if(digitalRead(button)==LOW){
    if(buttonState!=LOW){
      digitalWrite(relay,!digitalRead(relay));
      Blynk.virtualWrite(V1,digitalRead(relay));
    }
    buttonState=LOW;
  }else{
    buttonState=HIGH;
  }
}
BLYNK_CONNECTED() {
  Blynk.syncVirtual(V1);
}

void blinkLedWidget()
{
  if(Blynk.connected()==true){
    digitalWrite(ledSignal,!digitalRead(ledSignal));
  }
  if (led.getValue()) {
    led.off();
  } else {
    led.on();
  }
}
BLYNK_WRITE(V1)
{
  int pinValue = param.asInt();
  digitalWrite(relay,pinValue);
}

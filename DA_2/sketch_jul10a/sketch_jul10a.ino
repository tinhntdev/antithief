#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define BLYNK_TEMPLATE_ID "TMPLHet55qJa"
#define BLYNK_DEVICE_NAME "Antithief"
#define BLYNK_AUTH_TOKEN "B8mcIQ6cjhe_agA4Ptdkc2WJS5ujQxJO"

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "621 Truong Chinh Lau 4.1";
char pass[] = "@93755457@";

WidgetLED led1(V1);
WidgetLED led2(V2);

BlynkTimer timer;
// V1 LED Widget is blinking
void blinkLedWidget()
{
  if (led1.getValue()) {
    led1.off();
    Serial.println("LED on V1: off");
  } else {
    led1.on();
    Serial.println("LED on V1: on");
  }
}

BLYNK_WRITE(V0)
{
  int pinValue = param.asInt();

  Serial.print("V0 value is:");
  Serial.println(pinValue);

  digitalWrite(D2, pinValue);
  Blynk.virtualWrite(V2, pinValue);
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  timer.setInterval(1000L, blinkLedWidget);
  pinMode(D2, OUTPUT);
  digitalWrite(D2, 0);
}

void loop()
{
  Blynk.run();
  timer.run();
}
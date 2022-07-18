#define BLYNK_TEMPLATE_ID "TMPLHet55qJa"
#define BLYNK_DEVICE_NAME "Antithief"
#define BLYNK_AUTH_TOKEN "B8mcIQ6cjhe_agA4Ptdkc2WJS5ujQxJO"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "SerialCommand.h"
SerialCommand SCmd;
char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "621 Truong Chinh Lau 4.1";
char pass[] = "@93755457@";

BlynkTimer timer;

WidgetLED statusConnect(V1);

boolean isTurnOff;

void blinkLedWidget() {
  if (statusConnect.getValue()) {
    statusConnect.off();
    Serial.println("LED on V1: off");
  } else {
    statusConnect.on();
    Serial.println("LED on V1: on");
  }
}

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
//  int value = param.asInt();
  // Update state
  isTurnOff = param.asInt();
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
//  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
//  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
//  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

void setup()
{
  // Debug console
  Serial.begin(9600);
//  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
  // Setup a function to be called every second
  timer.setInterval(1000L, blinkLedWidget);
//  pinMode(D2, OUTPUT);
//  digitalWrite(D2, LOW);

  SCmd.addCommand("PIR", PIR_val, NULL, NULL, NULL);
}

void loop()
{
  Blynk.run();
  timer.run();
  SCmd.readString();
}

void PIR_val()
{
  int val;
  char *arg;
  arg = SCmd.next();
  if (arg != NULL) {
    val = atoi(arg);
    Serial.print("PIR value");
    Serial.println(val);
    Serial.print("isTurnOff value: ");
    Serial.println(isTurnOff);
    delay(3000);
    if (val == 1 && !isTurnOff) {
      Serial.write("LOCK");
    }
  }

}

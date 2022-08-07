#define BLYNK_TEMPLATE_ID "TMPLHet55qJa"
#define BLYNK_DEVICE_NAME "Antithief"
#define BLYNK_AUTH_TOKEN "B8mcIQ6cjhe_agA4Ptdkc2WJS5ujQxJO"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "621 Truong Chinh Lau 4.1";
char pass[] = "@93755457@";

BlynkTimer timer;

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{

}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{

  Blynk.virtualWrite(V2, millis() / 1000);
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  timer.setInterval(1000L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run();
}
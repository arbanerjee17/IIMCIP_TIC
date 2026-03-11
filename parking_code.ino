#define BLYNK_TEMPLATE_ID "TMPL3zG404Gif"
#define BLYNK_TEMPLATE_NAME "Smart Parking"
#define BLYNK_AUTH_TOKEN "wIYCRD_tbihCdpDaF-3WN5mFE8iAmcHn"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "realme 8";
char pass[] = "qwerrewq";

#define TRIG1 2
#define ECHO1 3

#define TRIG2 4
#define ECHO2 5

#define TRIG3 6
#define ECHO3 7

#define LED1 8
#define LED2 9

BlynkTimer timer;

long readDistance(int trigPin,int echoPin)
{
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);

  long duration = pulseIn(echoPin,HIGH);
  long distance = duration*0.034/2;

  return distance;
}

void checkParking()
{
  long d1 = readDistance(TRIG1,ECHO1);
  long d2 = readDistance(TRIG2,ECHO2);
  long d3 = readDistance(TRIG3,ECHO3);

  Serial.print("S1:");
  Serial.print(d1);
  Serial.print("  S2:");
  Serial.print(d2);
  Serial.print("  S3:");
  Serial.println(d3);

  int emptySlots = 0;

  /* SLOT 1 */

  if(d1 < 15)
  {
    digitalWrite(LED1,HIGH);
    Blynk.virtualWrite(V0,"Occupied");
  }
  else
  {
    digitalWrite(LED1,LOW);
    Blynk.virtualWrite(V0,"Empty");
    emptySlots++;
  }

  /* SLOT 2 */

  if(d2 < 15)
  {
    digitalWrite(LED2,HIGH);
    Blynk.virtualWrite(V1,"Occupied");
  }
  else
  {
    digitalWrite(LED2,LOW);
    Blynk.virtualWrite(V1,"Empty");
    emptySlots++;
  }

  /* SLOT 3 */

  if(d3 < 15)
  {
    Blynk.virtualWrite(V2,"Occupied");
  }
  else
  {
    Blynk.virtualWrite(V2,"Empty");
    emptySlots++;
  }

  /* SEND DATA TO BLYNK */

  Blynk.virtualWrite(V3,emptySlots);

  Blynk.virtualWrite(V4,d1);
  Blynk.virtualWrite(V5,d2);
  Blynk.virtualWrite(V6,d3);
}

void setup()
{
  Serial.begin(115200);

  pinMode(TRIG1,OUTPUT);
  pinMode(ECHO1,INPUT);

  pinMode(TRIG2,OUTPUT);
  pinMode(ECHO2,INPUT);

  pinMode(TRIG3,OUTPUT);
  pinMode(ECHO3,INPUT);

  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);

  Blynk.begin(BLYNK_AUTH_TOKEN,ssid,pass);

  timer.setInterval(1500L,checkParking);
}

void loop()
{
  Blynk.run();
  timer.run();
}
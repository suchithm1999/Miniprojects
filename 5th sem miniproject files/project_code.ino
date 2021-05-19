#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>


TinyGPS gps;
SoftwareSerial sgps(5,4);
WiFiClient client;


String MakerIFTTT_Key ;
String MakerIFTTT_Event;
char *append_str(char *here, String s) {  int i=0; while (*here++ = s[i]){i++;};return here-1;}
char *append_ul(char *here, unsigned long u) { char buf[20]; return append_str(here, ultoa(u, buf, 10));}
char post_rqst[256];char *p;char *content_length_here;char *json_start;int compi;
const char* ssid = "suchith";
const char* password = "suchithm";
const char* host = "maker.ifttt.com";
const int httpsPort = 443;
char PIN[14];
int led = 0;     // LED pin
int button = 16; // push button is connected
int temp = 0;    // temporary variable for reading the button pin status


void setup()
{
  Serial.begin(9600);
  pinMode(led, OUTPUT);   // declare LED as output
  pinMode(button, INPUT); // declare push button as input
  sgps.begin(9600);
  randomSeed( analogRead(A0) ); //randomize random seed value
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void loop()
{
  while (sgps.available())
  {
    int c = sgps.read();
    if (gps.encode(c))
    {
      float slat,slon;
      gps.f_get_position(&slat, &slon);
      Serial.print("Latitude :");
      Serial.println(slat, 6);
      Serial.print("Longitude:");
      Serial.println(slon, 6);
      temp = digitalRead(button);
      digitalWrite(led, LOW);
      if (temp == HIGH)
      {
        if (client.connect("maker.ifttt.com",80)) 
        {
          digitalWrite(led, HIGH);
          MakerIFTTT_Key ="XrOJoTMbLRljaQdk2vZI-";
          MakerIFTTT_Event ="noti";
          p = post_rqst;
          p = append_str(p, "POST /trigger/");
          p = append_str(p, MakerIFTTT_Event);
          p = append_str(p, "/with/key/");
          p = append_str(p, MakerIFTTT_Key);
          p = append_str(p, " HTTP/1.1\r\n");
          p = append_str(p, "Host: maker.ifttt.com\r\n");
          p = append_str(p, "Content-Type: application/json\r\n");
          p = append_str(p, "Content-Length: ");
          content_length_here = p;
          p = append_str(p, "NN\r\n");
          p = append_str(p, "\r\n");
          json_start = p;
          p = append_str(p, "{\"value1\":\"");
          p = append_str(p, String(slat,6));
          p = append_str(p, "\",\"value2\":\"");
          p = append_str(p, String(slon,6));
          p = append_str(p, "\",\"value3\":\"");
          p = append_str(p, "");
          p = append_str(p, "\"}");
      
          compi= strlen(json_start);
          content_length_here[0] = '0' + (compi/10);
          content_length_here[1] = '0' + (compi%10);
          client.print(post_rqst);
          delay(5000);
          digitalWrite(led, LOW);
        }
      }
    }
  }
}


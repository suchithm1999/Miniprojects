#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
TinyGPS gps;
SoftwareSerial sgps(5,4);
WiFiClient client;

String MakerIFTTT_Key ;
String MakerIFTTT_Event;
char *append_str(char *here, String s) {  int i=0; while (*here++ = s[i]){i++;};return here-1;}
char *append_ul(char *here, unsigned long u) { char buf[20]; return append_str(here, ultoa(u, buf, 10));}
char post_rqst[256];char *p;char *content_length_here;char *json_start;int compi;
const char* ssid = "Suchith Shetty";
const char* password = "suchithm";
const char* host = "maker.ifttt.com";
const int httpsPort = 443;
char PIN[14];
char auth[] = "3N5fTU5G_rU4pnn8DMOemodYOs3dTkBO";
char ssida[] = "Suchith Shetty";
char pass[] = "suchithm";
int base = 14;
int key = 16;
int key_status = 0;
int flag;
int led = 12;
int counter;
int led_status;

void setup()
{
  Serial.begin(9600);
  pinMode(key, INPUT);
  pinMode(base, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(A0, INPUT);
  digitalWrite(base,LOW);
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
  Blynk.begin(auth, ssida, pass);
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
          digitalWrite(base,LOW);
          key_status = digitalRead(key);
          flag = analogRead(A0);
          delay(2000);
          if ( (key_status == HIGH)  && (flag <=900)  )
            {
              do 
                {
                  digitalWrite(led,HIGH);
                  digitalWrite(base,HIGH);
                  delay(2000);
                  Serial.println("this is key mode on");
                  key_status = digitalRead(key);
                }
                while (key_status == LOW);
              digitalWrite(base,LOW);
              digitalWrite(led,LOW);
              delay(3000);
            }
          else if (  (key_status == LOW)  &&  (flag >=900)  )
            {
              led_status = digitalRead(led);
              if (led_status == LOW)
                {
                  if (client.connect("maker.ifttt.com",80)) 
                    {
                    MakerIFTTT_Key ="ELFzOCT38pbSYOoZG80q9";
                    MakerIFTTT_Event ="theft";
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
                    }
                  do
                    {
                      Serial.println("non keymode");
                      digitalWrite(base,HIGH);
                      key_status = digitalRead(key);
                      flag = analogRead(A0);
                      delay(1000);
                      do
                        {
                          Blynk.run();
                          base = digitalRead(14);
                        }
                        while (base == HIGH);
                      if (client.connect("maker.ifttt.com",80)) 
                        {
                          MakerIFTTT_Key ="ELFzOCT38pbSYOoZG80q9";
                          MakerIFTTT_Event ="lock";
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
                        }
                      break;
                      Blynk.begin(auth, ssida, pass);
                    }
                    while ((key_status == LOW));
                  digitalWrite(led,HIGH);
                }
            }
        }
    }
}

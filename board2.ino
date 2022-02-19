#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <string.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

const char* ssid = "psponk2736";
const char* password ="ponk1234";
const char* urlget = "https://exceed.pontakorn.dev/count/";
char str[50];
const int _size = 2*JSON_OBJECT_SIZE(4);
StaticJsonDocument<_size> JSONGet;

int green = 27;
int red = 32;
int num = 0;

void setup()
{
  lcd.begin();
  lcd.backlight();
  pinMode(red,OUTPUT);
  pinMode(green,OUTPUT);
  Serial.begin(9600);
}

void WiFi_Connect(){
  WiFi.disconnect();
  WiFi.begin(ssid,password);
  while(WiFi.status()!=WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting to WiFi.....");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());
}

void _get(){
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;

    http.begin(urlget);

    int httpCode = http.GET();

    if(httpCode == HTTP_CODE_OK){
      String payload = http.getString();
      DeserializationError err = deserializeJson(JSONGet,payload);
      if(err){
        Serial.print(F("deserializaJson() failed with code "));
        Serial.println(err.c_str());
      }
      else{
        num = JSONGet["amount"];
      }
    }
    else{
      Serial.println(httpCode);
      Serial.println("ERROR on HTTP Request");
    }
  }
  else{
    WiFi_Connect();
  }
}

void loop() {
  _get();
  lcd.clear();
  lcd.home();
  lcd.setCursor(0, 0);
  lcd.print("NUMBER OF PEOPLE");
  lcd.setCursor(7,1);
  lcd.print(num);
  if(num < 5){
    digitalWrite(green,HIGH);
    digitalWrite(red,LOW);
  }
  else{
    digitalWrite(green,LOW);
    digitalWrite(red,HIGH);
  }
  delay(10000);
}
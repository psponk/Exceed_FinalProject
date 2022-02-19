#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <string.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

const char* ssid = "psponk2736";
const char* password ="ponk1234";
const char* urlget = "https://exceed.pontakorn.dev/count/";
const char* urlpostleave = "https://exceed.pontakorn.dev/leave/";
char str[50];
const int _size = 2*JSON_OBJECT_SIZE(4);
StaticJsonDocument<_size> JSONGet;
StaticJsonDocument<_size> JSONPost;

int green = 27;
int red = 32;
int num = 0;
int receiv2 = 34;
int y = 0;
int em2 = 19;


void setup()
{
  lcd.begin();
  lcd.backlight();
  pinMode(receiv2, INPUT);
  pinMode(em2, OUTPUT);       
  pinMode(red,OUTPUT);
  pinMode(green,OUTPUT);
  Serial.begin(9600);
  xTaskCreatePinnedToCore(
    Task1,
    "Task 1",
    1024*32,
    NULL,
    1,
    NULL,
    0
  );
  xTaskCreatePinnedToCore(
    Task2,
    "Task 2",
    1024*32,
    NULL,
    1,
    NULL,
    0
  );
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

void _postleave(int a){
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;

    http.begin(urlpostleave);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", "Token 884a4d2cbc1daf6b9bd2a0b27d314b29c3e097e8");

    JSONPost["leave"] = a;

    serializeJson(JSONPost, str);
    int httpCode = http.POST(str);

    if(httpCode == HTTP_CODE_OK){
      String payload = http.getString();
      Serial.println(httpCode);
      Serial.println(payload);
    }
    else{
      Serial.println(httpCode);
      Serial.println("ERROR on HTTP Request");
    }
  }
  else{
    WiFi_Connect();
  }
  delay(100);
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

void Task1(void *parameter){
  while(1){
  _get();
  lcd.clear();
  lcd.home();
  lcd.setCursor(0, 0);
  lcd.print("NUMBER OF PEOPLE");
  lcd.setCursor(7,1);
  lcd.print(num);
  if(num < 8){
    digitalWrite(green,HIGH);
    digitalWrite(red,LOW);
  }
  else{
    digitalWrite(green,LOW);
    digitalWrite(red,HIGH);
  }
  delay(1500);
  vTaskDelay(1 / portTICK_PERIOD_MS);
  }
}

void Task2(void *parameter){
  int status2 = 1; //0 off 1 on
  while(1){
    y = analogRead(receiv2);
    delay(130);
    if(y > 4094 && status2 > 0){
      _postleave(1);
      status2 = 0;
    }
    if(y < 1000){
      status2 = 1;
    }
    //Serial.println(y);
    vTaskDelay(1 / portTICK_PERIOD_MS);
  }
}

void loop() {
  for(int i=0; i < 50; i++)
    {
      digitalWrite (em2,HIGH);
      digitalWrite (em2,LOW);
    }
}
#include <IRremote.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <string.h>

const char* ssid = "psponk2736";
const char* password ="ponk1234";
const char* urlpostenter = "https://exceed.pontakorn.dev/enter/";
const char* urlpostleave = "https://exceed.pontakorn.dev/leave/";
const int _size = 2*JSON_OBJECT_SIZE(4);
char str[50];
StaticJsonDocument<_size> JSONPost;

int receiv1 = 34;
int receiv2 = 33;
int x = 0;
int y = 0;
int people = 0;
int em1 = 18;
int em2 = 19;


void setup() {
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
  Serial.begin(9600);
  pinMode(receiv1, INPUT);
  pinMode(receiv2, INPUT);
  pinMode(em1, OUTPUT);
  pinMode(em2, OUTPUT);
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

void _postenter(int a){
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;

    http.begin(urlpostenter);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", "Token 884a4d2cbc1daf6b9bd2a0b27d314b29c3e097e8");

    JSONPost["enter"] = a;

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

void Task1(void *parameter){
  int status1 = 1; //0 off 1 on
  while(1){
    x = analogRead(receiv1);
    delay(95);
    if(x > 4094 && status1 > 0){
      people += 1;
      _postenter(1);
      status1 = 0;
      Serial.println(people);
    }
    if(x < 1000){
      status1 = 1;
    }
    //Serial.println(x);
    vTaskDelay(1 / portTICK_PERIOD_MS);
  } 
}

void Task2(void *parameter){
  int status2 = 1; //0 off 1 on
  while(1){
    y = analogRead(receiv2);
    delay(130);
    if(y > 4094 && status2 > 0){
      people -= 1;
      _postleave(1);
      status2 = 0;
      Serial.println(people);
    }
    if(y < 1000){
      status2 = 1;
    }
    if(people < 0){
      people = 0;
    }
    //Serial.println(y);
    vTaskDelay(1 / portTICK_PERIOD_MS);
  } 
}


void loop() {
  for(int i=0; i < 50; i++)
    {
      digitalWrite (em1,HIGH);
      digitalWrite (em2,HIGH);
      digitalWrite (em1,LOW);
      digitalWrite (em2,LOW);
    }
}

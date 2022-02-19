#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <string.h>

const char* ssid = "psponk2736";
const char* password ="ponk1234";
const char* urlpostenter = "https://exceed.pontakorn.dev/enter/";
const int _size = 2*JSON_OBJECT_SIZE(4);
char str[50];
StaticJsonDocument<_size> JSONPost;

int receiv1 = 34;
int x = 0;
int em1 = 18;
f
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
  Serial.begin(9600);
  pinMode(receiv1, INPUT);
  pinMode(em1, OUTPUT);
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

void Task1(void *parameter){
  int status1 = 1; //0 off 1 on
  while(1){
    x = analogRead(receiv1);
    delay(95);
    if(x > 4094 && status1 > 0){
      _postenter(1);
      status1 = 0;
    }
    if(x < 1000){
      status1 = 1;
    }
    //Serial.println(x);
    vTaskDelay(1 / portTICK_PERIOD_MS);
  } 
}

void loop() {
  for(int i=0; i < 50; i++)
    {
      digitalWrite (em1,HIGH);
      digitalWrite (em1,LOW);
    }
}

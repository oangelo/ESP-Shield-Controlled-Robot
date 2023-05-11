//Wifi SSID and other definitions will be using this name
#define RobotName "Robot1"

#if defined(ESP32)
#include <WiFi.h>
#include <SPIFFS.h>
#define DEVICE "ESP32"
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#define DEVICE "ESP8266"
#endif

#include <WiFiClient.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>
#include <WiFiUdp.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebSrv.h>

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
// Create an Event Source on /events
AsyncEventSource events("/events");

//Pins definitions
#define RightMotorSpeed 5
#define RightMotorDir   0
#define LeftMotorSpeed  4
#define LeftMotorDir    2

String command;             //String to store app command state.
int speedCar = 800;         // 400 - 1023.
int speed_Coeff = 3;

//variáveis de controle dos motores
int minRange = 300;
int maxRange = 900;
int minSpeed = 450;
int maxSpeed = 1020;
int noSpeed = 0;


void OTA() {
  ArduinoOTA.setHostname(RobotName);
  ArduinoOTA
  .onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  })
  .onEnd([]() {
    Serial.println("\nEnd");
  })
  .onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  })
  .onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
}

// Initialize SPIFFS
void initSPIFFS() {
  if (!SPIFFS.begin()) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");
}

void setup() {

  Serial.begin(115200);
  Serial.println("Booting");
  initSPIFFS();

  // setting motor pins as outputs
  pinMode(RightMotorSpeed, OUTPUT);
  pinMode(LeftMotorSpeed, OUTPUT);
  pinMode(RightMotorDir, OUTPUT);
  pinMode(LeftMotorDir, OUTPUT);

  // setting motor pins to LOW
  digitalWrite(RightMotorSpeed, LOW);
  digitalWrite(LeftMotorSpeed, LOW);
  digitalWrite(RightMotorDir, HIGH);
  digitalWrite(LeftMotorDir, HIGH);

  // generating WiFi
  WiFi.mode(WIFI_AP);
  WiFi.softAP(RobotName);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  OTA();

  // Web Server Root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.serveStatic("/", SPIFFS, "/");

  // Request for the latest sensor readings
  server.on("/control", HTTP_GET, [](AsyncWebServerRequest * request) {
    int paramsNr = request->params();
    Serial.println(paramsNr);

    for (int i = 0; i < paramsNr; i++)
    {
      AsyncWebParameter* p = request->getParam(i);
      Serial.print("Param name: ");
      Serial.println(p->name());

      Serial.print("Param value: ");
      Serial.println(p->value());

      Serial.println("------");
    }

    AsyncWebParameter* p = request->getParam(0);
    if( p->name()=="State" ){
      switch (p->value()[0]) {
        case 'F': goAhead(); break;
        case 'B': goBack(); break;
        case 'L': goLeft(); break;
        case 'R': goRight(); break;
        case 'I': goAheadRight(); break;
        case 'G': goAheadLeft(); break;
        case 'J': goBackRight(); break;
        case 'H': goBackLeft(); break;
        case '0': speedCar = 400; break;
        case '1': speedCar = 470; break;
        case '2': speedCar = 540; break;
        case '3': speedCar = 610; break;
        case '4': speedCar = 680; break;
        case '5': speedCar = 750; break;
        case '6': speedCar = 820; break;
        case '7': speedCar = 890; break;
        case '8': speedCar = 960; break;
        case '9': speedCar = 1023; break;
        case 'S': stopRobot(); break;
        default: break;
      }
    }
    request->send(200, "text/plain", "");
  });

  events.onConnect([](AsyncEventSourceClient * client) {
    if (client->lastId()) {
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);

  // Start server
  server.begin();


  Serial.println("Ready");
}

void goAhead() {

  Serial.println("goAhead");

  digitalWrite(RightMotorDir, HIGH);
  digitalWrite(LeftMotorDir, HIGH);
  analogWrite(RightMotorSpeed, maxSpeed);
  analogWrite(LeftMotorSpeed, maxSpeed);
}

void goBack() {
  Serial.println("goBack");
  digitalWrite(RightMotorDir, LOW);
  digitalWrite(LeftMotorDir, LOW);
  analogWrite(RightMotorSpeed, maxSpeed);
  analogWrite(LeftMotorSpeed, maxSpeed);

}

void goRight() {
  Serial.println("goRight");
  digitalWrite(RightMotorDir, HIGH);
  digitalWrite(LeftMotorDir, HIGH);
  analogWrite(RightMotorSpeed, minSpeed);
  analogWrite(LeftMotorSpeed, maxSpeed);


}

void goLeft() {
  Serial.println("goLeft");
  digitalWrite(RightMotorDir, HIGH);
  digitalWrite(LeftMotorDir, HIGH);
  analogWrite(RightMotorSpeed, maxSpeed);
  analogWrite(LeftMotorSpeed, minSpeed);

}

void goAheadRight() {
  Serial.println("goAheadRight");
  digitalWrite(RightMotorDir, HIGH);
  digitalWrite(LeftMotorDir, HIGH);
  analogWrite(RightMotorSpeed, minSpeed);
  analogWrite(LeftMotorSpeed, maxSpeed);

}

void goAheadLeft() {
  Serial.println("goAheadLeft");
  digitalWrite(RightMotorDir, LOW);
  digitalWrite(LeftMotorDir, LOW);
  analogWrite(RightMotorSpeed, maxSpeed);
  analogWrite(LeftMotorSpeed, minSpeed);

}

void goBackRight() {
  Serial.println("goBackRight");
  digitalWrite(RightMotorDir, LOW);
  digitalWrite(LeftMotorDir, LOW);
  analogWrite(RightMotorSpeed, minSpeed);
  analogWrite(LeftMotorSpeed, maxSpeed);

}

void goBackLeft() {
  Serial.println("goBackLeft");
  digitalWrite(RightMotorDir, LOW);
  digitalWrite(LeftMotorDir, LOW);
  analogWrite(RightMotorSpeed, maxSpeed);
  analogWrite(LeftMotorSpeed, minSpeed);

}

void stopRobot() {
  Serial.println("stopRobot");
  analogWrite(RightMotorSpeed, noSpeed);
  analogWrite(LeftMotorSpeed, noSpeed);
}

void loop() {
  ArduinoOTA.handle();
}

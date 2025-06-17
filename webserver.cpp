#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <html/home.html>
#include <vector>

std::vector<String> scannedNet;
std::vector<String> storeNetworks;

char* ssid = "pwned";
char* password = "password";
ESP8266WebServer server(80);

void handleRoot() {
  String html = home.html;
  server.send(200, "text/html", html);
}

void inputHandler() {
  if (server.hasArg("input")) {
    String input = server.arg("input");
    Serial.println("Received input: " + input);
    server.send(200, "text/plain", "200 ok" + input);
  } else {
    server.send(400, "text/plain", "error 400");
  }
}

void routerIPHandler() {
if (server.hasArg("routerIP")) {
    String routerIP = server.arg("routerIP");
    Serial.println("Received input: " + routerIP);
    server.send(200, "text/plain", "200 ok" + routerIP);
  } else {
    server.send(400, "text/plain", "error 400");
  }
}

void printOut(){
  String html = "<html><head><title>ESP8266 Data</title></head><body>";
  html += "<h1>Wi-Fi Scan Results</h1>";
  html += "<p>Found " + String(scannedNet.size()) + " networks:</p>";
  html += "<ul>";
  
  for (const auto& network : storeNetworks) {
    html += "<li>" + network + "</li>";
  }
  
  html += "</ul>";
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200); // pls dont touch this sec
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  Serial.println("goodies sta");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected, IP: " + WiFi.localIP().toString()); /// pls dont touch 
  
  server.on("/", handleRoot);
  server.on("/input", inputHandler);
  server.on("/monitor", printOut);
  server.begin();
  Serial.println("Web server started");
}


void loop() {
  server.handleClient();

  scannedNet = WiFi.scanNetworks();

  if (scannedNet == 0) {
    Serial.println("No networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    
    for (int i = 0; i < scannedNet; ++i) {
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(" dBm)");
      Serial.print(" [");
      Serial.print(getEncryptionType(WiFi.encryptionType(i)));
      Serial.println("]");
      delay(10);
      storeNetworks.push_back(WiFi.SSID());
      delay(10);
    }
  }
}
#include <ESP8266WiFi.h>

#include "globalVar.h"

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
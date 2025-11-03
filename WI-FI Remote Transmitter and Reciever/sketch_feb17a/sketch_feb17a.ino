#include <WiFi.h>
#include <esp_now.h>

#define BUTTON_PIN 0  // Button pin to send data on press

// Receiver MAC address
uint8_t receiverAddress[] = { 0xCC, 0xDB, 0xA7, 0x2E, 0x09, 0xEC };  // Change this to your receiver's MAC address

// Data structure for sending
typedef struct struct_message {
  char msg[32];
} struct_message;

struct_message outgoingMessage;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);  // Set device as station (client)

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Initialization failed");
    return;
  }

  // Register the peer (receiver) device
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  peerInfo.channel = 0;  // Default channel
  peerInfo.encrypt = false;  // No encryption
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // Setup button for triggering data send
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  // Check if the button is pressed
  if (digitalRead(BUTTON_PIN) == LOW) {
    // Send "Hello World" message to receiver
    strcpy(outgoingMessage.msg, "Hello World");
    esp_now_send(receiverAddress, (uint8_t *)&outgoingMessage, sizeof(outgoingMessage));
    Serial.println("Sent: Hello World");

    delay(1000);  // Debounce button press
  }
}

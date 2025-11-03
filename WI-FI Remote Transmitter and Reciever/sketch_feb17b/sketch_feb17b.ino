#include <WiFi.h>
#include <esp_now.h>

#define LED_PIN 2  // Built-in LED pin (usually GPIO 2 on ESP32)

bool ledState = false;
unsigned long lastBlinkTime = 0;
unsigned long blinkInterval = 500; // 500ms blink interval

// Data structure for receiving
typedef struct struct_message {
  char msg[32];
} struct_message;

struct_message incomingMessage;

// Callback function to handle received data
void onDataReceived(const esp_now_recv_info_t *recv_info, const uint8_t *data, int len) {
  // Copy the incoming data into the message structure
  memcpy(&incomingMessage, data, sizeof(incomingMessage));
  Serial.print("Received: ");
  Serial.println(incomingMessage.msg);

  if (strcmp(incomingMessage.msg, "Hello World") == 0) {
    // If "Hello World" is received, start blinking the LED
    lastBlinkTime = millis();
    ledState = true;  // Start blinking
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);  // Set device as station (client)

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Initialization failed");
    return;
  }

  // Register the callback function for received data
  esp_now_register_recv_cb(onDataReceived);

  // Set LED pin mode
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // Blink the LED if the message "Hello World" is received
  if (ledState) {
    unsigned long currentMillis = millis();
    if (currentMillis - lastBlinkTime >= blinkInterval) {
      lastBlinkTime = currentMillis;
      digitalWrite(LED_PIN, !digitalRead(LED_PIN)); // Toggle LED state
    }
  }
}

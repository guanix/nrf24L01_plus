#include <SPI.h>
#include <nrf24.h>

uint8_t rx_mac[5] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x00 };
uint8_t tx_mac[5] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x01 };

void setup() {
  Serial.begin(115200);
  Serial.println(F("sender"));
  // pin 2 is CE, pin 5 is CSN
  nrf24_init(2, 5);
  
  nrf24_config(2, 1);
  nrf24_rx_address(rx_mac);
  nrf24_tx_address(tx_mac);
  
  Serial.println(F("ready to send"));
}

uint8_t counter = 0;

void loop() {
  counter++;
  nrf24_send(&counter);
  while (nrf24_isSending());
  Serial.println(F("sent packet"));
  
  uint8_t status = nrf24_lastMessageStatus();
  if (status == NRF24_TRANSMISSION_OK) {
    Serial.println(F("transmission ok"));
    uint8_t count = nrf24_retransmissionCount();
    Serial.print(F("retransmission count: "));
    Serial.println(count);
  } else if (status == NRF24_MESSAGE_LOST) {
    Serial.println(F("message lost"));
  }
  
  // Turn receive back on after transmit
  nrf24_powerUpRx();

  unsigned long receiveStart = millis();
 
  // Try to listen for a response
  while (millis() - receiveStart <= 500 && !nrf24_dataReady());
  
  if (nrf24_dataReady()) {
    uint8_t response;
    nrf24_getData(&response);
    Serial.print(F("received response: "));
    Serial.println(response);
  } else {
    Serial.println(F("no response"));
  }
  
  delay(3000);
}


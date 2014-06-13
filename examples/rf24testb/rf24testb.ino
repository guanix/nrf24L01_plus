#include <SPI.h>
#include <nrf24.h>

uint8_t rx_mac[5] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x01 };
uint8_t tx_mac[5] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x00 };

void setup() {
  Serial.begin(115200);
  Serial.println(F("receiver"));
  // pin 2 is CE, pin 5 is CSN
  nrf24_init(2, 5);
  
  nrf24_config(2, 1);
  
  nrf24_rx_address(rx_mac);
  nrf24_tx_address(tx_mac);
  
  Serial.println(F("ready to receive"));
}

void loop() {
  uint8_t data;
  if (nrf24_dataReady()) {
    nrf24_getData(&data);
    Serial.print(F("received data: "));
    Serial.println(data);
    
    Serial.print(F("sending response..."));
    nrf24_send(&data);
    while (!nrf24_isSending());
    Serial.println(F(" sent"));
    
    nrf24_powerUpRx();
  }
}


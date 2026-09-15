#include <DHT.h>

const byte ldrPin = A0;
const byte dhtPin = 13;   // D7
const byte relayPin = 12; // D6

#define DHTTYPE DHT11
DHT dht(dhtPin, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  // Matikan Relay di awal menggunakan trik High-Impedance
  pinMode(relayPin, INPUT); 
  
  Serial.println("--- Sistem Smart Warehouse Dimulai ---");
}

void loop() {
  delay(2500); // Jeda pembacaan DHT11

  int ldrValue = analogRead(ldrPin);
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (isnan(temp) || isnan(hum)) {
    Serial.println("Peringatan: Gagal membaca DHT!");
    return;
  }

  Serial.print("Suhu: "); Serial.print(temp);
  Serial.print(" °C | ADC LDR: "); Serial.println(ldrValue);

  // RULE ENGINE
  // Kondisi buruk: Suhu > 34°C ATAU LDR ditutup (Nilai ADC > 800)
  if (temp > 34.0 || ldrValue > 800) {
    
    // NYALAKAN RELAY (Beri perintah LOW)
    pinMode(relayPin, OUTPUT);
    digitalWrite(relayPin, LOW);
    Serial.println("--> PERINGATAN! Kondisi Buruk -> RELAY ON (LED Hijau Nyala)");
    
  } else {
    
    // MATIKAN RELAY (Gunakan trik INPUT)
    pinMode(relayPin, INPUT);
    Serial.println("--> AMAN. Kondisi Normal -> RELAY OFF (LED Hijau Mati)");
    
  }
  Serial.println("----------------------------------------------");
}
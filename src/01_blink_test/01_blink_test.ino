/*
 * 01_blink_test.ino
 * โค้ดทดสอบไฟ LED กะพริบบนบอร์ด DOIT ESP32 DEVKIT V1 (พิน GPIO 2)
 */

#define LED_PIN 2

void setup() {
  // เริ่มต้น Serial Port ที่ Baud rate 115200
  Serial.begin(115200);
  
  // กำหนดให้ขา LED_PIN ทำงานเป็น Output
  pinMode(LED_PIN, OUTPUT);
  
  Serial.println("\n==================================");
  Serial.println("  ESP32 GPIO 2 Blink Test Started ");
  Serial.println("==================================");
}

void loop() {
  // สั่งเปิดไฟ LED
  digitalWrite(LED_PIN, HIGH);
  Serial.println("[Status] LED is ON");
  delay(1000); // รอ 1 วินาที

  // สั่งปิดไฟ LED
  digitalWrite(LED_PIN, LOW);
  Serial.println("[Status] LED is OFF");
  delay(1000); // รอ 1 วินาที
}

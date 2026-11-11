/*
 * 02_touch_sensor.ino
 * โค้ดอ่านค่า Capacitive Touch Sensor บนพิน GPIO 4 (Touch 0) สั่งเปิด-ปิดไฟ LED (GPIO 2)
 * บอร์ด: DOIT ESP32 DEVKIT V1
 */

#define LED_PIN 2
#define TOUCH_PIN 4
#define TOUCH_THRESHOLD 35 // เกณฑ์ค่าสัมผัส (ปกติไม่ได้สัมผัสจะอ่านได้ประมาณ 70-100 เมื่อแตะค่าจะลดลงต่ำกว่า 30)

bool ledState = false;
bool lastTouchState = false;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.println("\n========================================");
  Serial.println("  ESP32 Touch Sensor Controller (GPIO 4)");
  Serial.println("========================================");
  Serial.println("Tip: แตะที่สายไฟหรือขา GPIO 4 เพื่อเปิด-ปิดไฟ LED");
}

void loop() {
  // อ่านค่าสนามไฟฟ้าประจุจากขา Touch Sensor
  int touchValue = touchRead(TOUCH_PIN);
  bool isTouched = (touchValue < TOUCH_THRESHOLD);

  // ตรวจจับการแตะครั้งใหม่ (Debounce / Edge Detection เพื่อสลับสถานะ)
  if (isTouched && !lastTouchState) {
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState ? HIGH : LOW);
    
    Serial.printf("[Touch Triggered] Raw Value: %d | LED State: %s\n", 
                  touchValue, ledState ? "ON (เปิด)" : "OFF (ปิด)");
    
    // หน่วงเวลาเล็กน้อยเพื่อป้องกันสัญญาณรบกวนเด้งซ้ำ (Debounce)
    delay(250);
  }

  lastTouchState = isTouched;
  delay(50);
}

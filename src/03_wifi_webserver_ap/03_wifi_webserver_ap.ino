/*
 * 03_wifi_webserver_ap.ino
 * โค้ด Web Server แบบ SoftAP (Access Point Mode)
 * - ปล่อยสัญญาณ Wi-Fi ชื่อ: "ESP32-SmartHome"
 * - เข้าหน้าควบคุมผ่าน Web Browser: http://192.168.4.1
 * - สั่งเปิด-ปิดไฟ LED ประจำบอร์ด (GPIO 2)
 * บอร์ด: DOIT ESP32 DEVKIT V1
 */

#include <WiFi.h>
#include <WebServer.h>

#define LED_PIN 2

// ตั้งค่าชื่อ Wi-Fi (SSID) และรหัสผ่าน (ปล่อยว่างไว้เป็น Open Wi-Fi หรือใส่รหัสผ่านขั้นต่ำ 8 ตัวอักษร)
const char* ssid = "ESP32-SmartHome";
const char* password = ""; // เช่น "12345678"

WebServer server(80);
bool ledState = false;

// ฟังก์ชันสร้างหน้าเว็บ HTML แบบ Responsive สวยงามทันสมัย
String generateHTML() {
  String html = "<!DOCTYPE html><html lang='th'>";
  html += "<head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>ESP32 Smart Home Control</title>";
  html += "<style>";
  html += "body { font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Helvetica, Arial, sans-serif; background: #0f172a; color: #f8fafc; text-align: center; margin: 0; padding: 24px; }";
  html += ".container { max-width: 420px; margin: 30px auto; background: #1e293b; border-radius: 20px; padding: 36px 24px; box-shadow: 0 20px 25px -5px rgba(0,0,0,0.5), 0 8px 10px -6px rgba(0,0,0,0.5); border: 1px solid #334155; }";
  html += "h1 { margin: 0 0 8px; font-size: 26px; color: #38bdf8; font-weight: 700; }";
  html += ".subtitle { color: #94a3b8; font-size: 14px; margin-bottom: 28px; }";
  html += ".status-box { background: #0f172a; border-radius: 12px; padding: 18px; margin-bottom: 28px; border: 1px solid #334155; }";
  html += ".status-label { font-size: 14px; color: #94a3b8; margin-bottom: 6px; }";
  html += ".badge { display: inline-block; font-size: 18px; font-weight: 700; padding: 6px 20px; border-radius: 9999px; }";
  html += ledState ? ".badge { background: #166534; color: #4ade80; border: 1px solid #22c55e; }" 
                   : ".badge { background: #991b1b; color: #f87171; border: 1px solid #ef4444; }";
  html += ".btn-group { display: flex; flex-direction: column; gap: 14px; }";
  html += ".btn { display: block; padding: 16px; font-size: 18px; font-weight: 600; text-decoration: none; border-radius: 12px; transition: all 0.2s ease; border: none; cursor: pointer; }";
  html += ".btn-on { background: #22c55e; color: #ffffff; box-shadow: 0 4px 14px rgba(34,197,94,0.35); }";
  html += ".btn-off { background: #ef4444; color: #ffffff; box-shadow: 0 4px 14px rgba(239,68,68,0.35); }";
  html += ".btn:hover { transform: translateY(-2px); filter: brightness(1.1); }";
  html += ".btn:active { transform: translateY(0); }";
  html += ".info { margin-top: 28px; font-size: 12px; color: #64748b; line-height: 1.6; }";
  html += "</style></head><body>";
  html += "<div class='container'>";
  html += "<h1>🏠 ESP32 Smart Home</h1>";
  html += "<div class='subtitle'>Local SoftAP Control Hub</div>";
  html += "<div class='status-box'>";
  html += "<div class='status-label'>สถานะไฟ LED (GPIO 2)</div>";
  html += "<div class='badge'>" + String(ledState ? "● เปิดอยู่ (ON)" : "○ ปิดอยู่ (OFF)") + "</div>";
  html += "</div>";
  html += "<div class='btn-group'>";
  html += "<a href='/led/on' class='btn btn-on'>💡 เปิดไฟ (Turn ON)</a>";
  html += "<a href='/led/off' class='btn btn-off'>🔌 ปิดไฟ (Turn OFF)</a>";
  html += "</div>";
  html += "<div class='info'>บอร์ด: DOIT ESP32 DEVKIT V1<br>IP Address: 192.168.4.1 | Port 80</div>";
  html += "</div></body></html>";
  return html;
}

// Handler สำหรับหน้าแรก (/)
void handleRoot() {
  server.send(200, "text/html", generateHTML());
}

// Handler สำหรับคำสั่งเปิดไฟ (/led/on)
void handleLedOn() {
  ledState = true;
  digitalWrite(LED_PIN, HIGH);
  Serial.println("[Web Server] คำสั่ง: เปิดไฟ LED (GPIO 2)");
  server.sendHeader("Location", "/");
  server.send(303); // Redirect กลับหน้าแรก
}

// Handler สำหรับคำสั่งปิดไฟ (/led/off)
void handleLedOff() {
  ledState = false;
  digitalWrite(LED_PIN, LOW);
  Serial.println("[Web Server] คำสั่ง: ปิดไฟ LED (GPIO 2)");
  server.sendHeader("Location", "/");
  server.send(303); // Redirect กลับหน้าแรก
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.println("\n============================================");
  Serial.println("  ESP32 SoftAP Web Server Initialization   ");
  Serial.println("============================================");

  // เริ่มกระจายสัญญาณ Access Point
  WiFi.mode(WIFI_AP);
  if (strlen(password) > 0) {
    WiFi.softAP(ssid, password);
  } else {
    WiFi.softAP(ssid);
  }

  IPAddress myIP = WiFi.softAPIP();
  Serial.printf("[WiFi] ปล่อยสัญญาณ SSID: %s\n", ssid);
  Serial.print("[WiFi] IP Address: ");
  Serial.println(myIP);

  // กำหนดเส้นทาง URL
  server.on("/", handleRoot);
  server.on("/led/on", handleLedOn);
  server.on("/led/off", handleLedOff);

  server.begin();
  Serial.println("[HTTP] Web Server พร้อมทำงานที่พอร์ต 80");
  Serial.println("เปิดเบราว์เซอร์แล้วพิมพ์ http://192.168.4.1 เพื่อสั่งงาน");
}

void loop() {
  server.handleClient();
}

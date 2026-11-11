# ESP32 Smart Home Starter

โปรเจกต์รวบรวมตัวอย่างโค้ดเริ่มต้น (Starter Sketches) และคู่มือสรุปขั้นตอนการตั้งค่าบอร์ด **ESP32** บนระบบปฏิบัติการ **Ubuntu Linux (24.04 / 26.04 LTS)** ตั้งแต่การติดตั้งเครื่องมือ การตั้งค่า Permissions จนถึงเทคนิคฮาร์ดแวร์เพื่อแก้ปัญหาการเบิร์นโค้ด

---

## 📋 สเปกฮาร์ดแวร์ (Hardware Specifications)

* **โมเดลบอร์ด**: DOIT ESP32 DEVKIT V1 (รุ่น 30 ขา)
* **ไมโครคอนโทรลเลอร์**: ESP-WROOM-32 (Tensilica Xtensa Dual-Core 32-bit LX6 @ 240 MHz)
* **หน่วยความจำ**: 520 KB SRAM / 4 MB Flash Memory
* **ชิป USB-to-UART**: Silicon Labs **CP2102**
* **พอร์ตเชื่อมต่อ**: Micro-USB (รองรับทั้งไฟเลี้ยง 5V และสื่อสารข้อมูล Serial)
* **การเชื่อมต่อไร้สาย**: Wi-Fi 802.11 b/g/n (2.4 GHz) + Bluetooth v4.2 BR/EDR & BLE
* **ไฟ LED แสดงผลประจำบอร์ด**: ขา **GPIO 2** (Active HIGH)
* **แรงดันใช้งานระดับลอจิก**: **3.3V** (GPIO ไม่รองรับ 5V Tolerance)

---

## 📁 โครงสร้างโปรเจกต์ (Project Structure)

```text
esp32-smarthome-starter/
├── .gitignore
├── README.md
└── src/
    ├── 01_blink_test/
    │   └── 01_blink_test.ino          # โค้ดทดสอบไฟ LED GPIO 2 กะพริบ
    ├── 02_touch_sensor/
    │   └── 02_touch_sensor.ino        # โค้ดอ่าน Capacitive Touch (GPIO 4) สลับสถานะ LED
    └── 03_wifi_webserver_ap/
        └── 03_wifi_webserver_ap.ino   # โค้ด SoftAP Web Server คุมเปิด-ปิดไฟผ่าน 192.168.4.1
```

---

## 🐧 การเตรียมระบบและแก้ปัญหาบน Ubuntu 24.04 / 26.04 LTS

### 1. การติดตั้ง FUSE และการรัน Arduino IDE AppImage
บน Ubuntu 24.04 และเวอร์ชันใหม่กว่า มีการเปลี่ยนผ่านสถาปัตยกรรม 64-bit time (`t64`) และการจำกัดสิทธิ์ AppArmor บน Electron AppImage:

* **ติดตั้งไลบรารี FUSE 2:**
  ```bash
  # สำหรับ Ubuntu 24.04 หรือใหม่กว่า
  sudo apt install -y libfuse2t64

  # (สำหรับ Ubuntu 22.04 หรือเก่ากว่า ใช้: sudo apt install -y libfuse2)
  ```

* **การรัน AppImage ด้วยแฟล็ก `--no-sandbox`:**
  เนื่องจาก Electron บน Ubuntu เวอร์ชันใหม่มีนโยบายความปลอดภัยของระบบคุม User Namespace หากเปิด AppImage แล้วหน้าต่างไม่ขึ้น ให้เรียกใช้งานด้วยแฟล็ก:
  ```bash
  ~/Applications/arduino-ide.AppImage --no-sandbox
  ```
  *(หรือระบุในไฟล์ `.desktop` ที่บรรทัด `Exec=/home/<USER>/Applications/arduino-ide.AppImage --no-sandbox`)*

---

### 2. การปลดล็อกสิทธิ์พอร์ต Serial USB (`dialout`)
ตามค่าเริ่มต้นของ Ubuntu สิทธิ์การเข้าถึงอุปกรณ์ Serial (`/dev/ttyUSB*` หรือ `/dev/ttyACM*`) จะถูกจำกัดไว้สำหรับกลุ่ม `dialout`:

1. เพิ่มบัญชีผู้ใช้ปัจจุบันเข้ากลุ่ม `dialout`:
   ```bash
   sudo usermod -a -G dialout $USER
   ```
2. **สำคัญมาก:** หลังจากรันคำสั่งแล้ว ต้องทำการ **Log out** ออกจากระบบ หรือ **Reboot** เครื่อง 1 ครั้ง เพื่อให้ระบบรีเฟรชสิทธิ์กลุ่มใหม่
3. ตรวจสอบสถานะการเป็นสมาชิกในกลุ่ม:
   ```bash
   groups $USER
   # ต้องมีคำว่า dialout ปรากฏอยู่ในผลลัพธ์
   ```

---

## ⚙️ การตั้งค่าใน Arduino IDE

1. **เพิ่ม Board Manager URL:**
   * เปิด Arduino IDE ไปที่เมนู `File` -> `Preferences` (หรือกด `Ctrl + ,`)
   * ในช่อง **Additional boards manager URLs** ใส่ลิงก์:
     ```text
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
2. **ติดตั้ง Core ESP32:**
   * ไปที่แท็บ **Boards Manager** (ไอคอนบอร์ดด้านซ้าย)
   * ค้นหาคำว่า `esp32` โดย **Espressif Systems** แล้วคลิก **Install**
3. **กำหนดค่าบอร์ดสำหรับการอัปโหลด:**
   * **Board**: `DOIT ESP32 DEVKIT V1`
   * **Port**: `/dev/ttyUSB0` (หรือพอร์ตที่ปรากฏเมื่อเสียบสาย USB)
   * **Upload Speed**: `115200` (หรือ `921600` เพื่อความรวดเร็ว)
   * **Flash Frequency**: `80MHz`
   * **Core Debug Level**: `None`

---

## ⚡ เทคนิคฮาร์ดแวร์และข้อควรระวัง (Hardware Tips)

### 1. จังหวะอัปโหลดโค้ด (Connecting... Loop)
* ขณะที่ Arduino IDE กำลังคอมไพล์และเริ่มอัปโหลด หากขึ้นข้อความ:
  ```text
  Connecting........_____........_____
  ```
* **วิธีแก้:** ให้**กดปุ่ม `BOOT` (หรือ `IO0`) บนบอร์ดค้างไว้ประมาณ 1-2 วินาที** ทันทีที่ข้อความเริ่มนับจุด จนกระทั่งขึ้นข้อความ `Writing at 0x00010000... (xx %)` จึงปล่อยมือ
* *เทคนิคเสริม:* สามารถต่อตัวเก็บประจุ (Capacitor) ค่า `10uF` หรือ `1uF` คร่อมระหว่างขา `EN` กับ `GND` เพื่อให้บอร์ดเข้าโหมด Auto-Flash ได้โดยไม่ต้องกดปุ่มเอง

### 2. หลังเบิร์นเสร็จแล้วบอร์ดนิ่ง / ไม่เริ่มรันโค้ด
* หากการอัปโหลดแสดงผล `Leaving... Hard resetting via RTS pin...` แต่ไฟ LED ไม่กะพริบหรือไม่ส่งข้อมูลผ่าน Serial Monitor
* **วิธีแก้:** ให้**กดปุ่ม `EN` (Reset) สั้นๆ 1 ครั้ง** เพื่อบังคับให้ชิป ESP32 เริ่ม Boot เข้าโปรแกรมใหม่

### 3. ข้อจำกัดของ ADC2 เมื่อเปิดใช้ Wi-Fi
* ขาแอนะล็อกของ ESP32 แบ่งออกเป็น 2 กลุ่มคือ ADC1 และ ADC2
* **คำเตือน:** พินกลุ่ม **ADC2** (เช่น GPIO 0, 2, 4, 12, 13, 14, 15, 25, 26, 27) ใช้งานวงจรฮาร์ดแวร์ร่วมกับไดรเวอร์ Wi-Fi ของ ESP32 **เมื่อมีการเรียกใช้คำสั่ง `WiFi.begin()` หรือ `WiFi.softAP()` ฟังก์ชัน `analogRead()` บนขา ADC2 จะใช้งานไม่ได้ (คืนค่าผิดพลาดหรือล้มเหลว)**
* **แนวทางปฏิบัติ:** หากต้องการอ่านค่าเซนเซอร์แบบแอนะล็อกร่วมกับ Wi-Fi ให้ต่อเข้ากับพินกลุ่ม **ADC1** เท่านั้น ได้แก่:
  * **GPIO 32, 33, 34, 35, 36 (VP), 39 (VN)** *(หมายเหตุ: ขา GPIO 34, 35, 36, 39 เป็น Input Only ไม่มี Pull-up/Pull-down ภายใน)*

### 4. ระดับแรงดันลอจิก 3.3V
* ขา GPIO ทุกขาของ ESP32 ทำงานที่ระดับแรงดัน **3.3V**
* ห้ามต่อเซนเซอร์หรืออุปกรณ์ที่ปล่อยระดับสัญญาณ 5V เข้าขา GPIO ตรงๆ เพราะจะทำให้ชิปเสียหายถาวร ให้ใช้วงจรแบ่งแรงดัน (Voltage Divider) หรือโมดูล Logic Level Shifter เสมอ

---

## 🚀 รายละเอียดโค้ดตัวอย่างในโปรเจกต์

### 1. `01_blink_test`
* ทดสอบการทำงานพื้นฐานและการควบคุมสัญญาณ Output ของชิป
* สั่งกะพริบไฟ LED สีฟ้าประจำบอร์ดที่ขา `GPIO 2` สลับเปิด-ปิดทุกๆ 1 วินาที พร้อมส่งสถานะออกทาง Serial Monitor (115200 bps)

### 2. `02_touch_sensor`
* ทดสอบการใช้งาน Capacitive Touch Sensor ในตัวของ ESP32
* ใช้พิน `GPIO 4` (Touch Channel 0) ตรวจจับประจุไฟฟ้าจากปลายนิ้วสัมผัส
* มีฟังก์ชันตรวจจับขอบสัญญาณ (Edge Detection) และ Debounce เพื่อสลับสถานะ (Toggle) เปิด-ปิดไฟ LED ได้อย่างแม่นยำ

### 3. `03_wifi_webserver_ap`
* สร้าง Wi-Fi Access Point เฉพาะกิจ (SoftAP) ชื่อ **"ESP32-SmartHome"**
* ให้บริการ Web Server ที่พอร์ต 80 โดยมี IP Address ประจำบอร์ดคือ **`http://192.168.4.1`**
* หน้าเว็บ Responsive ดีไซน์สวยงาม รองรับการควบคุมจากสมาร์ตโฟนและคอมพิวเตอร์ เพื่อสั่งเปิด-ปิดไฟได้แบบ Real-time

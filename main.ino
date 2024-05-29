#include <U8g2lib.h>
#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>

#define WIFI_SSID "tro 1"
#define WIFI_PASSWORD "01010101"

#define FIREBASE_HOST "casio-5e6f3-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "AIzaSyCnbruR9nVKIbkSZTpYtp4RWNRiHQ9R0KM"

FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;


#define OLED_RESET -1
U8G2_SH1106_128X64_NONAME_F_HW_I2C display(U8G2_R0, /* reset=*/ OLED_RESET);

// Đường dẫn đến dữ liệu trên Realtime Database
#define DATABASE_PATH "test"

void setup() {
  display.begin();
  display.setFont(u8g2_font_6x10_tr);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    display.clearBuffer();
    display.drawStr(10, 10, ".");
    display.sendBuffer();
  }
  display.clearBuffer();
  display.drawStr(10, 10, "WiFi connected");
  display.sendBuffer();
  config.database_url = FIREBASE_HOST;
  config.api_key = FIREBASE_AUTH;
  Firebase.signUp(&config, &auth, "", "");
  Firebase.begin(&config, &auth);
}

void loop() {
  // Kiểm tra kết nối Firebase
  if (Firebase.ready()) {
    // Lấy dữ liệu từ Realtime Database
    if (Firebase.getString(firebaseData, DATABASE_PATH)) {
      // Lấy giá trị của dữ liệu
      String value = firebaseData.stringData();

      // Hiển thị giá trị trên màn hình OLED
      display.clearBuffer();
      display.drawStr(0, 10, value.c_str());
      display.sendBuffer();
    } else {
      display.clearBuffer();
      display.drawStr(10, 10, "Failed to get data");
      display.sendBuffer();
    }
  } else {
    display.clearBuffer();
    display.drawStr(10, 10, "Firebase is not ready");
    display.sendBuffer();
  }

  // Đợi 1 giây trước khi cập nhật dữ liệu
  delay(1000);
}

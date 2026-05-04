#pragma once 
#include "Arduino.h"
#include "IotVision.h"
#define I2C_SDA 21
#define I2C_SCL 22

void recoverI2C() {
  Wire.end(); 

  pinMode(I2C_SCL, OUTPUT);
  pinMode(I2C_SDA, INPUT_PULLUP);

  DBG("Đang kiểm tra Bus I2C...");

  if (digitalRead(I2C_SDA) == LOW) {
    DBG("Phát hiện SDA bị treo LOW. Đang gửi xung Clock phục hồi...");
    
    // Gửi tối đa 9 xung clock
    for (int i = 0; i < 9; i++) {
      digitalWrite(I2C_SCL, LOW);
      delayMicroseconds(5);
      digitalWrite(I2C_SCL, HIGH);
      delayMicroseconds(5);

      // Nếu SDA đã quay lại mức HIGH, dừng lại ngay
      if (digitalRead(I2C_SDA) == HIGH) {
        DBG("SDA đã được giải phóng!");
        break;
      }
    }
  }

  // 4. Tạo điều kiện STOP (SDA chuyển từ LOW sang HIGH trong khi SCL đang HIGH)
  pinMode(I2C_SDA, OUTPUT);
  digitalWrite(I2C_SDA, LOW);
  delayMicroseconds(5);
  digitalWrite(I2C_SCL, HIGH);
  delayMicroseconds(5);
  digitalWrite(I2C_SDA, HIGH);
  delayMicroseconds(5);

  // 5. Khởi động lại bộ I2C phần cứng
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setTimeOut(50); // Đặt timeout để tránh treo trong tương lai
  DBG("Bus I2C đã khởi động lại.");
}
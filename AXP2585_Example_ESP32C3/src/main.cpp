#include <Arduino.h>
#include "Wire.h"


volatile bool irqChanged = false;
volatile int irqState = LOW;

auto irqPin = 6;

void irqInterrupt() {
  irqState = digitalRead(irqPin);
  irqChanged = true;
}

// put function declarations here:
int myFunction(int, int);

void setup() {
  Serial.begin(115200);
  Wire.begin(11, 7); // SDA, SCL ->> C3 的 IO11 是 VDD_SPI 不能用
  pinMode(irqPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(7), irqInterrupt, CHANGE);
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
}

void loop() {
  Serial.println("Hello, World!");
  delay(1000);

  byte error, address;
  int nDevices = 0;

  if (irqChanged) {
    if (irqState == HIGH) {
      Serial.println("引脚 IRQ 从 LOW 变为 HIGH");
    } else {
      Serial.println("引脚 IRQ 从 HIGH 变为 LOW");
    }
    irqState = false;
  }

  Serial.println("正在扫描 I2C 设备 ...");
  for (address = 0x01; address < 0x7f; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.printf("在地址 0x%02X 发现 I2C 设备\n", address);
      nDevices++;
    } else if (error != 2 && error != 5) {
      Serial.printf("在地址 0x%02X 发生错误 %d\n", address, error);
    }
  }
  if (nDevices == 0) {
    Serial.println("未发现 I2C 设备");
  }
  delay(1000);
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}
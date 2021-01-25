#include <M5Stack.h>

#define Faces_Encoder_I2C_ADDR     0X5E

extern uint8_t rabbit_1[];
extern uint8_t rabbit_2[];
extern uint8_t rabbit_3[];
extern uint8_t wortel[];
extern uint8_t splash[];

uint8_t last_button, cur_button;


void GetValue(void) {
  int temp_encoder_increment;

  Wire.requestFrom(Faces_Encoder_I2C_ADDR, 3);
  if (Wire.available()) {
    temp_encoder_increment = Wire.read();
    cur_button = Wire.read();
  }

}

int getBatteryLevel()
{
  Wire.beginTransmission(0x75);
  Wire.write(0x78);
  if (Wire.endTransmission(false) == 0
      && Wire.requestFrom(0x75, 1)) {
    switch (Wire.read() & 0xF0) {
      case 0xE0: return 25;
      case 0xC0: return 50;
      case 0x80: return 75;
      case 0x00: return 100;
      default: return 0;
    }
  }
  return -1;
}


void Led(int i, int r, int g, int b) {
  Wire.beginTransmission(Faces_Encoder_I2C_ADDR);
  Wire.write(i);
  Wire.write(r);
  Wire.write(g);
  Wire.write(b);
  Wire.endTransmission();
}

void colorWoosh(int r, int g, int b, int del) {
  for (int i = 0; i < 12; i++)
  {
    Led(i, r, g, b);
    delay(del);
  }

}

void setup() {
  // put your setup code here, to run once:
  M5.begin();                   // M5STACK INITIALIZE
  M5.Power.begin();
  M5.Lcd.setBrightness(200);    // BRIGHTNESS = MAX 255
  M5.Lcd.fillScreen(WHITE);

  Wire.begin();

  colorWoosh(0, 255, 0, 10);
  colorWoosh(0, 0, 0, 10);


  M5.Lcd.drawJpg(splash, 100228);
  dacWrite(25, 0);
}

void loop() {
  M5.update();
  GetValue();

  if (last_button != cur_button) {
    last_button = cur_button;
  }
  if (cur_button) {
    colorWoosh(0, 0, 0, 0);
  } else {
    M5.Lcd.clear(WHITE);

    colorWoosh(0, 0, 0, 0);
    colorWoosh(255, 0, 0, 15);
    colorWoosh(0, 255, 0, 15);
    colorWoosh(0, 0, 255, 15);
    colorWoosh(0, 0, 0, 15);


    int rnd = random(1, 44);
    if (rnd < 18) {
      M5.Lcd.drawJpg(rabbit_1, 27870);
    } else if (rnd < 29) {
      M5.Lcd.drawJpg(rabbit_2, 31355);

    } else if (rnd < 34) {
      M5.Lcd.drawJpg(rabbit_3, 32176);

    } else {

      M5.Lcd.drawJpg(wortel, 30150);
    }
  }

  if (M5.BtnB.wasReleased() || M5.BtnB.pressedFor(1000, 200)) {
    M5.Lcd.clear();
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.setTextSize(5);
    M5.Lcd.printf("Batterij: %d%%", getBatteryLevel());
  }

}

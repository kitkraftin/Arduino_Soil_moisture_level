#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define SOIL_MOISTURE_PIN A0

void drawFace(int x, int y, int mood) {
  // Bigger circular face
  display.drawCircle(x, y, 20, SSD1306_WHITE); // Face outline
  display.fillCircle(x - 6, y - 6, 4, SSD1306_WHITE); // Left eye
  display.fillCircle(x + 6, y - 6, 4, SSD1306_WHITE); // Right eye

  if (mood == 1) { // 😊 Happy (Moisture > 60%)
    display.drawLine(x - 7, y + 8, x - 3, y + 10, SSD1306_WHITE);
    display.drawLine(x - 3, y + 10, x + 3, y + 10, SSD1306_WHITE);
    display.drawLine(x + 3, y + 10, x + 7, y + 8, SSD1306_WHITE);
  } else if (mood == 0) { // 😐 Neutral (30-60%)
    display.drawLine(x - 7, y + 10, x + 7, y + 10, SSD1306_WHITE);
  } else { // ☹️ Sad (Moisture < 30%)
    display.drawLine(x - 7, y + 10, x - 3, y + 8, SSD1306_WHITE);
    display.drawLine(x - 3, y + 8, x + 3, y + 8, SSD1306_WHITE);
    display.drawLine(x + 3, y + 8, x + 7, y + 10, SSD1306_WHITE);
  }
}

void setup() {
  pinMode(SOIL_MOISTURE_PIN, INPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;); // Stop if OLED fails to initialize
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(5, 10);
  display.print("KitKraft Soil Sensor");
  display.display();
  delay(2000);
}

void loop() {
  int moistureValue = analogRead(SOIL_MOISTURE_PIN);
  int percentage = map(moistureValue, 1023, 300, 0, 100);

  display.clearDisplay();

  // Display moisture level
  display.setTextSize(1); 
  display.setCursor(10, 10);
  display.print("Moisture:");
  
  display.setTextSize(3);  // % value
  display.setCursor(10, 30);
  display.print(percentage);
  display.print("%");

  // Display face based on moisture level
  if (percentage > 60) {
    drawFace(100, 30, 1); // Happy 😊
  } else if (percentage >= 30) {
    drawFace(100, 30, 0); // Neutral 😐
  } else {
    drawFace(100, 30, -1); // Sad ☹️
  }

  display.display();
  delay(2000);
}
